#ifndef __driver_h
 #define __driver_h
 #include "bitband.h"
 #include "stdio.h"
 #include "stm32f4xx_gpio.h"
 #include "stm32f4xx_usart.h"
 #include "stm32f4xx_dac.h"
 #include "stm32f4xx_adc.h"
 #include "stm32f4xx_tim.h"
 #include "main.h"
 #define led1_out Pxwrite(GPIOE_BASE,3)
 #define led2_out Pxwrite(GPIOE_BASE,4)
 #define led3_out Pxwrite(GPIOG_BASE,9)
 
 #define led1_read Pxread(GPIOE_BASE,3)
 #define led2_read Pxread(GPIOE_BASE,4)
 #define led3_read Pxread(GPIOG_BASE,9)
 
 extern uint8_t adc_run;
 /*led in PE3 PE4 PG9*/
 void init_LED_GPIO(void);
 /*init uart as 
    stop bit : one stop bits
    data bit :8 data bit
    partity bit : no parity
    hardware flow contrl : none

    the USART_TypeDef param can be:
        USART3
        
        USART5
        USART6
 */
typedef enum
{
    COM3 = 0,
    COM5 = 1,
    COM6 = 2
}COMType;
#define COMn 3
void COM_Print(COMType COM,uint8_t * fpoint);
void COMPUT_DATA(COMType COM ,uint8_t ch);
void init_LED_GPIO(void);
void init_uart(COMType COM,int32_t baudrate);
void init_timer(void);
void init_dac(void);
void init_adc(void);
uint16_t get_adc_val(void);
uint16_t Get_Adc(uint8_t ch);
void sleep_deep_on(void);
void sleep_deep_off(void);
void sysclock_init(void);
void delay_10_us(uint32_t delaynum);
void delay_ms(uint32_t delaynum);
#endif 

