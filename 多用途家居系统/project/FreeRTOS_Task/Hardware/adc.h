#ifndef __ADC_H
#define __ADC_H

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

extern unsigned short ADC1_Val[5];

void ADC1_Config(void);
void ADC3_Config(void);


#endif

