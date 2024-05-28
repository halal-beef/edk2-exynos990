[Defines]
  PLATFORM_NAME                  = EXYNOS9830Pkg
  PLATFORM_GUID                  = 28f1a3bf-193a-47e3-a7b9-5a435eaab2ee
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010019
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = EXYNOS9830Pkg/EXYNOS9830Pkg.fdf

!include EXYNOS9830Pkg/EXYNOS9830Pkg.dsc

[PcdsFixedAtBuild.common]
  # System Memory (12GB)
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x300000000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x80C00000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000      # 256K stack
  gEXYNOS9830PkgTokenSpaceGuid.PcdUefiMemPoolBase|0x80D00000         # DXE Heap base address
  gEXYNOS9830PkgTokenSpaceGuid.PcdUefiMemPoolSize|0x03300000         # UefiMemorySize, DXE heap size
  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x80C40000

  # Framebuffer (1440x3200)
  gEXYNOS9830PkgTokenSpaceGuid.PcdMipiFrameBufferAddress|0x0F1000000
  gEXYNOS9830PkgTokenSpaceGuid.PcdMipiFrameBufferWidth|1440
  gEXYNOS9830PkgTokenSpaceGuid.PcdMipiFrameBufferHeight|3200
  gEXYNOS9830PkgTokenSpaceGuid.PcdMipiFrameBufferVisibleWidth|1440
  gEXYNOS9830PkgTokenSpaceGuid.PcdMipiFrameBufferVisibleHeight|3200

  # SPEEDY Protocol
  gEXYNOS9830PkgTokenSpaceGuid.PcdSPEEDY0Base|0x15940000
  gEXYNOS9830PkgTokenSpaceGuid.PcdSPEEDY1Base|0x15950000

  # Samsung GPIO Protocol
  gEXYNOS9830PkgTokenSpaceGuid.BankOffset|0x20
  gEXYNOS9830PkgTokenSpaceGuid.GpaBase|0x15850000
  gEXYNOS9830PkgTokenSpaceGuid.GpbBase|0x10730000
  gEXYNOS9830PkgTokenSpaceGuid.GpcBase|0x10730000
  gEXYNOS9830PkgTokenSpaceGuid.GpfBase|0x13040000
  gEXYNOS9830PkgTokenSpaceGuid.GpgBase|0x10430000
  gEXYNOS9830PkgTokenSpaceGuid.GpmBase|0x15C30000
  gEXYNOS9830PkgTokenSpaceGuid.GppBase|0x10430000
  gEXYNOS9830PkgTokenSpaceGuid.GpqBase|0x15850000
  gEXYNOS9830PkgTokenSpaceGuid.GpvBase|0x15580000
