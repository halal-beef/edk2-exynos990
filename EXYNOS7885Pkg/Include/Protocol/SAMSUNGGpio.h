#ifndef __SAMSUNG_GPIO_H__
#define __SAMSUNG_GPIO_H__

#define SAMSUNG_GPIO_GUID                                             \
  {                                                                            \
    0x2c898318, 0x41c1, 0x4309,                                                \
    {                                                                          \
      0x89, 0x8a, 0x2f, 0x55, 0xc8, 0xcf, 0x0b, 0x87                           \
    }                                                                          \
  }

//
// Protocol interface structure
//
typedef struct _SAMSUNG_GPIO SAMSUNG_GPIO;

typedef
UINT32
(*GPIO_GET)(
  CHAR8 *bank, UINT32 bankNumber, UINT32 gpioNumber
  );

struct _SAMSUNG_GPIO {
  GPIO_GET         Get;
};

extern EFI_GUID  gSAMSUNGGpioProtocolGuid;

#endif