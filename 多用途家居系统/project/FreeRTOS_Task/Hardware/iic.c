#include "iic.h"

void IIC_Config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	/////////////////////��������////////////////////////
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_OUT;//���ģʽ
	GPIOInitStruct.GPIO_OType = GPIO_OType_OD;//��©  IIC֧�ֶ�����豸 �����������������ʿ��ܲ���
	GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;//����
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_UP;//�ڲ�����
	GPIO_Init(GPIOB,&GPIOInitStruct);
	
}

static void __IIC_SDAModeSelect(GPIOMode_TypeDef mode)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	/////////////////////��������////////////////////////
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_9;
	GPIOInitStruct.GPIO_Mode  = mode;//���ģʽ
	GPIOInitStruct.GPIO_OType = GPIO_OType_OD;//��©  IIC֧�ֶ�����豸 �����������������ʿ��ܲ���
	GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;//����
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_UP;//�ڲ�����
	GPIO_Init(GPIOB,&GPIOInitStruct);
	
}


void IIC_Start(void)
{
	//SDA����Ϊ���ģʽ
	__IIC_SDAModeSelect(GPIO_Mode_OUT);
	
	
    SCL = 1;
    SDA = 1;
    delay_us(5);//����һ��
    
    //�����źſ�ʼ
    SDA = 0;//SDA 1-->0  �½���
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
    //SDA����Ϊ���ģʽ
    __IIC_SDAModeSelect(GPIO_Mode_OUT);
	
    for(int i =0;i <8;i++)
    {
        SCL = 0; //�͵�ƽ�ڼ���������1��λ
        delay_us(2);
        if(WData & (0x80>>i))  
            SDA = 1;
        else
            SDA = 0;
        delay_us(3);
        
        SCL = 1;
        delay_us(5); //�ӻ�ֻ����
    }
	
	SCL = 0;
}

unsigned char  IIC_ReadByte(void)
{
    unsigned char  RData=0; //��ʼ��Ϊ0
    
    //SDA����Ϊ����ģʽ
	__IIC_SDAModeSelect(GPIO_Mode_IN);
	
    for(int i=0;i<8;i++)
    {
        SCL = 0;
        delay_us(5);//�ӻ�����һ��λ��ʱ��
        
		
        SCL = 1;//�����ڸߵ�ƽ�ڼ��ж�SDA������һ����λ
        delay_us(2);
        if(SDA_R)
            RData |= (0x80>>i);
        delay_us(3);
    }
	
	SCL = 0;
	return RData;//���ض����Ľ��
}


unsigned char  IIC_ReadACK(void)
{
    unsigned char  ack = 0;
    //SDA����Ϊ����ģʽ
    __IIC_SDAModeSelect(GPIO_Mode_IN);
	
    SCL = 0;
    delay_us(5);//���ӻ�����1bitӦ�����ݵ��¼�
    
    SCL = 1;
    delay_us(2);
    if(SDA_R)
         ack = 1;
    delay_us(3);
	
	SCL = 0;
    return  ack;//����Ӧ����
}


void  IIC_WriteACK(unsigned char ack)
{
    //����SDAΪ���ģʽ
	__IIC_SDAModeSelect(GPIO_Mode_OUT);
	
    SCL = 0;
    delay_us(2);
    SDA = ack;
    delay_us(3);
    
    SCL = 1;
    delay_us(5);
	
	SCL = 0;
}



