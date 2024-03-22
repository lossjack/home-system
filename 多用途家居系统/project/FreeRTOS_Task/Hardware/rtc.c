#include "led.h"
#include "uart.h"
#include "sys.h"
#include "delay.h"
#include "rtc.h"


/* Private variables ---------------------------------------------------------*/
__IO uint32_t TimeDisplay = 0;
 
NVIC_InitTypeDef NVIC_InitStructure;
 
// RTCʱ������
// hour,min,sec:Сʱ,����,����
// ampm:@RTC_AM_PM_Definitions  :RTC_H12_AM/RTC_H12_PM
// ����ֵ:SUCEE(1),�ɹ�
//        ERROR(0),�����ʼ��ģʽʧ��
ErrorStatus RTC_Set_Time(u8 hour, u8 min, u8 sec, u8 ampm)
{
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
 
	RTC_TimeTypeInitStructure.RTC_Hours = hour;
	RTC_TimeTypeInitStructure.RTC_Minutes = min;
	RTC_TimeTypeInitStructure.RTC_Seconds = sec;
	RTC_TimeTypeInitStructure.RTC_H12 = ampm;
 
	return RTC_SetTime(RTC_Format_BIN, &RTC_TimeTypeInitStructure);
}
// RTC��������
// year,month,date:��(0~99),��(1~12),��(0~31)
// week:����(1~7,0,�Ƿ�!)
// ����ֵ:SUCEE(1),�ɹ�
//        ERROR(0),�����ʼ��ģʽʧ��
ErrorStatus RTC_Set_Date(u8 year, u8 month, u8 date, u8 week)
{
 
	RTC_DateTypeDef RTC_DateTypeInitStructure;
	RTC_DateTypeInitStructure.RTC_Date = date;
	RTC_DateTypeInitStructure.RTC_Month = month;
	RTC_DateTypeInitStructure.RTC_WeekDay = week;
	RTC_DateTypeInitStructure.RTC_Year = year;
	return RTC_SetDate(RTC_Format_BIN, &RTC_DateTypeInitStructure);
}
 
// RTC��ʼ��
// ����ֵ:0,��ʼ���ɹ�;
//        1,LSE����ʧ��;
//        2,�����ʼ��ģʽʧ��;
u8 My_RTC_Init(void)
{
	printf("My_RTC_Init - begin\r\n");
	RTC_InitTypeDef RTC_InitStructure;					// ����RTC��ʼ���ṹ��
	u16 retry = 0X1FFF;									// �������Դ���
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); // ʹ��PWRʱ��
	PWR_BackupAccessCmd(ENABLE);						// ʹ�ܺ󱸼Ĵ�������
 
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5053) // ����Ƿ��һ������
	{
		RCC_LSEConfig(RCC_LSE_ON);							// ����LSE�������ⲿ����
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) // ���ָ����RCC��־λ�������,�ȴ����پ������
		{
			retry++;
			delay_us(10);
			//delay_ms(10);
		}
		if (retry == 0)
			return 1; // LSE ����ʧ��.
 
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); // ����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��
		RCC_RTCCLKCmd(ENABLE);					// ʹ��RTCʱ��
 
		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;			  // RTC�첽��Ƶϵ��(1~0X7F)
		RTC_InitStructure.RTC_SynchPrediv = 0xFF;			  // RTCͬ����Ƶϵ��(0~7FFF)
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; // RTC����Ϊ24Сʱ��ʽ
		RTC_Init(&RTC_InitStructure);						  // ��ʼ��RTC
 
		RTC_WriteBackupRegister(RTC_BKP_DR0, 0x5053); // ����Ѿ���ʼ������
	}
	
	printf("My_RTC_Init - end\r\n");
	return 0;
}
 
