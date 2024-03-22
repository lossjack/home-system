#include "spi.h"

void SPI_Config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	
	/////////////////////引脚配置////////////////////////
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_15;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_OUT;//输出模式
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;//推挽
	GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;//低速
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//浮空--无上下拉
	GPIO_Init(GPIOB,&GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_10;
	GPIO_Init(GPIOD,&GPIOInitStruct);

	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_14;
	GPIO_Init(GPIOE,&GPIOInitStruct);
	
	SPI_SCK = 0;//SPI 模式0  SCK默认为低电平
	
}

void SPI0_ReadWriteByte(unsigned char WData)
{
	for(int i=0;i<8;i++)
	{
		SPI_SCK = 0;//下降沿发送数据
		if(WData & (0x80>>i))
			SPI_MOSI = 1;
		else
			SPI_MOSI = 0;
		delay_us(1);
		
	
		
		SPI_SCK = 1;
		delay_us(1);
	}	
}



