/*
 * Copyright (c) 2024, halal-beef <halalbeef.stuff@gmail.com>
 * Based on the open source driver from Linaro
 */

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/TimerLib.h>

#include <Protocol/SPEEDY.h>

#include <Drivers/SPEEDY_values.h>

static void speedy_set_enable(UINT32 SPEEDY_BASE)
{
    UINT32 speedy_ctl = MmioRead32(SPEEDY_BASE + SPEEDY_CTRL);

    speedy_ctl |= SPEEDY_ENABLE;
    MmioWrite32(SPEEDY_BASE + SPEEDY_CTRL, speedy_ctl);
}


static void speedy_fifo_reset(UINT32 SPEEDY_BASE)
{
    UINT32 speedy_fifo_ctl = MmioRead32(SPEEDY_BASE + SPEEDY_FIFO_CTRL);

    speedy_fifo_ctl |= SPEEDY_FIFO_RESET;
    MmioWrite32(SPEEDY_BASE + SPEEDY_FIFO_CTRL, speedy_fifo_ctl);
    /* Wait for SPEEDY fifo to reset */
    MicroSecondDelay(10);
}

static void speedy_set_cmd(UINT32 SPEEDY_BASE, INT32 direction, UINT16 address, INT32 random, INT32 burst_length)
{
    UINT32 speedy_fifo_ctl = 0;
    UINT32 speedy_int_en = 0;
    UINT32 speedy_command = 0;

    speedy_fifo_reset(SPEEDY_BASE);
    speedy_fifo_ctl = MmioRead32(SPEEDY_BASE + SPEEDY_FIFO_CTRL);

    speedy_command |= SPEEDY_ADDRESS(address);

    switch(random)
    {
        case ACCESS_RANDOM:
            speedy_command |= SPEEDY_ACCESS_RANDOM;
            speedy_fifo_ctl |= (SPEEDY_RX_TRIGGER_LEVEL(1) |
                                SPEEDY_TX_TRIGGER_LEVEL(1));
            break;
        case ACCESS_BURST:
            speedy_command |= (SPEEDY_ACCESS_BURST | SPEEDY_BURST_INCR |
                               SPEEDY_BURST_LENGTH(burst_length - 1));

            speedy_fifo_ctl |= (
                SPEEDY_RX_TRIGGER_LEVEL(burst_length) |
                SPEEDY_TX_TRIGGER_LEVEL(1));
            break;
        default:
            DEBUG((EFI_D_ERROR, "[SPEEDY] Unsupported Access Mode!"));
            break;
    }

    /* Configure SPEEDY FIFO trigger level */
    MmioWrite32(SPEEDY_BASE + SPEEDY_FIFO_CTRL, speedy_fifo_ctl);

    speedy_int_en |= (SPEEDY_TIMEOUT_CMD_EN | SPEEDY_TIMEOUT_STANDBY_EN |
                      SPEEDY_TIMEOUT_DATA_EN);

    switch(direction)
    {
        case DIRECTION_READ:
	    speedy_command |= SPEEDY_DIRECTION_READ;
	    speedy_int_en |= (SPEEDY_FIFO_RX_ALMOST_FULL_EN |
	  		      SPEEDY_RX_FIFO_INT_TRAILER_EN |
			      SPEEDY_RX_MODEBIT_ERR_EN |
			      SPEEDY_RX_GLITCH_ERR_EN |
			      SPEEDY_RX_ENDBIT_ERR_EN |
			      SPEEDY_REMOTE_RESET_REQ_EN);
            break;

        case DIRECTION_WRITE:
	    speedy_command |= SPEEDY_DIRECTION_WRITE;
	    speedy_int_en |= (SPEEDY_TRANSFER_DONE_EN |
	    		      SPEEDY_FIFO_TX_ALMOST_EMPTY_EN |
			      SPEEDY_TX_LINE_BUSY_ERR_EN |
			      SPEEDY_TX_STOPBIT_ERR_EN |
			      SPEEDY_REMOTE_RESET_REQ_EN);
            break;

        default:
            DEBUG((EFI_D_ERROR, "[SPEEDY] Unsupported Direction!"));
            break;
    }

    /* clear SPEEDY interrupt status */
    MmioWrite32(SPEEDY_BASE + SPEEDY_INT_STATUS, 0xFFFFFFFF);

    /* enable SPEEDY interrupts */
    MmioWrite32(SPEEDY_BASE + SPEEDY_INT_ENABLE, speedy_int_en);

    /* configure SPEEDY command */
    MmioWrite32(SPEEDY_BASE + SPEEDY_CMD, speedy_command);
}

static int speedy_wait_transfer_done(UINT32 SPEEDY_BASE)
{
    UINT32 speedy_int_status;
    UINT64 timeout = 1000;
    INT32 ret = 0;

    while (timeout-- > 0)
    {
        speedy_int_status = MmioRead32(SPEEDY_BASE + SPEEDY_INT_STATUS);

        if (speedy_int_status & SPEEDY_TRANSFER_DONE)
        {
            ret = 0;
            break;
        }
    }

    if (timeout == 0)
    {
        ret = -1;
    }

    /* Clear SPEEDY interrupt status */
    MmioWrite32(SPEEDY_BASE + SPEEDY_INT_STATUS, 0xFFFFFFFF);

    return ret;
}

