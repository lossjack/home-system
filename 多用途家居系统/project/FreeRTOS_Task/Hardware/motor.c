#include "motor.h"
#include "stm32f4xx.h"

void MOTOR_Config(){
	//配置引脚
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIOInitStruct={0};
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_2;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_AN;//浮空模拟输入模式
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//浮空--无上下拉
	GPIO_Init(GPIOC,&GPIOInitStruct);

}