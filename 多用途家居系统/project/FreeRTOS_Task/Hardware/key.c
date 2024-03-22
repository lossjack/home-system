#include "key.h"


void KEY_Config(void)
{
	//单独开启GPIOF组  GPIOE组的时钟--必须放最前面
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	/////////////////////引脚配置////////////////////////
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_0;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_IN;//输入模式
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//浮空--无上下拉
	GPIO_Init(GPIOA,&GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init(GPIOE,&GPIOInitStruct);
}


//返回1--第一个按键按下 2---第二个按键按下  3  4    0--没有任何按键按下
unsigned char KEY_Get(void)
{
	unsigned char KeyFlag=0;
	
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 0)
		KeyFlag |= 0x01;//记录下是第一个按键按下
	
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) == 0)
		KeyFlag |= 0x01<<1;//记录下是第二个按键按下
		
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)
		KeyFlag |= 0x01<<2;//记录下是第二个按键按下
	
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)
		KeyFlag |= 0x01<<3;//记录下是第二个按键按下

	return KeyFlag;
}




void KEY_EXTIConfig(void)
{
	//0、使能EXTI(不需要使能 默认开启)和SYSCFG模块的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	//1、选择对应的EXTIx----指定引脚
	//PA0----EXTI0
	SYSCFG->EXTICR[0] &= ~0x0F;
	
	//2、EXTI配置---只检测下降沿+
	EXTI->FTSR |= 0x01;//选择下降沿触发---检测EXTI0下降沿是否有触发
	EXTI->RTSR &= ~0x01;//不检测上升沿
	EXTI->IMR  |= 0x01;//开启EXTI0的中断---去掉屏蔽
	
	
	
	//3、配置NVIC---配置EXTI0的中断优先级
	//NVIC_Init()
	NVIC_InitTypeDef NVICInitStruct;
	NVICInitStruct.NVIC_IRQChannel = EXTI0_IRQn;//全局中断号
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 10;//0~15
	NVICInitStruct.NVIC_IRQChannelSubPriority        = 0;
	NVICInitStruct.NVIC_IRQChannelCmd                = ENABLE;//使能
	NVIC_Init(&NVICInitStruct);//配置NVIC
}

//中断服务函数----中断触发后，CPU跳转过来的函数
void EXTI0_IRQHandler(void)
{
	if(EXTI->PR & 0x01)  //0000 0000 0000 0000 0000 0000 ... 0001
	{
		//标志位置位---代表下降沿触发
		//delay(2);
		//GPIO_ToggleBits(GPIOF,GPIO_Pin_9);
		
		//清零标志位 rc_w1  写1清零 写0无影响
		EXTI->PR = 0x01;//0000 0000 0000 0001
	}
}


