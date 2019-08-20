#ifndef __os_kernel_
#define __os_kernel_
	#include "stm32f4xx.h"
	typedef struct
	{
		uint32_t *PSP_pointer;
		void* task_pointer;
		uint16_t number;
		uint16_t priority;
		uint32_t *task_stack;
	}os_task_struct;
	void OS_Delay_Funcion(void);
#endif
