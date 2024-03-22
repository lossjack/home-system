#include "stm32f4xx.h" //оƬ�����мĴ�������
#include "led.h"
#include "delay.h"
#include "key.h"
#include "beep.h"
#include "tim.h"
#include "uart.h"
#include "sys.h"
#include "oled.h"
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "rtc.h"
#include "dht11.h"
#include "adc.h"

//����2���������������Ϣ��������ƿ���� TCB
TaskHandle_t Task1OLEDTCB;
TaskHandle_t Task2KEYTCB;

TaskHandle_t TaskRTCTCB;
TaskHandle_t TaskDHT11TCB;

TaskHandle_t TaskBeepCLOCKTCB;
TaskHandle_t TaskLedTCB;
TaskHandle_t TaskBlueToothTCB;


QueueHandle_t xQueue1;//ȫ�ֶ��о��

u32 QueueType;

u32 hour=19;
u32 minute=7;
u32 second=0;

typedef struct{
	char QueueType;//������������  0������������  1��RTC�����͵����� 2��....
	int  Data;
	int  set_time;//0:hour 1:minute 2:second
}QueueStruct;


char* oled_time(){
	RTC_TimeTypeDef RTC_TimeStruct; // ���ڴ洢RTC��ʱ��
	RTC_DateTypeDef RTC_DateStruct; // ���ڴ洢������Ϣ
	
	u8 tbuf[80]={0}; // ���ڴ洢��ʽ�����ʱ��������ַ���
	u8 t = 0;
	
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct); // ��ȡ��ǰ��������Ϣ
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct); // ��ȡ��ǰ��ʱ����Ϣ
	
	// �����ʽ��Ϊ�ַ����洢��tbuf�У�Ȼ��ͨ��LCD��ʾ����
	sprintf((char *)tbuf, "20%02d-%02d-%02d %02d:%02d:%02d",RTC_DateStruct.RTC_Year, RTC_DateStruct.RTC_Month, RTC_DateStruct.RTC_Date, RTC_TimeStruct.RTC_Hours, RTC_TimeStruct.RTC_Minutes, RTC_TimeStruct.RTC_Seconds);
	printf("%s\n",tbuf);
	return (char *)tbuf;
}

void Oled_show_clock(){
	RTC_TimeTypeDef RTC_TimeStruct; // ���ڴ洢RTC��ʱ��
	RTC_DateTypeDef RTC_DateStruct; // ���ڴ洢������Ϣ
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct); // ��ȡ��ǰ��������Ϣ
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct); // ��ȡ��ǰ��ʱ����Ϣ

	u8 t = 0;
	u8 tbuf_clock[80]={0}; // ���ڴ洢��ʽ�����ʱ��������ַ���
	
	OLED_Clear();
	sprintf((char *)tbuf_clock, "20%02d-%02d-%02d %02d:%02d:%02d",RTC_DateStruct.RTC_Year, RTC_DateStruct.RTC_Month, RTC_DateStruct.RTC_Date, hour%24, minute%60, second%60);
	
	OLED_ShowString(34,0,"set-clock",9,6);
	OLED_ShowString(0,3,tbuf_clock,strlen((char*)tbuf_clock),6);
	delay_ms(100);
}

void Task1_OLED(void *arg){
	QueueStruct Rbuf={0};
	//��ʼ��OLED
	OLED_Config();
	
	int time=123;
	u8 tbuf[80]={0}; // ���ڴ洢��ʽ�����ʱ��������ַ���
	while(1)
	{
		xQueueReceive(xQueue1,&Rbuf,portMAX_DELAY);
		printf("QueueType%d,Data=%d\n",Rbuf.QueueType,Rbuf.Data);
		if(Rbuf.QueueType == 0){
			//�����յ��˱�������͹�������Ϣ �������ǰ��������͹�����
			//OLED_ShowNum(30,0,Rbuf.Data,6,8);
			if(Rbuf.Data == 1){
				OLED_Clear();
				vTaskSuspend(TaskRTCTCB);
				vTaskSuspend(TaskDHT11TCB);
				Oled_show_clock();
				
			}else if(Rbuf.Data == 2){
				OLED_Clear();
				OLED_ShowString(34,0,"temp-show",9,8);
				vTaskResume(TaskDHT11TCB);
				vTaskSuspend(TaskRTCTCB);
			}else if(Rbuf.Data == 3){
				OLED_Clear();
				OLED_ShowString(34,0,"time-show",9,6);
				vTaskResume(TaskRTCTCB);
				vTaskSuspend(TaskDHT11TCB);
			}
		}else if(Rbuf.QueueType == 1){
			vTaskSuspend(TaskRTCTCB);
			vTaskSuspend(TaskDHT11TCB);
			OLED_ShowString(34,0,"alarm-show",9,6);
			if(Rbuf.set_time==0){
				hour++;
				OLED_Clear();
				Oled_show_clock();
			}else if(Rbuf.set_time==1){
				minute++;
				OLED_Clear();
				Oled_show_clock();
			}else if(Rbuf.set_time==2){
				second++;
				OLED_Clear();
				Oled_show_clock();
			}
		}
	}
}


