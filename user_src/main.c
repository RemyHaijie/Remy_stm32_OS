
#include "stm32f4xx.h"
#include "driver_init.h"
#include "main.h"
#include "RemyOS.h"


// Event to tasks

// Stack for each task (8Kbytes each - 1024 x 8 bytes)

// -------------------------------------------------------------
// Start of main program
int main(void)
{
	os_task_struct task1_struct={PSP_array,task0,0,0,task0_stack};
	os_task_struct task2_struct={PSP_array,task1,1,1,task1_stack};
	os_task_struct task3_struct={PSP_array,task2,2,2,task2_stack};
	os_task_struct task4_struct={PSP_array,task3,3,3,task3_stack};
	//SCB->CCR |= SCB_CCR_STKALIGN_Msk; // Enable double word stackalignment
	init_LED_GPIO();
	
	led1_out = 1;led2_out = 1; led3_out = 1;
	creat_task(&task1_struct);
	creat_task(&task2_struct);
	creat_task(&task3_struct);
	creat_task(&task4_struct);
	

	RemyOS_init();
	RemyOS_task_ENABLE();
	RemyOS_start();
	while(1){
	stop_cpu;// Should not be here
	};
}

// ------------------------------------------------------------
void task0(void) // Toggle LED #0
{ 
	while (1) {
	}
}
// ------------------------------------------------------------
void task1(void) // Toggle LED #1
{
	while (1) 
	{
//		if(systick_count & 0x400){
//		led3_out = 0;
//		}
//		else{
//			led3_out = 1;
//		}
		led3_out = !led3_out;
		RemyOS_delay(0x100);
	}
	
}
// ------------------------------------------------------------
void task2(void)
{
	while (1)
	{
//		if(systick_count & 0x200){
//		led2_out = 0;
//		}
//		else{
//			led2_out = 1;
//		}
		led2_out = !led2_out;
		RemyOS_delay(0x200);
	}
	
}
// ------------------------------------------------------------
void task3(void)
{ 
//	led1_out = !led1_out;
//	RemyOS_delay(0x400);
	
	while (1) 
	{
//		if(systick_count & 0x100){
//		led1_out = 0;
//		}
//		else{
//			led1_out = 1;
//		}
		led1_out = !led1_out;
		RemyOS_delay(0x400);
	}
	
}


