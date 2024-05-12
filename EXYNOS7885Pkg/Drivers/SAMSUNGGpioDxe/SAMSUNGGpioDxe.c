/*
 * Copyright (c) 2024, halal-beef <halalbeef.stuff@gmail.com>
 * Based on the open source driver from edk2-tensor and the key reading code from the uniLoader fork by BotchedRPR
 * Contributors: Predefine
 */

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/SAMSUNGGpio.h>

UINT32 bankOffset = FixedPcdGet32(BankOffset);
UINT32 gpaBase = FixedPcdGet32(GpaBase);
UINT32 gpbBase = FixedPcdGet32(GpbBase);
UINT32 gpcBase = FixedPcdGet32(GpcBase);
UINT32 gpfBase = FixedPcdGet32(GpfBase);
UINT32 gpgBase = FixedPcdGet32(GpgBase);
UINT32 gpmBase = FixedPcdGet32(GpmBase);
UINT32 gppBase = FixedPcdGet32(GppBase);
UINT32 gpqBase = FixedPcdGet32(GpqBase);
UINT32 gpvBase = FixedPcdGet32(GpvBase);

/*
* Gets a GPIO basked on bank like A,B,F... , bank number 0,1,2... and the gpio number 0,1,2...
*/

UINT32
gpio_get(CHAR8 *bank, UINT32 bankNumber, UINT32 gpioNumber)
{
    UINT32 bankOff = (bankOffset * bankNumber) + 0x4;
    UINT32 val = -1;


    if(bank == "A") val = (MmioRead32(gpaBase + bankOff) >> gpioNumber) & 0x1;
    else if (bank == "B") val = (MmioRead32(gpbBase + bankOff) >> gpioNumber) & 0x1;
    else if (bank == "C") val = (MmioRead32(gpcBase + bankOff) >> gpioNumber) & 0x1;
    else if (bank == "F") val = (MmioRead32(gpfBase + bankOff) >> gpioNumber) & 0x1;
    else if (bank == "G") val = (MmioRead32(gpgBase + bankOff) >> gpioNumber) & 0x1;
    else if (bank == "M") val = (MmioRead32(gpmBase + bankOff) >> gpioNumber) & 0x1;
    else if (bank == "P") val = (MmioRead32(gppBase + bankOff) >> gpioNumber) & 0x1;
    else if (bank == "Q") val = (MmioRead32(gpqBase + bankOff) >> gpioNumber) & 0x1;
    else if (bank == "V") val = (MmioRead32(gpvBase + bankOff) >> gpioNumber) & 0x1;

    return val;
}

SAMSUNG_GPIO  gSAMSUNGGpio = {
  gpio_get
};

EFI_STATUS
EFIAPI
SAMSUNGGpioDxeInitialize(
	IN EFI_HANDLE         ImageHandle,
	IN EFI_SYSTEM_TABLE   *SystemTable
)
{
  EFI_STATUS  Status = EFI_SUCCESS;
  EFI_HANDLE  Handle = NULL;

  //
  // Make sure the SAMSUNG Gpio protocol has not been installed in the system yet.
  //
  ASSERT_PROTOCOL_ALREADY_INSTALLED (NULL, &gSAMSUNGGpioProtocolGuid);

  // Install the GPIO Protocol onto a new handle
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gSAMSUNGGpioProtocolGuid,
                  &gSAMSUNGGpio,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    Status = EFI_OUT_OF_RESOURCES;
  }

	return Status;
}
