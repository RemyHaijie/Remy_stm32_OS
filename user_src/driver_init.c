#include "driver_init.h"
const uint32_t COM_Pin_TX[COMn] = {GPIO_Pin_10,GPIO_Pin_12,GPIO_Pin_6};
const uint32_t COM_Pin_RX[COMn] = {GPIO_Pin_11,GPIO_Pin_2,GPIO_Pin_7};
const uint32_t COM_PinSource_TX[COMn] = {GPIO_PinSource10,GPIO_PinSource12,GPIO_PinSource6};
const uint32_t COM_PinSource_RX[COMn] = {GPIO_PinSource11,GPIO_PinSource2,GPIO_PinSource7};
GPIO_TypeDef* COM_IOPeriph_TX[COMn] = {GPIOB,GPIOC,GPIOC};
GPIO_TypeDef* COM_IOPeriph_RX[COMn] = {GPIOB,GPIOD,GPIOC};
uint32_t COM_IOCLOCK_TX[COMn] = {RCC_AHB1ENR_GPIOBEN,RCC_AHB1ENR_GPIOCEN,RCC_AHB1ENR_GPIOCEN};
uint32_t COM_IOCLOCK_RX[COMn] = {RCC_AHB1ENR_GPIOBEN,RCC_AHB1ENR_GPIODEN,RCC_AHB1ENR_GPIOCEN};
const uint32_t COM_CLOCK[COMn] = {RCC_APB1ENR_USART3EN,RCC_APB1ENR_UART5EN,RCC_APB2ENR_USART6EN};
const uint8_t COM_AF[COMn] = {GPIO_AF_USART3,GPIO_AF_UART5,GPIO_AF_USART6};
USART_TypeDef* COM_USARTx[COMn] = {USART3,UART5,USART6};
static __IO uint32_t __TimingDelay;


void sysclock_init(void)
{
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.SYSCLK_Frequency/100000 - 12);
  __TimingDelay = 0;
}
void delay_10_us(uint32_t delaynum)
{
  __TimingDelay = delaynum;
  while(__TimingDelay);
}
void delay_ms(uint32_t delaynum)
{
  __TimingDelay = delaynum*100;
  while(__TimingDelay);
}
//void SysTick_Handler(void)
//{
//  __TimingDelay--;
//  /*time slip*/
//  ostime_counter++;
//  if(ostime_counter >= OSTIME){
//    ostime_counter = 0;
//    cur_task++;
//    if(cur_task >= OSTASKNUM){
//      cur_task = 0;
//    }
//  }
//	/*goto pendsv*/
//  if(next_task == cur_task){
//    //SCB->ICSR |= SCB_ICSR_PENDSTSET_Msk;
//		NVIC_SetPendingIRQ(PendSV_IRQn);
//  }
//}
void sleep_deep_on(void)
{
	SCB->SCR |= 1<<1;
	__wfi();
}
void sleep_deep_off(void)
{
	SCB->SCR &= 1<<0;
}
uint16_t get_adc_val(void)
{
  ADC_RegularChannelConfig(ADC3,ADC_Channel_9,1,ADC_SampleTime_480Cycles);
	ADC_SoftwareStartConv(ADC3);
	while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC));
	return ADC_GetConversionValue(ADC3);
}
void init_adc(void)
{
  /*init PF3 ADC3_IN9*/
	/*init PA5 ADC1_IN5*/
  ADC_InitTypeDef ADC_InitStruct;
  GPIO_InitTypeDef GPIO_InitStruct;
  ADC_CommonInitTypeDef ADC_Common_Struct;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOFEN,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2ENR_ADC3EN,ENABLE);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOF,&GPIO_InitStruct);
  ADC_DeInit();
  ADC_Common_Struct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_Common_Struct.ADC_Mode = ADC_Mode_Independent;
  ADC_Common_Struct.ADC_Prescaler = ADC_Prescaler_Div8;
  ADC_Common_Struct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_Common_Struct);
  
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStruct.ADC_NbrOfConversion =  1;
  ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;
  ADC_Init(ADC3,&ADC_InitStruct);
  ADC_Cmd(ADC3,ENABLE);
}
void init_timer(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;
	
  RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN,ENABLE); 
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_Period = 1000-1;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 8400-1;
  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
  TIM_Cmd(TIM2,ENABLE);
	
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
  NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
  NVIC_Init(&NVIC_InitStruct);
}
uint8_t ledtemp = 0;
uint8_t adc_run = 0;
void TIM2_IRQHandler(void)
{
  if(TIM_GetFlagStatus(TIM2,TIM_IT_Update) == SET)
	{
		adc_run = 1;
		led1_out = !led1_out;
		SCB->SCR &= 1<<0;
	}
  TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}
void init_dac(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  DAC_InitTypeDef DAC_InitStruct;
  RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA,&GPIO_InitStruct);

  DAC_InitStruct.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
  DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
  DAC_InitStruct.DAC_Trigger = DAC_Trigger_None;
  DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_Init(DAC_Channel_1,&DAC_InitStruct);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  DAC_Cmd(DAC_Channel_1,ENABLE);
}
void COMPUT_DATA(COMType COM ,uint8_t ch)
{ 	
	while((COM_USARTx[COM]->SR&0X40)==0);
	COM_USARTx[COM]->DR = (uint8_t) ch;
}
void COM_Print(COMType COM,uint8_t * fpoint)
{
  while(*fpoint != '\0')
  {
    COMPUT_DATA(COM,*fpoint);
    fpoint += 1;
  }
	COMPUT_DATA(COM,'\r');
	COMPUT_DATA(COM,'\n');
}
void init_LED_GPIO(void)
{
	/*led in PE3 PE4 PG9*/
	GPIO_InitTypeDef ledstruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	ledstruct.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
	ledstruct.GPIO_Mode = GPIO_Mode_OUT;
	ledstruct.GPIO_Speed = GPIO_Speed_100MHz;
	ledstruct.GPIO_OType = GPIO_OType_OD;
	ledstruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE,&ledstruct);
	
	ledstruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOG,&ledstruct);
}


void init_uart(COMType COM,int32_t baudrate)
{
  USART_InitTypeDef USART_InitStruct;
  GPIO_InitTypeDef GPIO_InitStruct;
  RCC_AHB1PeriphClockCmd(COM_IOCLOCK_RX[COM],ENABLE);
  RCC_AHB1PeriphClockCmd(COM_IOCLOCK_TX[COM],ENABLE);
  /*USART1 and USART6 be connect with APB2*/
  /*APB2 is twice fast as APB1*/
  if(COM == COM6){
      RCC_APB2PeriphClockCmd(COM_CLOCK[COM],ENABLE);
  }
  else{
  RCC_APB1PeriphClockCmd(COM_CLOCK[COM],ENABLE);
  }
  GPIO_PinAFConfig(COM_IOPeriph_RX[COM],COM_PinSource_RX[COM],COM_AF[COM]);
  GPIO_PinAFConfig(COM_IOPeriph_TX[COM],COM_PinSource_TX[COM],COM_AF[COM]);
	
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Pin = COM_Pin_TX[COM];
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(COM_IOPeriph_TX[COM],&GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = COM_Pin_RX[COM];
  GPIO_Init(COM_IOPeriph_RX[COM],&GPIO_InitStruct);

  USART_InitStruct.USART_BaudRate = baudrate;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStruct.USART_Parity = USART_Parity_No;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_Init(COM_USARTx[COM],&USART_InitStruct);
  USART_Cmd(COM_USARTx[COM],ENABLE);
}
