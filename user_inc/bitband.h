#ifndef __bitband_h
#define __bitband_h
#include "stm32f4xx.h"
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define ODR_DEV 20
#define IDR_DEV 16

#define GPIOx_ODR(GPIOx_BASE) GPIOx_BASE+ODR_DEV
#define GPIOx_IDR(GPIOx_BASE) GPIOx_BASE+IDR_DEV

#define Pxwrite(GPIOx_BASE,n) BIT_ADDR(GPIOx_ODR(GPIOx_BASE),n)
#define Pxread(GPIOx_BASE,n) BIT_ADDR(GPIOx_IDR(GPIOx_BASE),n)

#endif
