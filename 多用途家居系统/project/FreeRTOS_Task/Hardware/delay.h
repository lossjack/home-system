#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx.h" 
#include "FreeRTOS.h"
#include "task.h"
void delay(int tim);

void delay_us(int tus);
void delay_ms(int tms);
void delay_s(int ts);

#endif


