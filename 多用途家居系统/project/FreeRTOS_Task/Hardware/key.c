#include "key.h"


void KEY_Config(void)
{
	//��������GPIOF��  GPIOE���ʱ��--�������ǰ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	/////////////////////��������////////////////////////
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_0;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_IN;//����ģʽ
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//����--��������
	GPIO_Init(GPIOA,&GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init(GPIOE,&GPIOInitStruct);
}


//����1--��һ���������� 2---�ڶ�����������  3  4    0--û���κΰ�������
unsigned char KEY_Get(void)
{
	unsigned char KeyFlag=0;
	
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 0)
		KeyFlag |= 0x01;//��¼���ǵ�һ����������
	
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) == 0)
		KeyFlag |= 0x01<<1;//��¼���ǵڶ�����������
		
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)
		KeyFlag |= 0x01<<2;//��¼���ǵڶ�����������
	
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)
		KeyFlag |= 0x01<<3;//��¼���ǵڶ�����������

	return KeyFlag;
}




void KEY_EXTIConfig(void)
{
	//0��ʹ��EXTI(����Ҫʹ�� Ĭ�Ͽ���)��SYSCFGģ���ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	//1��ѡ���Ӧ��EXTIx----ָ������
	//PA0----EXTI0
	SYSCFG->EXTICR[0] &= ~0x0F;
	
	//2��EXTI����---ֻ����½���+
	EXTI->FTSR |= 0x01;//ѡ���½��ش���---���EXTI0�½����Ƿ��д���
	EXTI->RTSR &= ~0x01;//�����������
	EXTI->IMR  |= 0x01;//����EXTI0���ж�---ȥ������
	
	
	
	//3������NVIC---����EXTI0���ж����ȼ�
	//NVIC_Init()
	NVIC_InitTypeDef NVICInitStruct;
	NVICInitStruct.NVIC_IRQChannel = EXTI0_IRQn;//ȫ���жϺ�
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 10;//0~15
	NVICInitStruct.NVIC_IRQChannelSubPriority        = 0;
	NVICInitStruct.NVIC_IRQChannelCmd                = ENABLE;//ʹ��
	NVIC_Init(&NVICInitStruct);//����NVIC
}

//�жϷ�����----�жϴ�����CPU��ת�����ĺ���
void EXTI0_IRQHandler(void)
{
	if(EXTI->PR & 0x01)  //0000 0000 0000 0000 0000 0000 ... 0001
	{
		//��־λ��λ---�����½��ش���
		//delay(2);
		//GPIO_ToggleBits(GPIOF,GPIO_Pin_9);
		
		//�����־λ rc_w1  д1���� д0��Ӱ��
		EXTI->PR = 0x01;//0000 0000 0000 0001
	}
}


