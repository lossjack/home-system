#include "dht11.h"


u8 DHT11_init(void)
{
	//�򿪶˿�G��Ӳ��ʱ�ӣ����ǹ���
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);

	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_9; 	//9������
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_OUT;	//���ģʽ
	GPIOInitStruct.GPIO_OType = GPIO_OType_OD;	//��©
	GPIOInitStruct.GPIO_Speed = GPIO_High_Speed;//���٣��ٶ�Խ�ߣ���ӦԽ�죬���ǹ��Ļ����
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//��ʹ������������
	GPIO_Init(GPIOG,&GPIOInitStruct);

	//ֻҪ�����ģʽ���϶����г�ʼ��ƽ��״̬���������豸��˵����
	GPIO_SetBits(GPIOG,GPIO_Pin_9);
	
	DHT11_Rst();
	
	return DHT11_Check();

}


//�������ģʽѡ��
static void __DHT11_Pin_Select(GPIOMode_TypeDef mode)
{
	//ʱ��ʹ��--G��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	
	//��������
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_9;//9������
	GPIOInitStruct.GPIO_Mode  = mode;//ģʽѡ��
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;// �������
	GPIOInitStruct.GPIO_Speed = GPIO_Fast_Speed;//����
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//�ڲ�����
	
	GPIO_Init(GPIOG,&GPIOInitStruct);
	
}


//��λDHT11
void DHT11_Rst(void)	   
{                 
	__DHT11_Pin_Select(GPIO_Mode_OUT);//SET OUTPUT
    DHT11_DQ_OUT=0; 	//����DQ
    delay_ms(20);    	//��������18ms
    DHT11_DQ_OUT=1; 	//DQ=1 
	delay_us(30);     	//��������20~40us
}

//�ȴ�DHT11�Ļ�Ӧ
//����1:δ��⵽DHT11�Ĵ���
//����0:����
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	__DHT11_Pin_Select(GPIO_Mode_IN);//SET INPUT	 
    while (DHT11_DQ_IN&&retry<100)//DHT11������40~50us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11���ͺ���ٴ�����40~50us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

//��DHT11��ȡһ��λ
//����ֵ��1/0
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)//�ȴ���Ϊ�͵�ƽ 12-14us ��ʼ
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//�ȴ���ߵ�ƽ	 26-28us��ʾ0,116-118us��ʾ1
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//�ȴ�40us
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}

//��DHT11��ȡһ���ֽ�
//����ֵ������������
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

//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
u8 DHT11_Read_Data(u8 *temph,u8 *humih,u8 *templ,u8 *humil)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
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





