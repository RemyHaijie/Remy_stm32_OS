/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.h 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "driver_init.h"
#include "stm32f4xx.h"
#include "stdio.h"
#include "driver_init.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define OSTIME 10
#define OSTASKNUM 2
#define MEN2DRIVE(addr) (*((volatile unsigned long  *)(addr)))
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void task0(void); // Toggle LED0
void task1(void); // Toggle LED1
void task2(void); // Toggle LED2
void task3(void); // Toggle LED3
void __svc(0x01) OS_Delay(void);

//extern uint32_t cur_task;
//extern uint32_t next_task;
extern uint32_t ostime_counter;
//extern uint32_t PSP_pointer[OSTASKNUM];
extern uint32_t curr_task; // Current task
extern uint32_t next_task; // Next task
extern uint32_t PSP_array[4]; // Process Stack Pointer for each task
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
