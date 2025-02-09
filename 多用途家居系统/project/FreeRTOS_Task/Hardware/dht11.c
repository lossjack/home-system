#include "dht11.h"


u8 DHT11_init(void)
{
	//打开端口G的硬件时钟，就是供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);

	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_9; 	//9号引脚
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_OUT;	//输出模式
	GPIOInitStruct.GPIO_OType = GPIO_OType_OD;	//开漏
	GPIOInitStruct.GPIO_Speed = GPIO_High_Speed;//高速，速度越高，响应越快，但是功耗会更高
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//不使能上下拉电阻
	GPIO_Init(GPIOG,&GPIOInitStruct);

	//只要有输出模式，肯定会有初始电平的状态，看连接设备的说明书
	GPIO_SetBits(GPIOG,GPIO_Pin_9);
	
	DHT11_Rst();
	
	return DHT11_Check();

}


//输入输出模式选择
static void __DHT11_Pin_Select(GPIOMode_TypeDef mode)
{
	//时钟使能--G组
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	
	//引脚配置
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_9;//9号引脚
	GPIOInitStruct.GPIO_Mode  = mode;//模式选择
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;// 推挽输出
	GPIOInitStruct.GPIO_Speed = GPIO_Fast_Speed;//快速
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//内部上拉
	
	GPIO_Init(GPIOG,&GPIOInitStruct);
	
}


//复位DHT11
void DHT11_Rst(void)	   
{                 
	__DHT11_Pin_Select(GPIO_Mode_OUT);//SET OUTPUT
    DHT11_DQ_OUT=0; 	//拉低DQ
    delay_ms(20);    	//拉低至少18ms
    DHT11_DQ_OUT=1; 	//DQ=1 
	delay_us(30);     	//主机拉高20~40us
}

//等待DHT11的回应
//返回1:未检测到DHT11的存在
//返回0:存在
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	__DHT11_Pin_Select(GPIO_Mode_IN);//SET INPUT	 
    while (DHT11_DQ_IN&&retry<100)//DHT11会拉低40~50us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11拉低后会再次拉高40~50us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

//从DHT11读取一个位
//返回值：1/0
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)//等待变为低电平 12-14us 开始
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//等待变高电平	 26-28us表示0,116-118us表示1
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//等待40us
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}

//从DHT11读取一个字节
//返回值：读到的数据
u8 DHT11_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}

//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
u8 DHT11_Read_Data(u8 *temph,u8 *humih,u8 *templ,u8 *humil)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humih=buf[0];
			*humil=buf[1];
			*temph=buf[2];
			*templ=buf[4];
		}
		
	}else return 1;
	return 0;	    
}





