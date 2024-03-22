#include "uart.h"


void UART1_Config(unsigned int BaudRate)
{
	//��������
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_AF;//���ģʽ
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;//����
	GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;//���� 2000000Hz
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//����--��������
	GPIO_Init(GPIOA,&GPIOInitStruct);
	
	//ָ������
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	//ʹ�ܴ���1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//���ô���1
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//У��λ ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//���տɷ�
	USART_Init(USART1, &USART_InitStructure);
	
	
	//�������ڵ�ָ���ж�---RXNE �����ж�
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	
	//NVIC����
	NVIC_InitTypeDef NVICInitStruct;
	NVICInitStruct.NVIC_IRQChannel = USART1_IRQn;//ȫ���жϺ�
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 5;
	NVICInitStruct.NVIC_IRQChannelSubPriority        = 0;
	NVICInitStruct.NVIC_IRQChannelCmd                = ENABLE;//ʹ��
	NVIC_Init(&NVICInitStruct);//����NVIC
	
	//��������
	USART_Cmd(USART1,ENABLE);
}

char RxBuff[512]={0};
int  RxCount=0;//��¼�±�--�����յ��������ֽ���


//void USART1_IRQHandler(void)
//{
//	if(USART1->SR & (0x01<<5))
//	{
//		RxBuff[RxCount]= USART1->DR;
//		RxCount++;
//		//�����־λ
//	}
//}




void  USART1_SendString(char *str,int len)
{
	USART1->SR = ~(0x01<<6);//�������־λ
	while(len--)
	{
		//"hello\r\n"
		USART1->DR = *str++;
		while(!(USART1->SR & (0x01<<6)));//�ȴ��ոյ��ֽ���ȫ����
	}
}


int fputc(int ch, FILE * fp)
{
	USART1->SR = ~(0x01<<6);//�������־λ
	USART1->DR = (unsigned char)ch; // TDR = ch
	while(!(USART1->SR & (0x01<<6)));//�ȴ��ոյ��ֽ���ȫ����
	
	return 0;
}









void UART3_Config(unsigned int BaudRate)
{
	//��������
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_11;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_AF;//���ģʽ
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;//����
	GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;//���� 2000000Hz
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//����--��������
	GPIO_Init(GPIOB,&GPIOInitStruct);
	
	//ָ������
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
	
	//ʹ�ܴ���1ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	//���ô���1
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//У��λ ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//���տɷ�
	USART_Init(USART3, &USART_InitStructure);
	
	
	//�������ڵ�ָ���ж�---RXNE �����ж�
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	
	
	//NVIC����
	NVIC_InitTypeDef NVICInitStruct;
	NVICInitStruct.NVIC_IRQChannel = USART3_IRQn;//ȫ���жϺ�
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 5;
	NVICInitStruct.NVIC_IRQChannelSubPriority        = 0;
	NVICInitStruct.NVIC_IRQChannelCmd                = ENABLE;//ʹ��
	NVIC_Init(&NVICInitStruct);//����NVIC
	
	//��������
	USART_Cmd(USART3,ENABLE);
}


unsigned char ESP8266_RxBuff[512]={0};
int  ESP8266_RxCount=0;//��¼�±�--�����յ��������ֽ���
char ESP8266_RxFlags=0;

void USART3_IRQHandler(void)
{
	if(USART3->SR & (0x01<<5))
	{
		ESP8266_RxBuff[ESP8266_RxCount] = USART3->DR;

		ESP8266_RxCount++;
		//�����־λ
	}
}



void  USART3_SendString(unsigned char *str,int len)
{
	USART3->SR = ~(0x01<<6);//�������־λ
	while(len--)
	{
		//"hello\r\n"
		USART3->DR = *str++;
		while(!(USART3->SR & (0x01<<6)));//�ȴ��ոյ��ֽ���ȫ����
	}
}










