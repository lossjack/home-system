/*
 * FreeRTOS V202212.01
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* Ensure stdint is only used by the compiler, and not the assembler. */
#include <stdio.h>
#include <stdint.h>
extern uint32_t SystemCoreClock;


#define configUSE_PREEMPTION			1 //是否使用抢占式任务调用
#define configUSE_IDLE_HOOK				0 //空闲任务回调函数是否开启
#define configUSE_TICK_HOOK				0 //节拍任务的回调函数是否开启
#define configCPU_CLOCK_HZ				( SystemCoreClock ) //CPU主频参数 168Mhz
#define configTICK_RATE_HZ				( ( TickType_t ) 1000 ) //OS 节拍频率  1ms/节拍 200~1000
#define configMAX_PRIORITIES			( 32 ) //最大的任务优先级 最大：32   0~31
#define configMINIMAL_STACK_SIZE		( ( unsigned short ) 130 ) //空闲任务的栈大小 单位：字 word
#define configTOTAL_HEAP_SIZE			( ( size_t ) ( 75 * 1024 ) ) //给FreeRTOS使用的堆大小 75KB
#define configMAX_TASK_NAME_LEN			( 32 ) //
#define configUSE_TRACE_FACILITY		1  //可视化调试追踪
#define configUSE_16_BIT_TICKS			0  //选32bit变量记录系统节拍数
#define configIDLE_SHOULD_YIELD			1  //空闲任务的行为设置  1  0
#define configUSE_MUTEXES				1  //使用了互斥锁
#define configQUEUE_REGISTRY_SIZE		8  //可以注册和定义的队列及信号量的最大数量
#define configCHECK_FOR_STACK_OVERFLOW	0  //堆栈溢出检查 0:不做溢出检查 方法1  方法2√
#define configUSE_RECURSIVE_MUTEXES		1  //支持互斥锁嵌套  0:不能嵌套互斥锁
#define configUSE_MALLOC_FAILED_HOOK	0  //开启malloc失败的回调
#define configUSE_APPLICATION_TASK_TAG	0
#define configUSE_COUNTING_SEMAPHORES	1  //开启使用信号量  0：关闭信号量使用
#define configGENERATE_RUN_TIME_STATS	0  //不开启运行时间统计  1：还要开启另外2个宏





/* 软件定时器功能的定义 TIM1~TIM14--硬件定时器*/
#define configUSE_TIMERS				0  //开启了   
#define configTIMER_TASK_PRIORITY		( 2 )
#define configTIMER_QUEUE_LENGTH		10
#define configTIMER_TASK_STACK_DEPTH	( configMINIMAL_STACK_SIZE * 2 )

/* 通过给下面的宏置1可开启对应的函数  否则该函数不能用 */
#define INCLUDE_vTaskPrioritySet		1   //可以使用任务优先级修改函数
#define INCLUDE_uxTaskPriorityGet		1   //任务的优先级获取
#define INCLUDE_vTaskDelete				1	//任务删除函数
#define INCLUDE_vTaskCleanUpResources	1
#define INCLUDE_vTaskSuspend			1   //任务挂起函数INCLUDE_vTaskSuspend 
#define INCLUDE_vTaskDelayUntil			1	//任务的延时函数
#define INCLUDE_vTaskDelay				1	//精准延时函数



/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
	/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4        /* 15 priority levels */
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			0xf

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5  //做了一个优先级管理阈值 0~4的优先级RTOS不能管  5~15RTOS能管理

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/*断言函数，用于做检查  出现问题能够打印对应的文件名  函数名和行号*/
#define configASSERT( x ) if( ( x ) == 0 ) {printf("Err:%s %s %d\r\n",__FILE__,__FUNCTION__,__LINE__); taskDISABLE_INTERRUPTS(); for( ;; ); }

/* 替换 三个系统异常中断的中断函数名   这几个中断非常重要  和RTOS任务切换有关*/
#define vPortSVCHandler     SVC_Handler
#define xPortPendSVHandler  PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */

