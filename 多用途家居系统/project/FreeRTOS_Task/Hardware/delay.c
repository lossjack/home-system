#include "delay.h"

//����ʱ����
void delay(int tim)
{
	int i;
	while(tim--)
	{
		for(i=384000;i>0;i--);
	}
}



void delay_us(int tus)
{
#if 0
	//Ҫ�������ʱtms---��SysTick�ƶ�Ӧʱ�����
	SysTick->CTRL &= ~(0x01<<2);//ѡ��ʱ��Դ21Mhz
	SysTick->LOAD  = tus*21-1;
	SysTick->VAL   = 0;//��VAL����  �����־λ  ��LOADֵ���ص�VAL
	
	SysTick->CTRL |= 0x01;//����������
	
	//��ʼ�ȴ� �ȴ���ʱ�¼���---CTRL��16bit��λ
	while(1)
	{
		if(SysTick->CTRL & (0x01<<16))
		{
			break;//ʱ�䵽��
		}
	}
	SysTick->CTRL &= ~0x01;//�رռ�����
#endif
	u32 ticks;
	u32 told,tnow,reload,tcnt=0;

	reload = SysTick->LOAD;                     //��ȡ��װ�ؼĴ���ֵ
	ticks = tus * (SystemCoreClock / 1000000);  //����ʱ��ֵ
	told=SysTick->VAL;                          //��ȡ��ǰ��ֵ�Ĵ���ֵ����ʼʱ��ֵ��

	while(1)
	{
		  tnow=SysTick->VAL;          //��ȡ��ǰ��ֵ�Ĵ���ֵ
		  if(tnow!=told)              //��ǰֵ�����ڿ�ʼֵ˵�����ڼ���
		  {         

				 if(tnow<told)             //��ǰֵС�ڿ�ʼ��ֵ��˵��δ�Ƶ�0
					  tcnt+=told-tnow;     //����ֵ=��ʼֵ-��ǰֵ

				 else                  //��ǰֵ���ڿ�ʼ��ֵ��˵���ѼƵ�0�����¼���
						tcnt+=reload-tnow+told;   //����ֵ=��װ��ֵ-��ǰֵ+��ʼֵ  ����
												  //�ӿ�ʼֵ�Ƶ�0�� 

				 told=tnow;                //���¿�ʼֵ
				 if(tcnt>=ticks)break;     //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		  } 
	}     

}


//  tms*21000-1 <= 16777215
//  tms <= 16777216/21000
//  tms <= 798
void delay_ms(int tms)
{
	#if 0
	//Ҫ�������ʱtms---��SysTick�ƶ�Ӧʱ�����
	SysTick->CTRL &= ~(0x01<<2);//ѡ��ʱ��Դ21Mhz
	SysTick->LOAD  = tms*21000-1;
	SysTick->VAL   = 0;//��VAL����  �����־λ  ��LOADֵ���ص�VAL
	
	SysTick->CTRL |= 0x01;//����������
	
	//��ʼ�ȴ� �ȴ���ʱ�¼���---CTRL��16bit��λ
	while(1)
	{
		if(SysTick->CTRL & (0x01<<16))
		{
			break;//ʱ�䵽��
		}
	}
	SysTick->CTRL &= ~0x01;//�رռ�����
	#endif
	
	vTaskDelay(tms);
}


void delay_s(int ts)
{
	while(ts--)
	{
		delay_ms(500);
		delay_ms(500);
	}
}


