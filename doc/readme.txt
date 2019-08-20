in this program is text for adc dac usart timer wfi(low power function) multibly application in stm32.\

where is the hardware connect:
DAC used by PA5
ADC used by 
timer used by timer1
usart used by USART3 maindly. but also init USART6 and UART5 to text then funtional.

how to the data creat and translant:
 went every timer count out with this interrupt can wake up system to deal with:
    DAC -> ADC -> usart