
	EXPORT    PendSV_Handler
    EXPORT    SVC_Handler
;	EXPORT    OS_Delay_Funcion
	
	IMPORT    SVC_Handler_Cpp
	IMPORT    curr_task
	IMPORT    next_task
	IMPORT    PSP_array
	IMPORT    delay_falg
	IMPORT    os_null_flag
	IMPORT    PSP_OS
		
	PRESERVE8
	AREA |.text|, CODE, READONLY
	THUMB
PendSV_Handler
	
	MRS R0,PSP ;// Get current process stack pointer value
	LDR R1,=curr_task
	;LDR R2,[R2] ;// Get current task ID
	
	;test for delay flag
	LDR R2,=delay_falg
	LDR R3,[R2]
	TST R3,#1
	BNE Load
	
	;test for null task
	LDR R2,=os_null_flag
	LDR R3,[R2]
	TST R3,#1
	BNE Load_null_task
	
	;save now retur status
	LDR R3,=PSP_array
	STMDB R0!,{R4-R11} ;// Save R4 to R11 in task stack (8 regs)
	LDR R2,[R1]
	STR R0,[R3, R2, LSL #2]; // Save PSP value into PSP_array
Load
	;clear delay_flag
	MOV R4,#0
	STR R4,[R2]

	;Load next contexfdss
	LDR R4,=next_task
	LDR R4,[R4]
	STR R4,[R1]
	;TR R4,[R1] ;// Set curr_task = next_task
	LDR R3,=PSP_array
	LDR R0,[R3, R4, LSL #2] ;// Load PSP value from PSP_array
	LDMIA R0!,{R4-R11} ;// Load R4 to R11 from task stack (8 regs)
	MSR PSP, R0 ;// Set PSP to next task
	BX LR ;// Return
Load_null_task
	;clear os_null_falg
	MOV R4,#0
	STR R4,[R2]
	;go to null task
	LDR R0,=PSP_OS
	LDR R0,[R0]
	LDMIA R0!,{R4-R11}
	MSR PSP,R0
	BX LR
	ALIGN 4

SVC_Handler
	LDR R3,=delay_falg
	LDR R3,[R3]
	TST R3,#1
	BNE OS_Delay_L
	
	;read now sp poinnter what is use
	TST LR,#4
	ITE EQ
	MRSEQ R0,MSP
	MRSNE R0,PSP
	;read svc number
	;LDR   R2,[R2,#24]
	;LDRB  R0,[R0,#-2]
	;go to svc interrupt C language program part
	LDR R1,=SVC_Handler_Cpp
	BX R1
OS_Delay_L
	;SAVE R4-R11
	MRS R0,PSP
	STMDB R0!,{R4-R11}
	;SAVE PSP
	LDR R1,=curr_task
	LDR R1,[R1]
	LDR R2,=PSP_array
	STR R0,[R2,R1,LSL #2]
	;CLEAR DELAY_FLAG
	BX LR
	ALIGN 4
	END