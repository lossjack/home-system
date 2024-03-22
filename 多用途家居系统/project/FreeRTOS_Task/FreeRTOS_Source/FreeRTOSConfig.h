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


#define configUSE_PREEMPTION			1 //�Ƿ�ʹ����ռʽ�������
#define configUSE_IDLE_HOOK				0 //��������ص������Ƿ���
#define configUSE_TICK_HOOK				0 //��������Ļص������Ƿ���
#define configCPU_CLOCK_HZ				( SystemCoreClock ) //CPU��Ƶ���� 168Mhz
#define configTICK_RATE_HZ				( ( TickType_t ) 1000 ) //OS ����Ƶ��  1ms/���� 200~1000
#define configMAX_PRIORITIES			( 32 ) //�����������ȼ� ���32   0~31
#define configMINIMAL_STACK_SIZE		( ( unsigned short ) 130 ) //���������ջ��С ��λ���� word
#define configTOTAL_HEAP_SIZE			( ( size_t ) ( 75 * 1024 ) ) //��FreeRTOSʹ�õĶѴ�С 75KB
#define configMAX_TASK_NAME_LEN			( 32 ) //
#define configUSE_TRACE_FACILITY		1  //���ӻ�����׷��
#define configUSE_16_BIT_TICKS			0  //ѡ32bit������¼ϵͳ������
#define configIDLE_SHOULD_YIELD			1  //�����������Ϊ����  1  0
#define configUSE_MUTEXES				1  //ʹ���˻�����
#define configQUEUE_REGISTRY_SIZE		8  //����ע��Ͷ���Ķ��м��ź������������
#define configCHECK_FOR_STACK_OVERFLOW	0  //��ջ������ 0:���������� ����1  ����2��
#define configUSE_RECURSIVE_MUTEXES		1  //֧�ֻ�����Ƕ��  0:����Ƕ�׻�����
#define configUSE_MALLOC_FAILED_HOOK	0  //����mallocʧ�ܵĻص�
#define configUSE_APPLICATION_TASK_TAG	0
#define configUSE_COUNTING_SEMAPHORES	1  //����ʹ���ź���  0���ر��ź���ʹ��
#define configGENERATE_RUN_TIME_STATS	0  //����������ʱ��ͳ��  1����Ҫ��������2����





/* �����ʱ�����ܵĶ��� TIM1~TIM14--Ӳ����ʱ��*/
#define configUSE_TIMERS				0  //������   
#define configTIMER_TASK_PRIORITY		( 2 )
#define configTIMER_QUEUE_LENGTH		10
#define configTIMER_TASK_STACK_DEPTH	( configMINIMAL_STACK_SIZE * 2 )

/* ͨ��������ĺ���1�ɿ�����Ӧ�ĺ���  ����ú��������� */
#define INCLUDE_vTaskPrioritySet		1   //����ʹ���������ȼ��޸ĺ���
#define INCLUDE_uxTaskPriorityGet		1   //��������ȼ���ȡ
#define INCLUDE_vTaskDelete				1	//����ɾ������
#define INCLUDE_vTaskCleanUpResources	1
#define INCLUDE_vTaskSuspend			1   //���������INCLUDE_vTaskSuspend 
#define INCLUDE_vTaskDelayUntil			1	//�������ʱ����
#define INCLUDE_vTaskDelay				1	//��׼��ʱ����



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
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5  //����һ�����ȼ�������ֵ 0~4�����ȼ�RTOS���ܹ�  5~15RTOS�ܹ���

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/*���Ժ��������������  ���������ܹ���ӡ��Ӧ���ļ���  ���������к�*/
#define configASSERT( x ) if( ( x ) == 0 ) {printf("Err:%s %s %d\r\n",__FILE__,__FUNCTION__,__LINE__); taskDISABLE_INTERRUPTS(); for( ;; ); }

/* �滻 ����ϵͳ�쳣�жϵ��жϺ�����   �⼸���жϷǳ���Ҫ  ��RTOS�����л��й�*/
#define vPortSVCHandler     SVC_Handler
#define xPortPendSVHandler  PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */

