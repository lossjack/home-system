#include "led.h"



void LED_Config(void)
{
	//��������GPIOF��  GPIOE���ʱ��--�������ǰ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	/////////////////////��������////////////////////////
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_OUT;//���ģʽ
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;//����
	GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;//����
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//����--��������
	GPIO_Init(GPIOF,&GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOE,&GPIOInitStruct);
	
	//��ȫ��
	GPIO_WriteBit(GPIOF,GPIO_Pin_9 | GPIO_Pin_10,(BitAction)1);
	GPIO_WriteBit(GPIOE,GPIO_Pin_13 | GPIO_Pin_14,(BitAction)1);
}

//no �ڼ���LED  1~4 
//sw ����״̬ 1-��  0-��
void LED_Ctr(char no,char sw)
{
	switch(no)
	{
		case 1:
			if(sw)
				GPIO_WriteBit(GPIOF,GPIO_Pin_9,(BitAction)0);//����
			else
				GPIO_WriteBit(GPIOF,GPIO_Pin_9,(BitAction)1);//���
			break;
		case 2:
			if(sw)
				GPIO_WriteBit(GPIOF,GPIO_Pin_10,(BitAction)0);//����
			else
				GPIO_WriteBit(GPIOF,GPIO_Pin_10,(BitAction)1);//���
			break;
		case 3:
			if(sw)
				GPIO_WriteBit(GPIOE,GPIO_Pin_13,(BitAction)0);//����
			else
				GPIO_WriteBit(GPIOE,GPIO_Pin_13,(BitAction)1);//���
			break;
		case 4:
			if(sw)
				GPIO_WriteBit(GPIOE,GPIO_Pin_14,(BitAction)0);//����
			else
				GPIO_WriteBit(GPIOE,GPIO_Pin_14,(BitAction)1);//���
			break;
	}
}


