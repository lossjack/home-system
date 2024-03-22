#include "motor.h"
#include "stm32f4xx.h"

void MOTOR_Config(){
	//��������
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIOInitStruct={0};
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_2;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_AN;//����ģ������ģʽ
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//����--��������
	GPIO_Init(GPIOC,&GPIOInitStruct);

}