#include "spi.h"

void SPI_Config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	
	/////////////////////��������////////////////////////
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_15;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_OUT;//���ģʽ
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;//����
	GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;//����
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//����--��������
	GPIO_Init(GPIOB,&GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_10;
	GPIO_Init(GPIOD,&GPIOInitStruct);

	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_14;
	GPIO_Init(GPIOE,&GPIOInitStruct);
	
	SPI_SCK = 0;//SPI ģʽ0  SCKĬ��Ϊ�͵�ƽ
	
}

void SPI0_ReadWriteByte(unsigned char WData)
{
	for(int i=0;i<8;i++)
	{
		SPI_SCK = 0;//�½��ط�������
		if(WData & (0x80>>i))
			SPI_MOSI = 1;
		else
			SPI_MOSI = 0;
		delay_us(1);
		
	
		
		SPI_SCK = 1;
		delay_us(1);
	}	
}



