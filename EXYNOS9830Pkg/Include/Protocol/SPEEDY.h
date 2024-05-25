#ifndef __SPEEDY_H__
#define __SPEEDY_H__

#define SPEEDY_PROTOCOL_GUID                                                   \
  {                                                                            \
    0x2c898318, 0x41c1, 0x4309,                                                \
    {                                                                          \
      0x89, 0x8a, 0x2f, 0x55, 0xc8, 0xcf, 0x0b, 0x86                           \
    }                                                                          \
  }

typedef struct _SPEEDY_PROTOCOL SPEEDY_PROTOCOL;

typedef EFI_STATUS(*speedy_write_t)(UINT32 SPEEDY_BASE, UINT8 slave, UINT8 addr, UINT8 data);
typedef EFI_STATUS(*speedy_write_burst_t)(UINT32 SPEEDY_BASE, UINT8 slave, UINT8 addr, UINT8 *data, UINT8 count);
typedef EFI_STATUS(*speedy_read_t)(UINT32 SPEEDY_BASE, UINT8 slave, UINT8 addr, UINT8 *data);
typedef EFI_STATUS(*speedy_read_burst_t)(UINT32 SPEEDY_BASE, UINT8 slave, UINT8 addr, UINT8 *data, UINT8 count);

struct _SPEEDY_PROTOCOL {
  speedy_write_t  Write;
  speedy_write_burst_t  Write_Burst;
  speedy_read_t  Read;
  speedy_read_burst_t  Read_Burst;
};

extern EFI_GUID gSPEEDYProtocolGuid;

#endif //__SPEEDY_H__
