#include "iic.h"

void IIC_Config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	/////////////////////引脚配置////////////////////////
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_OUT;//输出模式
	GPIOInitStruct.GPIO_OType = GPIO_OType_OD;//开漏  IIC支持多个从设备 推挽输出引脚输出功率可能不够
	GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;//低速
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_UP;//内部上拉
	GPIO_Init(GPIOB,&GPIOInitStruct);
	
}

static void __IIC_SDAModeSelect(GPIOMode_TypeDef mode)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	/////////////////////引脚配置////////////////////////
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_9;
	GPIOInitStruct.GPIO_Mode  = mode;//输出模式
	GPIOInitStruct.GPIO_OType = GPIO_OType_OD;//开漏  IIC支持多个从设备 推挽输出引脚输出功率可能不够
	GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;//低速
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_UP;//内部上拉
	GPIO_Init(GPIOB,&GPIOInitStruct);
	
}


void IIC_Start(void)
{
	//SDA配置为输出模式
	__IIC_SDAModeSelect(GPIO_Mode_OUT);
	
	
    SCL = 1;
    SDA = 1;
    delay_us(5);//空闲一会
    
    //启动信号开始
    SDA = 0;//SDA 1-->0  下降沿
    delay_us(5);

    SCL = 0;
}


void IIC_Stop(void)
{
	__IIC_SDAModeSelect(GPIO_Mode_OUT);
	
    SCL = 1;
    SDA = 0;
    delay_us(5);
    
    SDA = 1;
}

void  IIC_SendByte(unsigned char  WData)
{
    //SDA配置为输出模式
    __IIC_SDAModeSelect(GPIO_Mode_OUT);
	
    for(int i =0;i <8;i++)
    {
        SCL = 0; //低电平期间主机发送1个位
        delay_us(2);
        if(WData & (0x80>>i))  
            SDA = 1;
        else
            SDA = 0;
        delay_us(3);
        
        SCL = 1;
        delay_us(5); //从机只能收
    }
	
	SCL = 0;
}

unsigned char  IIC_ReadByte(void)
{
    unsigned char  RData=0; //初始化为0
    
    //SDA设置为输入模式
	__IIC_SDAModeSelect(GPIO_Mode_IN);
	
    for(int i=0;i<8;i++)
    {
        SCL = 0;
        delay_us(5);//从机发送一个位的时间
        
		
        SCL = 1;//主机在高电平期间判断SDA来接收一个个位
        delay_us(2);
        if(SDA_R)
            RData |= (0x80>>i);
        delay_us(3);
    }
	
	SCL = 0;
	return RData;//返回读到的结果
}


unsigned char  IIC_ReadACK(void)
{
    unsigned char  ack = 0;
    //SDA设置为输入模式
    __IIC_SDAModeSelect(GPIO_Mode_IN);
	
    SCL = 0;
    delay_us(5);//给从机发送1bit应答数据的事件
    
    SCL = 1;
    delay_us(2);
    if(SDA_R)
         ack = 1;
    delay_us(3);
	
	SCL = 0;
    return  ack;//返回应答结果
}


void  IIC_WriteACK(unsigned char ack)
{
    //设置SDA为输出模式
	__IIC_SDAModeSelect(GPIO_Mode_OUT);
	
    SCL = 0;
    delay_us(2);
    SDA = ack;
    delay_us(3);
    
    SCL = 1;
    delay_us(5);
	
	SCL = 0;
}



