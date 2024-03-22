#include "beep.h"



void BEEP_Config(void)
{
	//单独开启GPIOF组  GPIOE组的时钟--必须放最前面
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
	/////////////////////引脚配置////////////////////////
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_8;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_OUT;//输出模式
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;//推挽
	GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;//低速
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//浮空--无上下拉
	GPIO_Init(GPIOF,&GPIOInitStruct);
	
}
