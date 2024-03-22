#include "led.h"



void LED_Config(void)
{
	//单独开启GPIOF组  GPIOE组的时钟--必须放最前面
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	/////////////////////引脚配置////////////////////////
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_OUT;//输出模式
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;//推挽
	GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;//低速
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//浮空--无上下拉
	GPIO_Init(GPIOF,&GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOE,&GPIOInitStruct);
	
	//灯全灭
	GPIO_WriteBit(GPIOF,GPIO_Pin_9 | GPIO_Pin_10,(BitAction)1);
	GPIO_WriteBit(GPIOE,GPIO_Pin_13 | GPIO_Pin_14,(BitAction)1);
}

//no 第几个LED  1~4 
//sw 开关状态 1-开  0-关
void LED_Ctr(char no,char sw)
{
	switch(no)
	{
		case 1:
			if(sw)
				GPIO_WriteBit(GPIOF,GPIO_Pin_9,(BitAction)0);//亮灯
			else
				GPIO_WriteBit(GPIOF,GPIO_Pin_9,(BitAction)1);//灭灯
			break;
		case 2:
			if(sw)
				GPIO_WriteBit(GPIOF,GPIO_Pin_10,(BitAction)0);//亮灯
			else
				GPIO_WriteBit(GPIOF,GPIO_Pin_10,(BitAction)1);//灭灯
			break;
		case 3:
			if(sw)
				GPIO_WriteBit(GPIOE,GPIO_Pin_13,(BitAction)0);//亮灯
			else
				GPIO_WriteBit(GPIOE,GPIO_Pin_13,(BitAction)1);//灭灯
			break;
		case 4:
			if(sw)
				GPIO_WriteBit(GPIOE,GPIO_Pin_14,(BitAction)0);//亮灯
			else
				GPIO_WriteBit(GPIOE,GPIO_Pin_14,(BitAction)1);//灭灯
			break;
	}
}


