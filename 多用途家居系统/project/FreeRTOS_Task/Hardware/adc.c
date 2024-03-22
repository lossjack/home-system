#include "adc.h"
#include "stdio.h"
#include "delay.h"


unsigned short ADC1_Val[5];//RAM�е�DMA����ADC1->DR��Ŀ�ĵ� RAM�ռ�

void ADC1_Config(void)
{
	//��������
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIOInitStruct={0};
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_5;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_AN;//����ģ������ģʽ
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//����--��������
	GPIO_Init(GPIOA,&GPIOInitStruct);
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//ʱ��ʹ��
	DMA_InitTypeDef       DMA_InitStructure={0};
	DMA_InitStructure.DMA_Channel = DMA_Channel_0; //ѡ��һ��DMAͨ�� 0~7
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	//ȷ��DMA�İ��˷��� ����Ĵ���--->RAM
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));//����Ĵ����׵�ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC1_Val;//�ڴ�ĳ��������ռ���׵�ַ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//������ʼ��ַ�������ݿ�� 2Byte 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�ڴ�RAMĿ�ĵ����������ݿ�� 2Byte
	DMA_InitStructure.DMA_BufferSize = 5;//�����������  ѭ��ģʽ����0 ��Ż��Զ�����Ϊԭ����5  ��ͨģʽ����
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ����ʹ��
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ����ʹ��
	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA����ģʽ  ����ģʽ  ѭ��ģʽ
	
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//����ǰ���������������ȼ� �� ��  ��  �ǳ���
	
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;//����ģʽѡ��  ֱ��ģʽ  ����FIFOģʽ       
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;//FIFO����ģʽ����ѡ��--ֱ��ģʽ���ù�
	DMA_InitStructure.DMA_MemoryBurst = 	DMA_MemoryBurst_Single;//������
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//������
	
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);//����DMA
	DMA_Cmd(DMA2_Stream0, ENABLE);//ʹ��DMA

	
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	//3��ADC�������ֳ�ʼ��
	ADC_CommonInitTypeDef ADC_CommonInitStructure={0};
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ��������
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//84Mhz/4 = 21Mhz<36Mhz��������
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//ֻ�Զ���ADCģʽ���� ����ADC����
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//ֻ�Զ���ADCģʽ���� ����ADC����
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	//���ĳ������ADC������  ADC1
	ADC_InitTypeDef       ADC_InitStructure={0};
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//�ֱ���  ����  6  8  10  12
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//ɨ��ת��ģʽ ������ADC���˶��ͨ��  �أ���ADC��ͨ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//����ת��ģʽ  ��������ת��   �أ�����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//�ⲿ����ת���ı���ѡ��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//�ⲿ����Դѡ��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //12bit���ݷ���16bit�Ĵ�����������  ��
	ADC_InitStructure.ADC_NbrOfConversion = 1;     //ת����ͨ������  1
	ADC_Init(ADC1, &ADC_InitStructure);

	//ת����������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles); //12T+3T = 15T

	//����ADC1��  CR2  DDS--bit9
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	
	//ʹ��ADC��DMAģʽ  CR2 DMA--bit8
	ADC_DMACmd(ADC1, ENABLE);

	
	//ʹ��ADC
	ADC_Cmd(ADC1,ENABLE);
	
	//����ת��  ������--�����Ĵ���������    Ӳ����--�ⲿ��������
	ADC_SoftwareStartConv(ADC1);
}


void ADC3_Config(void)
{
	//��������
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	GPIO_InitTypeDef GPIOInitStruct={0};
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_7;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_AN;//����ģ������ģʽ
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//����--��������
	GPIO_Init(GPIOF,&GPIOInitStruct);
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	//3��ADC�������ֳ�ʼ��
	ADC_CommonInitTypeDef ADC_CommonInitStructure={0};
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ��������
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//84Mhz/4 = 21Mhz<36Mhz��������
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//ֻ�Զ���ADCģʽ���� ����ADC����
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//ֻ�Զ���ADCģʽ���� ����ADC����
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	//���ĳ������ADC������  ADC3
	ADC_InitTypeDef       ADC_InitStructure={0};
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//�ֱ���  ����  6  8  10  12
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//ɨ��ת��ģʽ ������ADC���˶��ͨ��  �أ���ADC��ͨ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//����ת��ģʽ  ��������ת��   �أ�����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//�ⲿ����ת���ı���ѡ��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//�ⲿ����Դѡ��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //12bit���ݷ���16bit�Ĵ�����������  ��
	ADC_InitStructure.ADC_NbrOfConversion = 1;     //ת����ͨ������  1
	ADC_Init(ADC3, &ADC_InitStructure);

	//ת����������
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_3Cycles); //12T+3T = 15T

	
//	//����ADC��ָ���ж�---EOC �����ж�
//	ADC_ITConfig(ADC3,ADC_IT_EOC,ENABLE);
	
	//NVIC����
	NVIC_InitTypeDef NVICInitStruct;
	NVICInitStruct.NVIC_IRQChannel = ADC_IRQn;//ȫ���жϺ�
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVICInitStruct.NVIC_IRQChannelSubPriority        = 0;
	NVICInitStruct.NVIC_IRQChannelCmd                = ENABLE;//ʹ��
	NVIC_Init(&NVICInitStruct);//����NVIC	
	
	//ʹ��ADC
	ADC_Cmd(ADC3,ENABLE);
	
	//����ת��  ������--�����Ĵ���������    Ӳ����--�ⲿ��������
	ADC_SoftwareStartConv(ADC3);
}



//void ADC_IRQHandler(void)
//{
//	/* ִ��������������ж�*/
//	taskENTER_CRITICAL_FROM_ISR();
//	if(ADC3->SR & 0x02)
//	{
//		TIM14->CCR1 =1000 - (ADC3->DR-870)*1000/(3800-870);//���ݹ�������ʵ��ֵ��Χ�Լ��޸� ���ҵƹ������880+ ���Ƹ������3700+
//		printf("ADC3_IN5���:%d\r\n",ADC3->DR);
//		/* ����ʹ���ж� */
//		delay_ms(1000);
//	}
//	taskEXIT_CRITICAL_FROM_ISR(1);
//	
//}