void Task2_KEY(void *arg){
	KEY_Config();//���ð�������
	
	int num=1;
	QueueStruct val;
	val.QueueType = 0;
	
	while(1)
	{
		if(PAin(0)==0)
		{
			val.QueueType = 0;
			delay_ms(20);
			if(PAin(0)==0)
			{
				num++;
				if(num > 3){
					num = 1;
				}
				val.Data = num;
				xQueueSend(xQueue1,&val, portMAX_DELAY);
				while(PAin(0) == 0);
			}
		}
		
		if(PEin(2)==0)
		{
			val.QueueType = 1;
			delay_ms(20);
			if(PEin(2)==0)
			{
				val.set_time = 0;
				xQueueSend(xQueue1,&val, portMAX_DELAY);
				while(PEin(2) == 0);
			}
		}
		
		if(PEin(3)==0)
		{
			val.QueueType = 1;
			delay_ms(20);
			if(PEin(3)==0)
			{
				val.set_time = 1;
				xQueueSend(xQueue1,&val, portMAX_DELAY);
				while(PEin(3) == 0);
			}
		}
		
		if(PEin(4)==0)
		{
			val.QueueType = 1;
			delay_ms(20);
			if(PEin(4)==0)
			{
				val.set_time = 2;
				xQueueSend(xQueue1,&val, portMAX_DELAY);
				while(PEin(4)==0);
			}
		}
		
		vTaskDelay(10);//��ʱ1000������ 1s
	}

}


void Oled_show_time(void *arg){
	OLED_Config();
	OLED_Clear();
	while(1){
		u8 tbuf[80]={0}; // ���ڴ洢��ʽ�����ʱ��������ַ���
		char* timeString = oled_time();
		//memcpy(tbuf,oled_time(),strlen(oled_time()));
		memcpy(tbuf,timeString,strlen(timeString));
		//OLED_ShowString(0,3,tbuf,strlen((char*)tbuf),6);
		OLED_ShowString(0,3,tbuf,strlen((char*)tbuf),6);
		delay_ms(1000);
	}
}

//void Oled_show_temp(){
//	u8 temph;
//	u8 humih;
//	u8 templ;
//	u8 humil;

//	while(1){
//		u8 tbuf[80]={0}; // ���ڴ洢��ʽ�����ʱ��������ַ���
//		DHT11_Read_Data(&temph,&humih,&templ,&humil);
//	
//		sprintf((char *)tbuf, "temp:%d,hum:%d", temph,humih);
//		OLED_ShowString(0,3,tbuf,strlen((char*)tbuf),6);
//		printf("%s\r\n",tbuf);
//		delay_ms(1000);
//	}
//}

void Oled_show_temp(){
	u8 temph;
	u8 humih;
	u8 templ;
	u8 humil;

	
	while(1){
		//u8 tbuf[80]={0};
		u8 tbuf_tem[40]={0}; // ���ڴ洢��ʽ�����ʱ��������ַ���
		u8 tbuf_hum[40]={0}; // ���ڴ洢��ʽ�����ʱ��������ַ���
		DHT11_Read_Data(&temph,&humih,&templ,&humil);
				
		float adc = ADC1->DR*(3.3/4096);
		
		float current_temp = (float)temph-7*adc;
		float current_hum = (float)humih-27.71;
		//sprintf((char *)tbuf, "temp:%d,hum:%d", temph,humih);
		sprintf((char *)tbuf_tem, "tem:%.2f", current_temp);
		sprintf((char *)tbuf_hum, "hum:%.2f", current_hum);
		//sprintf((char *)tbuf, "temp:%.2f,hum:%.2f", current_temp,current_hum);
		
		OLED_ShowString(30,3,tbuf_tem,strlen((char*)tbuf_tem),8);
		OLED_ShowString(30,5,tbuf_hum,strlen((char*)tbuf_hum),8);
		printf("temp=%.2f^c ,hum=%.2f^c\r\n",current_temp,current_hum);
		delay_ms(1000);
	}
}

