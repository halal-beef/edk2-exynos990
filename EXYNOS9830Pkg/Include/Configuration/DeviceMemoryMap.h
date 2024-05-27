#ifndef _DEVICE_MEMORY_MAP_H_
#define _DEVICE_MEMORY_MAP_H_

#include <Library/ArmLib.h>

#define MAX_ARM_MEMORY_REGION_DESCRIPTOR_COUNT 128

/* Below flag is used for system memory */
#define SYSTEM_MEMORY_RESOURCE_ATTR_CAPABILITIES                               \
  EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED |        \
      EFI_RESOURCE_ATTRIBUTE_TESTED | EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |     \
      EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |                               \
      EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |                         \
      EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |                            \
      EFI_RESOURCE_ATTRIBUTE_EXECUTION_PROTECTABLE

typedef enum { NoHob, AddMem, AddDev, HobOnlyNoCacheSetting, MaxMem } DeviceMemoryAddHob;

#define MEMORY_REGION_NAME_MAX_LENGTH 32

typedef struct {
  CHAR8                        Name[MEMORY_REGION_NAME_MAX_LENGTH];
  EFI_PHYSICAL_ADDRESS         Address;
  UINT64                       Length;
  DeviceMemoryAddHob           HobOption;
  EFI_RESOURCE_TYPE            ResourceType;
  EFI_RESOURCE_ATTRIBUTE_TYPE  ResourceAttribute;
  EFI_MEMORY_TYPE              MemoryType;
  ARM_MEMORY_REGION_ATTRIBUTES ArmAttributes;
} ARM_MEMORY_REGION_DESCRIPTOR_EX, *PARM_MEMORY_REGION_DESCRIPTOR_EX;

#define MEM_RES EFI_RESOURCE_MEMORY_RESERVED
#define MMAP_IO EFI_RESOURCE_MEMORY_MAPPED_IO
#define SYS_MEM EFI_RESOURCE_SYSTEM_MEMORY

#define SYS_MEM_CAP SYSTEM_MEMORY_RESOURCE_ATTR_CAPABILITIES
#define INITIALIZED EFI_RESOURCE_ATTRIBUTE_INITIALIZED
#define WRITE_COMBINEABLE EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE
#define UNCACHEABLE EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE

#define Reserv EfiReservedMemoryType
#define Conv EfiConventionalMemory
#define BsData EfiBootServicesData
#define BsCode EfiBootServicesCode
#define RtData EfiRuntimeServicesData
#define RtCode EfiRuntimeServicesCode
#define MmIO EfiMemoryMappedIO

#define DEVICE ARM_MEMORY_REGION_ATTRIBUTE_DEVICE
#define WRITE_THROUGH ARM_MEMORY_REGION_ATTRIBUTE_WRITE_THROUGH
#define WRITE_THROUGH_XN ARM_MEMORY_REGION_ATTRIBUTE_WRITE_THROUGH
#define WRITE_BACK ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK
#define WRITE_BACK_XN ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK
#define UNCACHED_UNBUFFERED ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED
#define UNCACHED_UNBUFFERED_XN ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED

static ARM_MEMORY_REGION_DESCRIPTOR_EX gDeviceMemoryDescriptorEx[] = {
/*                                                    EFI_RESOURCE_ EFI_RESOURCE_ATTRIBUTE_ EFI_MEMORY_TYPE ARM_REGION_ATTRIBUTE_
     MemLabel(32 Char.),  MemBase,    MemSize, BuildHob, ResourceType, ResourceAttribute, MemoryType, CacheAttributes
*/

//----------------------- DDR ------------------------

    {"SEC Debug",         0x80000000, 0x00001000, AddMem, SYS_MEM, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED},
    {"HLOS 0",            0x80001000, 0x00BFF000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN},
    {"UEFI Stack",        0x80C00000, 0x00040000, AddMem, SYS_MEM, SYS_MEM_CAP,  BsData, WRITE_BACK},
    {"CPU Vectors",       0x80C40000, 0x00010000, AddMem, SYS_MEM, SYS_MEM_CAP,  BsCode, WRITE_BACK},
    {"HLOS 1",            0x80C50000, 0x0F3B0000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN},
    {"UEFI FD",           0x90000000, 0x0B200000, AddMem, SYS_MEM, SYS_MEM_CAP, BsCode, WRITE_BACK},
    {"HLOS 2",            0x9B200000, 0x1F900000, AddMem, SYS_MEM, SYS_MEM_CAP, BsCode, WRITE_BACK},

    // Memory Hole: 0xBAB00000 -> 0xC1200000 (0x6700000)

    {"HLOS 3",            0xC1200000, 0x01E00000, AddMem, SYS_MEM, SYS_MEM_CAP, BsCode, WRITE_BACK},
    {"SEC Log",           0xC3000000, 0x00080000, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK},
    {"HLOS 4",            0xC3080000, 0x1CF80000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},

    // Memory Hole: 0xE0000000 -> 0xE1900000 (0x01900000)

    {"HLOS 5",            0xE1900000, 0x0F700000, AddMem, SYS_MEM, SYS_MEM_CAP,  Conv,   WRITE_BACK},
    {"Display Reserved",  0xF1000000, 0x01194000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_THROUGH_XN},
    {"HLOS 6",            0xF2194000, 0x05E1C000, AddMem, SYS_MEM, SYS_MEM_CAP,  Conv,   WRITE_BACK},
    {"ABOX",              0xF7FB0000, 0x02A50000, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK},
    {"HLOS 7",            0xFAA00000, 0x05600000, AddMem, SYS_MEM, SYS_MEM_CAP,  Conv,   WRITE_BACK},

    // Memory Hole: 0x100000000 -> 0x880000000 (0x780000000)

    {"DXE Heap",          0x880000000, 0x2C000000, AddMem, SYS_MEM, SYS_MEM_CAP,  Conv,   WRITE_BACK},
    {"HLOS 8",            0x8AC000000, 0x234000000, AddMem, SYS_MEM, SYS_MEM_CAP,  Conv,   WRITE_BACK},


//--------------------- Register ---------------------

    {"GIC Distributor",   0x10101000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, DEVICE},
    {"GIC Redistributors", 0x10102000, 0x00006000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, DEVICE},
    {"PinCtrl 1",         0x10430000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, DEVICE},
    {"PinCtrl 2",         0x10730000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, DEVICE},
    {"PinCtrl 3",         0x13040000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, DEVICE},
    {"PinCtrl 4",         0x15580000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, DEVICE},
    {"PinCtrl 5",         0x15850000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, DEVICE},
    {"SPEEDY 0",          0x15940000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, DEVICE},
    {"SPEEDY 1",          0x15950000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, DEVICE},
    {"PinCtrl 6",         0x15C30000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, DEVICE},

    /* Terminator for MMU */
    { "Terminator", 0, 0, 0, 0, 0, 0, 0}};

#endif
