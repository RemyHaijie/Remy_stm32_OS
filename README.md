# remy stm32 os
介绍 : stm32 Remy OS 是一个很基础的操作系统框架，包含了在PendSV中进行上下文切换、在SVC中进行系统内核调用、使用 操作系统延迟提高代码效率的功能。旨在给初学者一个参考和借鉴的作用

声明 : stm32 Remy OS 是我的一个程序编写小作业，也是第一个上传github的作品。 上传github的目的是为了给学习cortex M 内核的同学们一些指引，为编写操作系统内核的同学们一个启蒙性的借鉴。 本操作系统参考书物为《cortex M3/M4 权威指南》
目录结构如下.
├── doc
│   └── README.md
├── MDK_OUTPUT
├── MDK-PRO
│   ├── Listings
│   │   ├── os_kernel.lst
│   │   ├── profile.map
│   │   └── startup_stm32f40_41xxx.lst
│   ├── Objects
│   ├── profile.uvguix.Administrator
│   ├── profile.uvguix.Administrator.bak
│   ├── profile.uvoptx
│   └── profile.uvprojx
├── office_inc
│   ├── CMSIS
│   │   ├── arm_common_tables.h
│   │   ├── arm_const_structs.h
│   │   ├── arm_math.h
│   │   ├── core_cm0.h
│   │   ├── core_cm0plus.h
│   │   ├── core_cm3.h
│   │   ├── core_cm4.h
│   │   ├── core_cm4_simd.h
│   │   ├── core_cmFunc.h
│   │   ├── core_cmInstr.h
│   │   ├── core_sc000.h
│   │   ├── core_sc300.h
│   │   ├── fonts.h
│   │   ├── lcd_log_conf_template.h
│   │   ├── lcd_log.h
│   │   └── stm32_eval_legacy.h
│   ├── MDK-ARM
│   │   └── stm32f4xx.h
│   ├── STM32_StdPeriph_Driver
│   │   ├── misc.h
│   │   ├── stm32f4xx_adc.h
│   │   ├── stm32f4xx_can.h
│   │   ├── stm32f4xx_crc.h
│   │   ├── stm32f4xx_cryp.h
│   │   ├── stm32f4xx_dac.h
│   │   ├── stm32f4xx_dbgmcu.h
│   │   ├── stm32f4xx_dcmi.h
│   │   ├── stm32f4xx_dma2d.h
│   │   ├── stm32f4xx_dma.h
│   │   ├── stm32f4xx_exti.h
│   │   ├── stm32f4xx_flash.h
│   │   ├── stm32f4xx_flash_ramfunc.h
│   │   ├── stm32f4xx_fmc.h
│   │   ├── stm32f4xx_fsmc.h
│   │   ├── stm32f4xx_gpio.h
│   │   ├── stm32f4xx.h
│   │   ├── stm32f4xx_hash.h
│   │   ├── stm32f4xx_i2c.h
│   │   ├── stm32f4xx_iwdg.h
│   │   ├── stm32f4xx_ltdc.h
│   │   ├── stm32f4xx_pwr.h
│   │   ├── stm32f4xx_rcc.h
│   │   ├── stm32f4xx_rng.h
│   │   ├── stm32f4xx_rtc.h
│   │   ├── stm32f4xx_sai.h
│   │   ├── stm32f4xx_sdio.h
│   │   ├── stm32f4xx_spi.h
│   │   ├── stm32f4xx_syscfg.h
│   │   ├── stm32f4xx_tim.h
│   │   ├── stm32f4xx_usart.h
│   │   └── stm32f4xx_wwdg.h
│   └── Utilities
│       ├── fonts.h
│       ├── lcd_log.h
│       ├── stm324x7i_eval_audio_codec.h
│       ├── stm324x7i_eval_fmc_sram.h
│       ├── stm324x7i_eval.h
│       ├── stm324x7i_eval_i2c_ee.h
│       ├── stm324x7i_eval_ioe.h
│       ├── stm324x7i_eval_lcd.h
│       ├── stm324x7i_eval_sdio_sd.h
│       └── stm32_eval_legacy.h
├── office_src
│   ├── CMSIS
│   ├── MDK-ARM
│   │   ├── startup_stm32f401xx.s
│   │   ├── startup_stm32f40_41xxx.s
│   │   ├── startup_stm32f40xx.s
│   │   ├── startup_stm32f411xe.s
│   │   ├── startup_stm32f427_437xx.s
│   │   ├── startup_stm32f427x.s
│   │   └── startup_stm32f429_439xx.s
│   ├── STM32_StdPeriph_Driver
│   │   ├── misc.c
│   │   ├── stm32f4xx_adc.c
│   │   ├── stm32f4xx_can.c
│   │   ├── stm32f4xx_crc.c
│   │   ├── stm32f4xx_cryp_aes.c
│   │   ├── stm32f4xx_cryp.c
│   │   ├── stm32f4xx_cryp_des.c
│   │   ├── stm32f4xx_cryp_tdes.c
│   │   ├── stm32f4xx_dac.c
│   │   ├── stm32f4xx_dbgmcu.c
│   │   ├── stm32f4xx_dcmi.c
│   │   ├── stm32f4xx_dma2d.c
│   │   ├── stm32f4xx_dma.c
│   │   ├── stm32f4xx_exti.c
│   │   ├── stm32f4xx_flash.c
│   │   ├── stm32f4xx_flash_ramfunc.c
│   │   ├── stm32f4xx_fmc.c
│   │   ├── stm32f4xx_fsmc.c
│   │   ├── stm32f4xx_gpio.c
│   │   ├── stm32f4xx_hash.c
│   │   ├── stm32f4xx_hash_md5.c
│   │   ├── stm32f4xx_hash_sha1.c
│   │   ├── stm32f4xx_i2c.c
│   │   ├── stm32f4xx_iwdg.c
│   │   ├── stm32f4xx_ltdc.c
│   │   ├── stm32f4xx_pwr.c
│   │   ├── stm32f4xx_rcc.c
│   │   ├── stm32f4xx_rng.c
│   │   ├── stm32f4xx_rtc.c
│   │   ├── stm32f4xx_sai.c
│   │   ├── stm32f4xx_sdio.c
│   │   ├── stm32f4xx_spi.c
│   │   ├── stm32f4xx_syscfg.c
│   │   ├── stm32f4xx_tim.c
│   │   ├── stm32f4xx_usart.c
│   │   └── stm32f4xx_wwdg.c
│   └── Utilities
│       ├── fonts.c
│       ├── lcd_log.c
│       ├── stm324x7i_eval_audio_codec.c
│       ├── stm324x7i_eval.c
│       ├── stm324x7i_eval_fmc_sram.c
│       ├── stm324x7i_eval_i2c_ee.c
│       ├── stm324x7i_eval_ioe.c
│       ├── stm324x7i_eval_lcd.c
│       └── stm324x7i_eval_sdio_sd.c
├── README.md
├── user_inc
│   ├── bitband.h
│   ├── driver_init.h
│   ├── main.h
│   ├── os_kernel.h
│   ├── stm32f4xx_conf.h
│   ├── stm32f4xx_it.h
│   └── system_stm32f4xx.h
├── user_src
│   ├── driver_init.c
│   ├── main.c
│   ├── os_kernel.asm
│   ├── RemyOS.c
│   ├── RemyOS.h
│   ├── stm32f4xx_it.c
│   └── system_stm32f4xx.c
└── vspro.code-workspace

17 directories, 135 files
