#include "beep.h"



void BEEP_Config(void)
{
	//��������GPIOF��  GPIOE���ʱ��--�������ǰ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
	/////////////////////��������////////////////////////
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_8;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_OUT;//���ģʽ
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;//����
	GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;//����
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//����--��������
	GPIO_Init(GPIOF,&GPIOInitStruct);
	
}
