#include "uart.h"


void UART1_Config(unsigned int BaudRate)
{
	//配置引脚
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_AF;//输出模式
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;//推挽
	GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;//低速 2000000Hz
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//浮空--无上下拉
	GPIO_Init(GPIOA,&GPIOInitStruct);
	
	//指定复用
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	//使能串口1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//配置串口1
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//校验位 无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//可收可发
	USART_Init(USART1, &USART_InitStructure);
	
	
	//开启串口的指定中断---RXNE 接收中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	
	//NVIC配置
	NVIC_InitTypeDef NVICInitStruct;
	NVICInitStruct.NVIC_IRQChannel = USART1_IRQn;//全局中断号
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 5;
	NVICInitStruct.NVIC_IRQChannelSubPriority        = 0;
	NVICInitStruct.NVIC_IRQChannelCmd                = ENABLE;//使能
	NVIC_Init(&NVICInitStruct);//配置NVIC
	
	//开启串口
	USART_Cmd(USART1,ENABLE);
}

char RxBuff[512]={0};
int  RxCount=0;//记录下标--串口收到的数据字节数


//void USART1_IRQHandler(void)
//{
//	if(USART1->SR & (0x01<<5))
//	{
//		RxBuff[RxCount]= USART1->DR;
//		RxCount++;
//		//清除标志位
//	}
//}




void  USART1_SendString(char *str,int len)
{
	USART1->SR = ~(0x01<<6);//先清零标志位
	while(len--)
	{
		//"hello\r\n"
		USART1->DR = *str++;
		while(!(USART1->SR & (0x01<<6)));//等待刚刚的字节完全发完
	}
}


int fputc(int ch, FILE * fp)
{
	USART1->SR = ~(0x01<<6);//先清零标志位
	USART1->DR = (unsigned char)ch; // TDR = ch
	while(!(USART1->SR & (0x01<<6)));//等待刚刚的字节完全发完
	
	return 0;
}









void UART3_Config(unsigned int BaudRate)
{
	//配置引脚
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_11;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_AF;//输出模式
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;//推挽
	GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;//低速 2000000Hz
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//浮空--无上下拉
	GPIO_Init(GPIOB,&GPIOInitStruct);
	
	//指定复用
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
	
	//使能串口1时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	//配置串口1
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//校验位 无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//可收可发
	USART_Init(USART3, &USART_InitStructure);
	
	
	//开启串口的指定中断---RXNE 接收中断
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	
	
	//NVIC配置
	NVIC_InitTypeDef NVICInitStruct;
	NVICInitStruct.NVIC_IRQChannel = USART3_IRQn;//全局中断号
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 5;
	NVICInitStruct.NVIC_IRQChannelSubPriority        = 0;
	NVICInitStruct.NVIC_IRQChannelCmd                = ENABLE;//使能
	NVIC_Init(&NVICInitStruct);//配置NVIC
	
	//开启串口
	USART_Cmd(USART3,ENABLE);
}


unsigned char ESP8266_RxBuff[512]={0};
int  ESP8266_RxCount=0;//记录下标--串口收到的数据字节数
char ESP8266_RxFlags=0;

void USART3_IRQHandler(void)
{
	if(USART3->SR & (0x01<<5))
	{
		ESP8266_RxBuff[ESP8266_RxCount] = USART3->DR;

		ESP8266_RxCount++;
		//清除标志位
	}
}



void  USART3_SendString(unsigned char *str,int len)
{
	USART3->SR = ~(0x01<<6);//先清零标志位
	while(len--)
	{
		//"hello\r\n"
		USART3->DR = *str++;
		while(!(USART3->SR & (0x01<<6)));//等待刚刚的字节完全发完
	}
}