// ��������ʱ��(����������,24Сʱ��)
// week:���ڼ�(1~7) @ref  RTC_Alarm_Definitions
// hour,min,sec:Сʱ,����,����
void RTC_Set_AlarmA(u8 week, u8 hour, u8 min, u8 sec)
{
	EXTI_InitTypeDef EXTI_InitStructure;		 // �����ⲿ�ж��߽ṹ��
	RTC_AlarmTypeDef RTC_AlarmTypeInitStructure; // ����RTC���ӽṹ��
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;	 // ����RTCʱ��ṹ��
 
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE); // �ر�����A
 
	RTC_TimeTypeInitStructure.RTC_Hours = hour;		// ����Сʱ
	RTC_TimeTypeInitStructure.RTC_Minutes = min;	/// ���÷���
	RTC_TimeTypeInitStructure.RTC_Seconds = sec;	// ��������
	RTC_TimeTypeInitStructure.RTC_H12 = RTC_H12_AM; // ����Ϊ����
 
	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDay = week;								  // ��������
	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay; // ��������
	RTC_AlarmTypeInitStructure.RTC_AlarmMask = RTC_AlarmMask_None;						  // ��ȷƥ�����ڣ�ʱ����
	RTC_AlarmTypeInitStructure.RTC_AlarmTime = RTC_TimeTypeInitStructure;				  // ��������ʱ��
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmTypeInitStructure);				  // ��������
 
	RTC_ClearITPendingBit(RTC_IT_ALRA);	 // ���RTC����A�ı�־
	EXTI_ClearITPendingBit(EXTI_Line17); // ���LINE17�ϵ��жϱ�־λ
 
	RTC_ITConfig(RTC_IT_ALRA, ENABLE); // ��������A�ж�
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE); // ��������A
 
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;			   // LINE17
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	   // �ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // �����ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;			   // ʹ��LINE17
	EXTI_Init(&EXTI_InitStructure);						   // �����ⲿ�ж���
 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; // ��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;		 // �����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 // ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);								 // �����ж�������
}
 
// �����Ի��Ѷ�ʱ������
/*wksel:  @ref RTC_Wakeup_Timer_Definitions
#define RTC_WakeUpClock_RTCCLK_Div16        ((uint32_t)0x00000000)
#define RTC_WakeUpClock_RTCCLK_Div8         ((uint32_t)0x00000001)
#define RTC_WakeUpClock_RTCCLK_Div4         ((uint32_t)0x00000002)
#define RTC_WakeUpClock_RTCCLK_Div2         ((uint32_t)0x00000003)
#define RTC_WakeUpClock_CK_SPRE_16bits      ((uint32_t)0x00000004)
#define RTC_WakeUpClock_CK_SPRE_17bits      ((uint32_t)0x00000006)
*/
// cnt:�Զ���װ��ֵ.����0,�����ж�.
/*���ѹ��ܿ�����RTCʵʱʱ�����趨��ʱ�������󴥷��жϣ�
�Ӷ�����ϵͳ��ִ���ض����������磬�ڵ͹���Ӧ���У�
ϵͳ���ܻ����˯��ģʽ�Խ�ʡ����������Ȼ��Ҫ��ĳ��ʱ��������ִ��ĳЩ����
���������ʾ���ɼ����������ݵȡ�ͨ�����û��ѹ��ܣ�
RTCʵʱʱ�ӿ�����Ԥ����ʱ�������󴥷��жϣ��Ӷ�����ϵͳ��ִ����Ӧ������*/
void RTC_Set_WakeUp(u32 wksel, u16 cnt)
{
	EXTI_InitTypeDef EXTI_InitStructure;
 
	RTC_WakeUpCmd(DISABLE); // �ر�WAKE UP
 
	RTC_WakeUpClockConfig(wksel); // ����ʱ��ѡ��
 
	RTC_SetWakeUpCounter(cnt); // ����WAKE UP�Զ���װ�ؼĴ���
 
	RTC_ClearITPendingBit(RTC_IT_WUT);	 // ���RTC WAKE UP�ı�־
	EXTI_ClearITPendingBit(EXTI_Line22); // ���LINE22�ϵ��жϱ�־λ
 
	RTC_ITConfig(RTC_IT_WUT, ENABLE); // ����WAKE UP ��ʱ���ж�
	RTC_WakeUpCmd(ENABLE);			  // ����WAKE UP ��ʱ����
 
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;			   // LINE22
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	   // �ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // �����ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;			   // ʹ��LINE22
	EXTI_Init(&EXTI_InitStructure);						   // ����
 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; // ��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;		 // �����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 // ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);								 // ����NVIC�ж�������
}
 
// RTC�����жϷ�����
void RTC_Alarm_IRQHandler(void)
{
	if (RTC_GetFlagStatus(RTC_FLAG_ALRAF) == SET) // ALARM A�ж�?
	{
		RTC_ClearFlag(RTC_FLAG_ALRAF); // ����жϱ�־
		printf("ALARM A!\r\n");
	}
	EXTI_ClearITPendingBit(EXTI_Line17); // ����ж���17���жϱ�־
}
 
// RTC WAKE UP�жϷ�����
void RTC_WKUP_IRQHandler(void)
{
	if (RTC_GetFlagStatus(RTC_FLAG_WUTF) == SET) // WK_UP�ж�?
	{
		RTC_ClearFlag(RTC_FLAG_WUTF); // ����жϱ�־
		PEout(14) = !PEout(14);
	}
	EXTI_ClearITPendingBit(EXTI_Line22); // ����ж���22���жϱ�־
}

