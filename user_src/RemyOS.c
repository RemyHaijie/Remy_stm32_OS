#include "RemyOS.h"
#include "main.h"
volatile uint32_t systick_count=0;
uint32_t task0_stack[1024], task1_stack[1024],task2_stack[1024], task3_stack[1024];
uint32_t task_null_stack[32];
uint32_t PSP_OS;
// Data use by OS
uint32_t curr_task=0; // Current task
uint32_t next_task=1; // Next task
uint32_t PSP_array[TRASKNUM]; // Process Stack Pointer for each task
uint16_t now_task_id = 0;
uint16_t _time_next_number = 0;
uint32_t delay_falg = 0;
uint32_t os_null_flag = 0;
struct __RemyOS_struct
{
    /*access task attribute*/
    uint16_t os_ready_num;
    uint16_t os_waiting;
    os_task_struct * task[TRASKNUM];
    /*anytask delay variable*/
    uint32_t os_task_delay[TRASKNUM];
    OS_STATUS os_status[TRASKNUM];
    uint16_t os_ready_list[TRASKNUM];
    
}_RemyOS;
/*
    \brief : svc interrupt Cpp part
                    SVC中断的C语言部分
    \param  : svc_array should be a uint32_t array but also can be allowed four directly parameter
                    SVC函数传递最好使用数组传递，不过也可以使用最大4个的直接参数传递
*/
void SVC_Handler_Cpp(uint32_t * svc_array)
{
	uint32_t svc_stack_r[4];
    uint16_t svc_id;
    /*
        cortex interrupt push stack structure
        cortex 中断压栈结构
                 asm          C program languge
        SP   ->  R0        ->    first  param
        SP+1 ->  R1        ->    second param
        SP+2 ->  R2        ->    third  param
        SP+3 ->  R3        ->    fourth param
        SP+4 ->  R12       ->      NULL
        SP+5 ->  LR        ->      NULL
        SP+6 ->  return_PC ->      NULL

        svc function structure
        SVC 标号函数在内存中的结构
        return_pc - 2    svc number
        return_pc + 0    svc function entry address
    
    */
	svc_stack_r[0] = svc_array[0];
    svc_stack_r[1] = svc_array[1];
    svc_stack_r[2] = svc_array[2];
    svc_stack_r[3] = svc_array[3];
	svc_id = ((char *)svc_array[6])[-2];
    switch (svc_id)
    {
        /*delay svc*/
    case 0x01:

        break;
    
    default:
        break;
    }

    return;
}
void os_null_task(void)
{
    while(1){

    }
}
/*
    \brief :  OS delay is a behavior that abandon now cpu control until delay done
                    系统延迟是一个直接在延迟完成前一直放弃CPU控制权的行为
    \param  : timedelay the number of time slices to delay
                    要延迟的时间片个数
*/
void RemyOS_delay(uint32_t timedelay)
{
    _RemyOS.os_status[curr_task] = OS_ENABLE;
    _RemyOS.os_task_delay[curr_task] = timedelay;
	delay_falg = 1;
    OS_Delay();
    while(delay_falg == 1);
    //task_despatch_time();
}
/*
    自由时间轮转
    \brief：each task has the same slice of time whether or not is has delay
                    每一个任务拥有相同的时间片，不管任务是否有延迟
*/
void task_despatch_raw(void)
{
    if(curr_task >= TRASKNUM){
        stop_cpu;
    }
    else if(curr_task == TRASKNUM - 1){
        next_task = 0;
    }
    else{
        next_task++;
    }
}
/*
    按延迟就绪表轮转
    \brief ： when a task is not in the delay ,it will be marked as being ready
                    each task marked ready is added to a ready list 
                    task time take turns OS according to the ready list
                    当一个任务没有掉入系统延迟，他就会被标记为就绪状态
                    每一个就绪状态的任务被加入到就绪表中
                    系统根据就绪表进行时间轮转
*/
void task_despatch_time(void)
{
    uint16_t i = 0; 
    uint16_t temp = 0;
    /*更新进程延迟时间*/
	for(;i<TRASKNUM;i++){
        if(_RemyOS.os_task_delay[i] != 0){
            _RemyOS.os_task_delay[i] --;
        }
        if(_RemyOS.os_task_delay[i] == 0 && _RemyOS.os_status[i] == OS_ENABLE){
            _RemyOS.os_status[i] = OS_READY;
        }
    }
    /*更新就绪表*/
    RemyOS_status_reflash();
    /*在就绪表中进行自由时间轮转*/
        /*就绪表空*/
    if(_RemyOS.os_ready_num == 0){
        os_null_flag = 1;
    }
        /*就绪表不空,观察下一状态合理性*/
    else{
        /*不空且扫描到尽头*/
        if(_time_next_number + 1 >= _RemyOS.os_ready_num){
            _time_next_number = 0;
            temp = _RemyOS.os_ready_list[0];
        }
        else{
            /*不空未扫描到尽头,陷入系统延迟,有任务被删除*/
            if(delay_falg == 1){
                temp = _RemyOS.os_ready_list[_time_next_number];
            }
            /*不空未扫描到尽头,未陷入系统延迟,没有任务被删除*/
            else{
                _time_next_number = _time_next_number + 1;
                temp = _RemyOS.os_ready_list[_time_next_number];
            }
        }
    }
        if(temp >= TRASKNUM){
            stop_cpu;
        }
        else{
            next_task = temp;
        }
}
/*
void SysTick_Handler(void) // 1KHz
{
	systick_count++;
    task_despatch_raw();
    if (curr_task!=next_task){
        SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
    }
	return;
}
*/
void SysTick_Handler(void) // 1KHz
{
    systick_count++;
    task_despatch_time();
    if (curr_task!=next_task){
    	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	}
	return;
}

