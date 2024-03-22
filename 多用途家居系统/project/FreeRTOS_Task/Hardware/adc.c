#include "adc.h"
#include "stdio.h"
#include "delay.h"


unsigned short ADC1_Val[5];//RAM中的DMA搬运ADC1->DR的目的地 RAM空间

void ADC1_Config(void)
{
	//配置引脚
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIOInitStruct={0};
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_5;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_AN;//浮空模拟输入模式
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//浮空--无上下拉
	GPIO_Init(GPIOA,&GPIOInitStruct);
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//时钟使能
	DMA_InitTypeDef       DMA_InitStructure={0};
	DMA_InitStructure.DMA_Channel = DMA_Channel_0; //选择一个DMA通道 0~7
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	//确定DMA的搬运方向 外设寄存器--->RAM
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));//外设寄存器首地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC1_Val;//内存某个已申请空间的首地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设起始地址搬运数据宽度 2Byte 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//内存RAM目的地容器的数据宽度 2Byte
	DMA_InitStructure.DMA_BufferSize = 5;//传输的数据量  循环模式减到0 后才会自动重载为原来的5  普通模式不会
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址递增使能
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址递增使能
	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA传输模式  正常模式  循环模式
	
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//给当前搬运任务配置优先级 低 中  高  非常高
	
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;//缓冲模式选择  直接模式  缓冲FIFO模式       
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;//FIFO缓冲模式的子选择--直接模式不用管
	DMA_InitStructure.DMA_MemoryBurst = 	DMA_MemoryBurst_Single;//单脉冲
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//单脉冲
	
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);//配置DMA
	DMA_Cmd(DMA2_Stream0, ENABLE);//使能DMA

	
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	//3个ADC公共部分初始化
	ADC_CommonInitTypeDef ADC_CommonInitStructure={0};
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式！！！！
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//84Mhz/4 = 21Mhz<36Mhz！！！！
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//只对多重ADC模式有用 独立ADC不用
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//只对多重ADC模式有用 独立ADC不用
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	//针对某个独立ADC的配置  ADC1
	ADC_InitTypeDef       ADC_InitStructure={0};
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//分辨率  精度  6  8  10  12
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//扫描转换模式 开：单ADC开了多个通道  关：单ADC单通道
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//连续转换模式  开：连续转换   关：单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//外部触发转换的边沿选择
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//外部触发源选择
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //12bit数据放在16bit寄存器的左还是右  右
	ADC_InitStructure.ADC_NbrOfConversion = 1;     //转换的通道数量  1
	ADC_Init(ADC1, &ADC_InitStructure);

	//转换规则配置
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles); //12T+3T = 15T

	//配置ADC1在  CR2  DDS--bit9
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	
	//使能ADC的DMA模式  CR2 DMA--bit8
	ADC_DMACmd(ADC1, ENABLE);

	
	//使能ADC
	ADC_Cmd(ADC1,ENABLE);
	
	//启动转换  软启动--操作寄存器来启动    硬启动--外部触发启动
	ADC_SoftwareStartConv(ADC1);
}


void ADC3_Config(void)
{
	//配置引脚
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	GPIO_InitTypeDef GPIOInitStruct={0};
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_7;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_AN;//浮空模拟输入模式
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//浮空--无上下拉
	GPIO_Init(GPIOF,&GPIOInitStruct);
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	//3个ADC公共部分初始化
	ADC_CommonInitTypeDef ADC_CommonInitStructure={0};
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式！！！！
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//84Mhz/4 = 21Mhz<36Mhz！！！！
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//只对多重ADC模式有用 独立ADC不用
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//只对多重ADC模式有用 独立ADC不用
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	//针对某个独立ADC的配置  ADC3
	ADC_InitTypeDef       ADC_InitStructure={0};
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//分辨率  精度  6  8  10  12
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//扫描转换模式 开：单ADC开了多个通道  关：单ADC单通道
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//连续转换模式  开：连续转换   关：单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//外部触发转换的边沿选择
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//外部触发源选择
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //12bit数据放在16bit寄存器的左还是右  右
	ADC_InitStructure.ADC_NbrOfConversion = 1;     //转换的通道数量  1
	ADC_Init(ADC3, &ADC_InitStructure);

	//转换规则配置
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_3Cycles); //12T+3T = 15T

	
//	//开启ADC的指定中断---EOC 接收中断
//	ADC_ITConfig(ADC3,ADC_IT_EOC,ENABLE);
	
	//NVIC配置
	NVIC_InitTypeDef NVICInitStruct;
	NVICInitStruct.NVIC_IRQChannel = ADC_IRQn;//全局中断号
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVICInitStruct.NVIC_IRQChannelSubPriority        = 0;
	NVICInitStruct.NVIC_IRQChannelCmd                = ENABLE;//使能
	NVIC_Init(&NVICInitStruct);//配置NVIC	
	
	//使能ADC
	ADC_Cmd(ADC3,ENABLE);
	
	//启动转换  软启动--操作寄存器来启动    硬启动--外部触发启动
	ADC_SoftwareStartConv(ADC3);
}



//void ADC_IRQHandler(void)
//{
//	/* 执行这句代码后，屏蔽中断*/
//	taskENTER_CRITICAL_FROM_ISR();
//	if(ADC3->SR & 0x02)
//	{
//		TIM14->CCR1 =1000 - (ADC3->DR-870)*1000/(3800-870);//根据光敏电阻实际值范围稍加修改 教室灯光下最低880+ 手掌覆盖最高3700+
//		printf("ADC3_IN5结果:%d\r\n",ADC3->DR);
//		/* 重新使能中断 */
//		delay_ms(1000);
//	}
//	taskEXIT_CRITICAL_FROM_ISR(1);
//	
//}

