#include "tim.h"


void TIM6_Config(void)
{
	TIM_TimeBaseInitTypeDef TimInitStruct;
	
	//ʹ��ʱ�� APB1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	//TIM6 ʱ����Ԫ
	TimInitStruct.TIM_Prescaler = 8400-1;
	TimInitStruct.TIM_Period    = 10000-1;
	TIM_TimeBaseInit(TIM6,&TimInitStruct);
	TIM_SetCounter(TIM6,0);
	
	//ʹ��TIM6�ĸ����ж�
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	
	//�������Ķ�ʱ���ж�����NVIC���ȼ�����NVICʹ��
	NVIC_InitTypeDef NVICInitStruct;
	NVICInitStruct.NVIC_IRQChannel = TIM6_DAC_IRQn;//ȫ���жϺ�
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVICInitStruct.NVIC_IRQChannelSubPriority        = 0;
	NVICInitStruct.NVIC_IRQChannelCmd                = ENABLE;//ʹ��
	NVIC_Init(&NVICInitStruct);//����NVIC
	
	
	//����TIM6������
	TIM_Cmd(TIM6,ENABLE);
}


void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) == SET)
	{
		GPIO_ToggleBits(GPIOE,GPIO_Pin_14);
		
		//TIM6->SR = ~0x01;//1111 1111 1111 1110
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	}
}


void TIM2345_Config(void)
{
	TIM_TimeBaseInitTypeDef TimInitStruct={0};
	
	//ʹ��ʱ�� APB1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	
	//TIM2 3 4 5 ʱ����Ԫ
	TimInitStruct.TIM_Prescaler = 8400-1; // T = 0.1ms
	TimInitStruct.TIM_Period    = 3000-1;// 0.3s
	TimInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TimInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2,&TimInitStruct);
	TIM_SetCounter(TIM2,0);
	
	TimInitStruct.TIM_Prescaler = 8400-1; // T = 0.1ms
	TimInitStruct.TIM_Period    = 6000-1;// 0.6s
	TIM_TimeBaseInit(TIM3,&TimInitStruct);
	TIM_SetCounter(TIM3,0);
	
	TimInitStruct.TIM_Prescaler = 8400-1; // T = 0.1ms
	TimInitStruct.TIM_Period    = 8000-1;// 0.8s
	TIM_TimeBaseInit(TIM4,&TimInitStruct);
	TIM_SetCounter(TIM4,0);
	
	TimInitStruct.TIM_Prescaler = 8400-1; // T = 0.1ms
	TimInitStruct.TIM_Period    = 12000-1;// 1.2s
	TIM_TimeBaseInit(TIM5,&TimInitStruct);
	TIM_SetCounter(TIM5,0);
	
	//ʹ��TIM2 3 4 5�ĸ����ж�
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	
	//�������Ķ�ʱ���ж�����NVIC���ȼ�����NVICʹ��
	NVIC_InitTypeDef NVICInitStruct;
	NVICInitStruct.NVIC_IRQChannel = TIM2_IRQn;//ȫ���жϺ�
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVICInitStruct.NVIC_IRQChannelSubPriority        = 0;
	NVICInitStruct.NVIC_IRQChannelCmd                = ENABLE;//ʹ��
	NVIC_Init(&NVICInitStruct);//����NVIC
	
	NVICInitStruct.NVIC_IRQChannel = TIM3_IRQn;//ȫ���жϺ�
	NVIC_Init(&NVICInitStruct);//����NVIC
	
	NVICInitStruct.NVIC_IRQChannel = TIM4_IRQn;//ȫ���жϺ�
	NVIC_Init(&NVICInitStruct);//����NVIC
	
	NVICInitStruct.NVIC_IRQChannel = TIM5_IRQn;//ȫ���жϺ�
	NVIC_Init(&NVICInitStruct);//����NVIC
	
	//����TIM2 3 4 5������
	TIM_Cmd(TIM2,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
	TIM_Cmd(TIM5,ENABLE);
}


void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		GPIO_ToggleBits(GPIOF,GPIO_Pin_9);
		
		//TIM6->SR = ~0x01;//1111 1111 1111 1110
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}


void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET)
	{
		GPIO_ToggleBits(GPIOF,GPIO_Pin_10);
		
		//TIM6->SR = ~0x01;//1111 1111 1111 1110
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}

void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == SET)
	{
		GPIO_ToggleBits(GPIOE,GPIO_Pin_13);
		
		//TIM6->SR = ~0x01;//1111 1111 1111 1110
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}
}

void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update) == SET)
	{
		GPIO_ToggleBits(GPIOE,GPIO_Pin_14);
		
		//TIM6->SR = ~0x01;//1111 1111 1111 1110
		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
	}
}



void TIM14_PWMConfig(void)
{
	//��������ΪAF���ù���  
	//��������GPIOF��  GPIOE���ʱ��--�������ǰ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
	/////////////////////��������////////////////////////
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_9;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_AF;//���ģʽ
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;//����
	GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;//���� 2000000Hz
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//����--��������
	GPIO_Init(GPIOF,&GPIOInitStruct);
	
	//ָ��PF9����ΪTIM14����
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14);
	
	
	TIM_TimeBaseInitTypeDef TimInitStruct={0};
	
	//ʹ��ʱ�� APB1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);
	//TIM14ʱ����Ԫ
	TimInitStruct.TIM_Prescaler = 84-1; //1Mhz T = 1us
	TimInitStruct.TIM_Period    = 1000-1;// 1ms  0<CCR<ARR
	TimInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TimInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM14,&TimInitStruct);
	TIM_SetCounter(TIM14,0);
	
	//TIM14����ȽϹ��ܺ�CCR1
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);
	
	//ccrԤװ��ʹ�����ܹ���CCR��ֵ����һ��PWM����֮ǰ�ٸ��£�������������
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);
	
	//����TIM14
	TIM_Cmd(TIM14,ENABLE);
}