void RemyOS_init(void)
{
	uint16_t i = 0;
	curr_task = 0;
    _RemyOS.os_waiting = 0;
    PSP_OS = (uint32_t)task_null_stack + (sizeof (task_null_stack)) - 16*4;
	HW32_REG((PSP_OS + (14<<2))) = (uint32_t)os_null_task;
	// initial Program Counter
	HW32_REG((PSP_OS + (15<<2))) = 0x01000000;
	for(;i<TRASKNUM;i++){
			_RemyOS.os_task_delay[i] = 0;
            _RemyOS.os_status[i] = OS_ENABLE;
	}
}
void creat_task(os_task_struct * os_task)
{
    _RemyOS.task[now_task_id] = os_task;
    now_task_id++;
    init_stack(os_task);
}
void RemyOS_task_ENABLE(void)
{
    uint16_t i = 0;
    for(;i<TRASKNUM;i++){
			if(_RemyOS.os_status[i] == OS_ENABLE)
			    _RemyOS.os_status[i] = OS_READY;
	}
		RemyOS_status_reflash();
}
void RemyOS_start(void)
{
	
	__set_PSP((PSP_array[curr_task] + 16*4));
	NVIC_SetPriority(PendSV_IRQn, 0xFF);
	SysTick_Config(168000);
	__set_CONTROL(0x3);
	__ISB();

    ((void(*)(void))(_RemyOS.task[0])->task_pointer)();
}
void init_stack(os_task_struct * os_task)
{
	PSP_array[os_task->number] = (uint32_t)os_task->task_stack + (sizeof (task2_stack)) - 16*4;
	HW32_REG((PSP_array[os_task->number] + (14<<2))) = (uint32_t)os_task->task_pointer;
	// initial Program Counter
	HW32_REG((PSP_array[os_task->number] + (15<<2))) = 0x01000000;
}
void RemyOS_status_reflash(void)
{
    uint16_t i = 0;
		uint16_t j = 0;
    for(;i<TRASKNUM;i++){
        if(_RemyOS.os_status[i] == OS_READY){
            _RemyOS.os_ready_list[j] = i;
						j++;
        }
    }
		if(j == 0){
			_RemyOS.os_ready_num = 0;
		}
		else{
			_RemyOS.os_ready_num = j;
		}
    
    do{
        _RemyOS.os_ready_list[j] = TRASKNUM;
				j++;
    }while(j<=TRASKNUM);
}