static void speedy_swreset(UINT32 SPEEDY_BASE)
{
    UINT32 speedy_ctl = MmioRead32(SPEEDY_BASE + SPEEDY_CTRL);

    speedy_ctl |= SPEEDY_SW_RST;
    MmioWrite32(SPEEDY_BASE + SPEEDY_CTRL, speedy_ctl);

    /* Wait for SPEEDY to reset */
    MicroSecondDelay(10);
}

static void speedy_init(UINT32 SPEEDY_BASE)
{
    /* Clear SPEEDY interrupt status */
    MmioWrite32(SPEEDY_BASE + SPEEDY_INT_STATUS, 0xFFFFFFFF);

    /* Reset SPEEDY, it may be in use by BL */
    speedy_swreset(SPEEDY_BASE);
    speedy_set_enable(SPEEDY_BASE);
}

EFI_STATUS speedy_read(UINT32 SPEEDY_BASE, UINT8 slave, UINT8 addr, UINT8 *data)
{
    EFI_STATUS status = EFI_SUCCESS;
    INT32 result = 0;
    UINT16 address = addr + ((slave & 0xF) << 8);

    speedy_set_cmd(SPEEDY_BASE, DIRECTION_READ, address, ACCESS_RANDOM, 1);

    result = speedy_wait_transfer_done(SPEEDY_BASE);

    if (result == 0)
    {
        *data = (unsigned char)MmioRead32(SPEEDY_BASE + SPEEDY_RX_DATA);
    }
    else
    {
        DEBUG((EFI_D_ERROR, "Timeout on read!\n"));
	status = EFI_TIMEOUT;
    }

    return status;
}

EFI_STATUS speedy_read_burst(UINT32 SPEEDY_BASE, UINT8 slave, UINT8 addr, UINT8 *data, UINT8 count)
{
    EFI_STATUS status = EFI_SUCCESS;
    INT32 result = 0;
    UINT16 address = addr + ((slave & 0xF) << 8);
    INT32 i;

    speedy_set_cmd(SPEEDY_BASE, DIRECTION_READ, address, ACCESS_BURST, count);

    result = speedy_wait_transfer_done(SPEEDY_BASE);

    if (result == 0)
    {
        for (i = 0; i < count; i++)
            data[i] = (unsigned char)MmioRead32(SPEEDY_BASE + SPEEDY_RX_DATA);
    }
    else
    {
        DEBUG((EFI_D_ERROR, "Timeout on burst read!\n"));
	status = EFI_TIMEOUT;
    }

    return status;
}

EFI_STATUS speedy_write(UINT32 SPEEDY_BASE, UINT8 slave, UINT8 addr, UINT8 data)
{
    EFI_STATUS status = EFI_SUCCESS;
    INT32 result = 0;
    UINT16 address = addr + ((slave & 0xF) << 8);

    speedy_set_cmd(SPEEDY_BASE, DIRECTION_WRITE, address, ACCESS_RANDOM, 1);
    MmioWrite32(SPEEDY_BASE + SPEEDY_TX_DATA, data);

    result = speedy_wait_transfer_done(SPEEDY_BASE);

    if (result != 0)
    {
        DEBUG((EFI_D_ERROR, "Timeout on write!\n"));
	status = EFI_TIMEOUT;
    }

    return status;
}

EFI_STATUS speedy_write_burst(UINT32 SPEEDY_BASE, UINT8 slave, UINT8 addr, UINT8 *data, UINT8 count)
{
    EFI_STATUS status = EFI_SUCCESS;
    INT32 result = 0;
    UINT16 address = addr + ((slave & 0xF) << 8);
    INT32 i;

    speedy_set_cmd(SPEEDY_BASE, DIRECTION_WRITE, address, ACCESS_RANDOM, 1);
    for (i = 0; i < count; i++)
        MmioWrite32(SPEEDY_BASE + SPEEDY_TX_DATA, data[i]);

    result = speedy_wait_transfer_done(SPEEDY_BASE);

    if (result != 0) 
    {
        DEBUG((EFI_D_ERROR, "Timeout on burst write!\n"));
	status = EFI_TIMEOUT;
    }

    return status;
}

SPEEDY_PROTOCOL gSPEEDYProtocol = {
  speedy_write,
  speedy_write_burst,
  speedy_read,
  speedy_read_burst
};


EFI_STATUS
EFIAPI
SPEEDYDxeInitialize(
	IN EFI_HANDLE         ImageHandle,
	IN EFI_SYSTEM_TABLE   *SystemTable
)
{
	EFI_STATUS  Status = EFI_SUCCESS;
	EFI_HANDLE Handle = NULL;
        UINT32 SPEEDY0_BASE = FixedPcdGet32(PcdSPEEDY0Base);
        UINT32 SPEEDY1_BASE = FixedPcdGet32(PcdSPEEDY1Base);

	  Status = gBS->InstallMultipleProtocolInterfaces(
      &Handle, &gSPEEDYProtocolGuid, &gSPEEDYProtocol, NULL);

	speedy_init(SPEEDY0_BASE);
	speedy_init(SPEEDY1_BASE);

	DEBUG((EFI_D_ERROR, "SPEEDY initialised!"));

	ASSERT_EFI_ERROR(Status);

	return Status;
}
