#include "delay.h"

//粗延时函数
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
	//要计算出延时tms---让SysTick计对应时间的数
	SysTick->CTRL &= ~(0x01<<2);//选择时钟源21Mhz
	SysTick->LOAD  = tus*21-1;
	SysTick->VAL   = 0;//将VAL清零  清零标志位  将LOAD值加载到VAL
	
	SysTick->CTRL |= 0x01;//开启计数器
	
	//开始等待 等待延时事件到---CTRL的16bit置位
	while(1)
	{
		if(SysTick->CTRL & (0x01<<16))
		{
			break;//时间到了
		}
	}
	SysTick->CTRL &= ~0x01;//关闭计数器
#endif
	u32 ticks;
	u32 told,tnow,reload,tcnt=0;

	reload = SysTick->LOAD;                     //获取重装载寄存器值
	ticks = tus * (SystemCoreClock / 1000000);  //计数时间值
	told=SysTick->VAL;                          //获取当前数值寄存器值（开始时数值）

	while(1)
	{
		  tnow=SysTick->VAL;          //获取当前数值寄存器值
		  if(tnow!=told)              //当前值不等于开始值说明已在计数
		  {         

				 if(tnow<told)             //当前值小于开始数值，说明未计到0
					  tcnt+=told-tnow;     //计数值=开始值-当前值

				 else                  //当前值大于开始数值，说明已计到0并重新计数
						tcnt+=reload-tnow+told;   //计数值=重装载值-当前值+开始值  （已
												  //从开始值计到0） 

				 told=tnow;                //更新开始值
				 if(tcnt>=ticks)break;     //时间超过/等于要延迟的时间,则退出.
		  } 
	}     

}


//  tms*21000-1 <= 16777215
//  tms <= 16777216/21000
//  tms <= 798
void delay_ms(int tms)
{
	#if 0
	//要计算出延时tms---让SysTick计对应时间的数
	SysTick->CTRL &= ~(0x01<<2);//选择时钟源21Mhz
	SysTick->LOAD  = tms*21000-1;
	SysTick->VAL   = 0;//将VAL清零  清零标志位  将LOAD值加载到VAL
	
	SysTick->CTRL |= 0x01;//开启计数器
	
	//开始等待 等待延时事件到---CTRL的16bit置位
	while(1)
	{
		if(SysTick->CTRL & (0x01<<16))
		{
			break;//时间到了
		}
	}
	SysTick->CTRL &= ~0x01;//关闭计数器
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


