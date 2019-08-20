#ifndef __remyos_
    #define __remyos_
    /*include file block*/
    #include "os_kernel.h"
    /*USER EDIT DEFINE*/
    #define TRASKNUM 4

    #define HW32_REG(ADDRESS) (*((volatile unsigned long *)(ADDRESS)))
    /* Use Breakpoint to stop when error is detected
    (KEIL MDK specific intrinsic) */
    /* it can be changed to while(1) XXif needed */
    #define stop_cpu __breakpoint(0)
    typedef enum
    {
        OS_ENABLE    = 1,
        OS_ACTIVE    = 2,
        OS_READY     = 3,
        OS_DISENABLE = 4

    }OS_STATUS;
    
    
    extern volatile uint32_t systick_count;
    extern uint32_t task0_stack[1024], task1_stack[1024],task2_stack[1024], task3_stack[1024];
    /*OS data*/
    extern uint32_t curr_task; // Current task
    extern uint32_t next_task; // Next task
    extern uint32_t PSP_array[4]; // Process Stack Pointer for each task
		extern uint32_t delay_falg;
		extern uint32_t os_null_flag;
		extern uint32_t PSP_OS;
		/*svc funtion*/
    
		void SVC_Handler_Cpp(uint32_t * svc_array);
    void init_stack(os_task_struct * os_task);
    void RemyOS_start(void);
    void RemyOS_init(void);
    void RemyOS_status_reflash(void);
    void creat_task(os_task_struct * os_task);
    void RemyOS_delay(uint32_t timedelay);
    void task_despatch_raw(void);
    void RemyOS_task_ENABLE(void);
    void task_despatch_time(void);
    void os_null_task(void);
#endif // !__remyos_
