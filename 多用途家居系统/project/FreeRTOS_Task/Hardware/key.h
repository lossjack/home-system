#ifndef __KEY_H
#define __KEY_H

#include "stm32f4xx.h" 

void 			KEY_Config(void);
unsigned char 	KEY_Get(void);	
void 			KEY_EXTIConfig(void);
#endif