void Beep_Clock(){
	BEEP_Config();
	RTC_TimeTypeDef RTC_TimeStruct; // ���ڴ洢RTC��ʱ��
	RTC_DateTypeDef RTC_DateStruct; // ���ڴ洢������Ϣ
	
	u8 tbuf[80]={0}; 
	u8 tbuf_clock[80]={0};
	
	while(1){
		RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct); // ��ȡ��ǰ��������Ϣ
		RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct); // ��ȡ��ǰ��ʱ����Ϣ
	
		// �����ʽ��Ϊ�ַ����洢��tbuf�У�Ȼ��ͨ��LCD��ʾ����
		sprintf((char *)tbuf, "20%02d-%02d-%02d %02d:%02d:%02d",RTC_DateStruct.RTC_Year, RTC_DateStruct.RTC_Month, RTC_DateStruct.RTC_Date, RTC_TimeStruct.RTC_Hours, RTC_TimeStruct.RTC_Minutes, RTC_TimeStruct.RTC_Seconds);
		sprintf((char *)tbuf_clock, "20%02d-%02d-%02d %02d:%02d:%02d",RTC_DateStruct.RTC_Year, RTC_DateStruct.RTC_Month, RTC_DateStruct.RTC_Date, hour%24, minute%60, second%60);
		
		//printf("tbuf_clock=%s\n",tbuf_clock);
		if(strcmp(tbuf,tbuf_clock)==0){
			PFout(8) = 1;
			delay_ms(500);
			PFout(8) = 0;
			delay_ms(500);
		}
		delay_ms(1000);
	}
}

void Turn_led(){
	ADC1_Config();
	ADC3_Config();
	while(1){
		if(ADC3->SR & 0x02){
			TIM14->CCR1 =1000 - (ADC3->DR-870)*1000/(3800-870);//���ݹ�������ʵ��ֵ��Χ�Լ��޸� ���ҵƹ������880+ ���Ƹ������3700+
			//printf("ADC3_IN5:%d\r\n",ADC3->DR);

			if(ADC3->DR>1200 & ADC3->DR<=2000){
				PFout(9) = 0;
				PFout(10) = 1;
			}else if(ADC3->DR>2000 & ADC3->DR<=2500){
				PFout(9) = 0;
				PFout(10) = 0;
			}else if( ADC3->DR>2500){
				PFout(9) = 0;
				PFout(10) = 0;
			}else{
				PFout(9) = 1;
				PFout(10) = 1;
			}
			delay_ms(500);
		}
	}
}



uint16_t uart1_recv_data;

//���ڽ����жϽ�������������������
void USART1_IRQHandler()
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        //����жϱ�־λ   ������д1   ��סһ��Ҫ���
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
        uart1_recv_data =  USART_ReceiveData(USART1);
    }
}

void BlueTooth_Ctrl(){
	while(1)
	{
		if(uart1_recv_data == '0')
        {
			PEout(13) = ~PEout(13);
			uart1_recv_data = 4;
        }
        if(uart1_recv_data == '1')
        {	
			PEout(13) =1;
        }
        if(uart1_recv_data == '5')
        {	
			hour++;
			uart1_recv_data = 4;
			OLED_Clear();
			Oled_show_clock();
        }
		if(uart1_recv_data == '6')
        {	
			minute++;
			uart1_recv_data = 4;
			OLED_Clear();
			Oled_show_clock();
        }
		if(uart1_recv_data == '7')
        {	
			second++;
			uart1_recv_data = 4;
			OLED_Clear();
			Oled_show_clock();
        }
		if(uart1_recv_data == '8')
        {	
			PFout(8) = ~PFout(8) ;
			uart1_recv_data = 4;
        }
		delay_ms(100);
	}
}


int main(){
	//�����ж����ȼ����� 4bit 04--��0  13  22   31  40--��4
	//��ռ���ȼ�0~15  ��Ӧ���ȼ�0~15
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//4bit  4bit��ռ���ȼ�  0bit��Ӧ���ȼ�
	LED_Config();
	UART1_Config(115200);//����1����
	ADC1_Config();
	printf("�˶��ֻ�\r\n");
	
    xQueue1 = xQueueCreate( 10, sizeof(QueueStruct) );
    if( xQueue1 == NULL )
    {
        printf("��������ʧ��\r\n");
    }
	
	
	//����FreeRTOS����---��ʼ����
	//           ������  ������    ջ��С ���������Ĵ���  ���ȼ�  ������TCB����ʱΪNULL��
	xTaskCreate( Task1_OLED, "TaskOLED", 128, NULL,               10,    &Task1OLEDTCB);
	xTaskCreate( Task2_KEY,  "TaskKEY",  128, NULL,               10,    &Task2KEYTCB);
	
	xTaskCreate( Oled_show_time,  "TaskRTC",  256, NULL,          10,    &TaskRTCTCB);
	xTaskCreate( Oled_show_temp,  "TaskDHT11",  128, NULL,        10,    &TaskDHT11TCB);
	xTaskCreate( Beep_Clock,      "TaskBeepCLOCK",  256, NULL,         10,    &TaskBeepCLOCKTCB);
	xTaskCreate( Turn_led,        "Turn_led",  128, NULL,         12,    &TaskLedTCB);
	xTaskCreate( BlueTooth_Ctrl,  "BlueTooth",  128, NULL,         12,    &TaskBlueToothTCB);
	
	
	vTaskSuspend(TaskRTCTCB);
	vTaskSuspend(TaskDHT11TCB);
	//vTaskSuspend(TaskCLOCK);
	//����RTOS---������������  ����SysTick.....
	vTaskStartScheduler();
	
	return 0;
}



