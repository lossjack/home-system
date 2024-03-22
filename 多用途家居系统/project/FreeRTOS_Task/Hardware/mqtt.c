#include "mqtt.h"

//mode: 1station   2AP   3sta+AP
int ESP8266_CWModeSet(char mode)
{
	char ATBuffer[512]={0};
	sprintf(ATBuffer,"AT+CWMODE_CUR=%d\r\n",mode);
	
	memset(ESP8266_RxBuff,0,sizeof(ESP8266_RxBuff));
	ESP8266_RxCount = 0;
	
	USART3_SendString(ATBuffer,strlen(ATBuffer));
	
	//�ȴ�һ��ʱ��---��ESP8266 ����һ��OK
	int wait=3000;
	while(wait--)
	{
		if(strstr(ESP8266_RxBuff,"OK") != NULL)
		{
			return 0;//��������ESP8266��ģʽ�ɹ�
		}
		delay_ms(1);
	}
	
	return -1;//����ʱ�� �Է���û����OK  ����ʧ��
}

int ESP8266_ATE(char mode)
{
	char ATBuffer[512]={0};
	sprintf(ATBuffer,"ATE%d\r\n",mode);
	
	memset(ESP8266_RxBuff,0,sizeof(ESP8266_RxBuff));
	ESP8266_RxCount = 0;
	
	USART3_SendString(ATBuffer,strlen(ATBuffer));
	
	//�ȴ�һ��ʱ��---��ESP8266 ����һ��OK
	int wait=3000;
	while(wait--)
	{
		if(strstr(ESP8266_RxBuff,"OK") != NULL)
		{
			return 0;//��������ESP8266��ģʽ�ɹ�
		}
		delay_ms(1);
	}
	
	return -1;//����ʱ�� �Է���û����OK  ����ʧ��
}


int ESP8266_ConnectWifi(char *SSID,char *Passwd)
{
	char ATBuffer[512]={0};
	sprintf(ATBuffer,"AT+CWJAP_CUR=\"%s\",\"%s\"\r\n",SSID,Passwd);
	
	//��մ���3 �Ļ�������ͼ���ֵ
	memset(ESP8266_RxBuff,0,sizeof(ESP8266_RxBuff));
	ESP8266_RxCount = 0;
	
	USART3_SendString(ATBuffer,strlen(ATBuffer));
	
	//�ȴ�һ��ʱ��---��ESP8266 ����һ��OK
	int wait=10000;
	while(wait--)
	{
		if(strstr(ESP8266_RxBuff,"OK") != NULL)
		{
			return 0;//��������ESP8266��ģʽ�ɹ�
		}
		delay_ms(1);
	}
	
	return -1;//����ʱ�� �Է���û����OK  ����ʧ��
}


int ESP8266_ConnectTCP(char *ip,unsigned short port)
{
	char ATBuffer[512]={0};
	sprintf(ATBuffer,"AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",ip,port);
	
	//��մ���3 �Ļ�������ͼ���ֵ
	memset(ESP8266_RxBuff,0,sizeof(ESP8266_RxBuff));
	ESP8266_RxCount = 0;
	
	USART3_SendString(ATBuffer,strlen(ATBuffer));
	
	//�ȴ�һ��ʱ��---��ESP8266 ����һ��OK
	int wait=10000;
	while(wait--)
	{
		if(strstr(ESP8266_RxBuff,"OK") != NULL)
		{
			return 0;//��������ESP8266��ģʽ�ɹ�
		}
		delay_ms(1);
	}
	
	return -1;//����ʱ�� �Է���û����OK  ����ʧ��
}

//mode 0:��ͨģʽ  1��͸��ģʽ
int ESP8266_SetCIPMODE(char mode)
{
	char ATBuffer[512]={0};
	sprintf(ATBuffer,"AT+CIPMODE=%d\r\n",mode);
	
	//��մ���3 �Ļ�������ͼ���ֵ
	memset(ESP8266_RxBuff,0,sizeof(ESP8266_RxBuff));
	ESP8266_RxCount = 0;
	
	USART3_SendString(ATBuffer,strlen(ATBuffer));
	
	//�ȴ�һ��ʱ��---��ESP8266 ����һ��OK
	int wait=3000;
	while(wait--)
	{
		if(strstr(ESP8266_RxBuff,"OK") != NULL)
		{
			return 0;//��������ESP8266��ģʽ�ɹ�
		}
		delay_ms(1);
	}
	
	return -1;//����ʱ�� �Է���û����OK  ����ʧ��
}

//͸��ģʽ������ֱ�����ݷ���
int ESP8266_SendData(void)
{
	char ATBuffer[512]={0};
	sprintf(ATBuffer,"AT+CIPSEND\r\n");
	
	//��մ���3 �Ļ�������ͼ���ֵ
	memset(ESP8266_RxBuff,0,sizeof(ESP8266_RxBuff));
	ESP8266_RxCount = 0;
	
	USART3_SendString(ATBuffer,strlen(ATBuffer));
	
	//�ȴ�һ��ʱ��---��ESP8266 ����һ��OK
	int wait=3000;
	while(wait--)
	{
		if(strstr(ESP8266_RxBuff,">") != NULL)
		{
			return 0;//��������ESP8266��ģʽ�ɹ�
		}
		delay_ms(1);
	}
	
	return -1;//����ʱ�� �Է���û����OK  ����ʧ��
}

int ESP8266_ExitSend(void)
{
	USART3_SendString((unsigned char *)"+++",3);
}


//MQTT���ӱ��ĵ����ݴ��
// ClientID �� "Client1"
int MQTT_Connect(char *ClientID,char *UserName,char *Passwd,unsigned short KeepAlive)
{
	unsigned char TxBuff[512]={0};
	int           TxCount    = 0;
	
	//�ٹ̶���ͷ
	TxBuff[TxCount++] = 0x10;// 0001 0000
	
	int data_len = 10+2+strlen(ClientID)+2+strlen(UserName)+2+strlen(Passwd);//ʣ�೤��
	printf("data_len:%d\r\n",data_len);
//	TxBuff[TxCount++] = data_len;
	int encodedByte = 0;
	//ʣ�೤��(�������̶�ͷ��)
    do
    {
        encodedByte = data_len % 128;// 176 %128 = 48  & 0x7F  0111 1111
        data_len = data_len / 128;//data_len == 1
        //if there are more data to encode, set the top bit of this byte
        if( data_len > 0 )
            encodedByte = encodedByte | 0x80;//encodedByte ==  1011 0000
        TxBuff[TxCount++] = encodedByte;//1011 0000   0000 0001
    } while( data_len > 0 );
	
	
	//�ڿɱ䱨ͷ---10Byte
		//Э�鳤��+Э����  6Byte
		TxBuff[TxCount++]= 0x00;
		TxBuff[TxCount++]= 0x04;
		TxBuff[TxCount++]= 'M';
		TxBuff[TxCount++]= 'Q';
		TxBuff[TxCount++]= 'T';
		TxBuff[TxCount++]= 'T';
		//Э�鼶��---Э��汾 3.1.1    4
		TxBuff[TxCount++]= 0x04;
		//���ӱ�־
		TxBuff[TxCount++]= 0xC2;//1100 0010  ֻ�����û����������־  ������Ự��־λ  ����ûҪ
		
		//��������ʱ������--Keep Alive 2bYTE
		TxBuff[TxCount++]= KeepAlive>>8 & 0xFF;//keep alive ��8bit   
		TxBuff[TxCount++]= KeepAlive & 0xFF;//keep alive ��8bit   0000 0001 0000 0100
		
		
	//����Ч�غ�
		//�ͻ��˱�ʶ��---ClientID
		TxBuff[TxCount++]= strlen(ClientID)>>8 & 0xFF;
		TxBuff[TxCount++]= strlen(ClientID) & 0xFF;
		
		memcpy(&TxBuff[TxCount],ClientID,strlen(ClientID));
		TxCount += strlen(ClientID);//���±����ֵTxCount�ӵ��µĿ�λ
		
		//�û�������2Byte+�����û�����������NByte
		TxBuff[TxCount++]= strlen(UserName)>>8 & 0xFF;
		TxBuff[TxCount++]= strlen(UserName) & 0xFF;
		memcpy(&TxBuff[TxCount],UserName,strlen(UserName));
		TxCount += strlen(UserName);//���±����ֵTxCount�ӵ��µĿ�λ
		
		//���볤��2Byte+������������ NByte
		TxBuff[TxCount++]= strlen(Passwd)>>8 & 0xFF;
		TxBuff[TxCount++]= strlen(Passwd) & 0xFF;
		memcpy(&TxBuff[TxCount],Passwd,strlen(Passwd));
		TxCount += strlen(Passwd);//���±����ֵTxCount�ӵ��µĿ�λ
		
		
		//���͸�MQTT������
		//��մ���3 �Ļ�������ͼ���ֵ
		memset(ESP8266_RxBuff,0,sizeof(ESP8266_RxBuff));
		ESP8266_RxCount = 0;
		
		//���͵�����3----����͸��ģʽ�µ�ESP8266--->MQTT������
		USART3_SendString((char *)TxBuff,TxCount);
		
		int wait=5000;
		while(wait--)
		{
			//����Ҫ�Ż�
			if(ESP8266_RxCount == 4)
			{
				if(ESP8266_RxBuff[0]>>4 == 2)
				{
					//��������ȷ��������ȷ��   ������--->�ͻ���
					if(ESP8266_RxBuff[3] == 0)//����ȷ����ֵ��0  �������ӱ�����
					{
						//���ӳɹ�
						return 0;//������������MQTT�������ɹ�
					}
					else
					{
						return -1;
					}
				}
				
			}
			delay_ms(1);
		}
	
		return -1;//����ʱ�� �Է���û����OK  ����ʧ��
		
}




// ��������
int MQTT_Publish(char *ClientID,char *UserName,char *Passwd,unsigned short KeepAlive)
{
	unsigned char TxBuff[512]={0};
	int           TxCount    = 0;
	
	//�ٹ̶���ͷ
	TxBuff[TxCount++] = 0x30;// 0011 0 00 0
	
	int data_len = 10+2+strlen(ClientID)+2+strlen(UserName)+2+strlen(Passwd);//ʣ�೤��
	printf("data_len:%d\r\n",data_len);

	
	int encodedByte = 0;
	//ʣ�೤��(�������̶�ͷ��)
    do
    {
        encodedByte = data_len % 128;// 176 %128 = 48  & 0x7F  0111 1111
        data_len = data_len / 128;//data_len == 1
        //if there are more data to encode, set the top bit of this byte
        if( data_len > 0 )
            encodedByte = encodedByte | 0x80;//encodedByte ==  1011 0000
        TxBuff[TxCount++] = encodedByte;//1011 0000   0000 0001
    } while( data_len > 0 );
	
	
	//�ڿɱ䱨ͷ---10Byte
		//���ⳤ��
		TxBuff[TxCount++]= 0x00;
		TxBuff[TxCount++]= 0x04;
		
		//��������������  xxxx/xxx/xxxx
		
	
		//���ı�ʶ�� Qos1  Qos2����Ҫ���ı�ʶ��
//		TxBuff[TxCount++]= KeepAlive>>8 & 0xFF;//keep alive ��8bit   
//		TxBuff[TxCount++]= KeepAlive & 0xFF;//keep alive ��8bit   0000 0001 0000 0100
		
		
	//����Ч�غ�
		//����Ҫ���͵���Ϣ
	
	
	
		
	//���͸�MQTT������
	//��մ���3 �Ļ�������ͼ���ֵ
	memset(ESP8266_RxBuff,0,sizeof(ESP8266_RxBuff));
	ESP8266_RxCount = 0;
	
	//���͵�����3----����͸��ģʽ�µ�ESP8266--->MQTT������
	USART3_SendString(TxBuff,TxCount);
	
	int wait=5000;
	while(wait--)
	{
		//����Ҫ�Ż�
		if(ESP8266_RxCount == 4)
		{
			if(ESP8266_RxBuff[0]>>4 == 2)
			{
				//��������ȷ��������ȷ��   ������--->�ͻ���
				if(ESP8266_RxBuff[3] == 0)//����ȷ����ֵ��0  �������ӱ�����
				{
					//���ӳɹ�
					return 0;//������������MQTT�������ɹ�
				}
				else
				{
					return -1;
				}
			}
			
		}
		delay_ms(1);
	}

	return -1;//����ʱ�� �Է���û����OK  ����ʧ��
		
}



// �������ⱨ��
/*
Topic:Ҫ���ĵ�����  ֧�ֵ���+�Ͷ��#ͨ���
Qos:�ͻ��˶��ĵ����⣬���������Ϣ������������
flags:�ͻ��˱�ʶ��  ��������  ������������о�
*/
int MQTT_Subscribe(char *Topic,unsigned char Qos,unsigned short flags)
{
	unsigned char TxBuff[512]={0};
	int           TxCount    = 0;
	
	//�ٹ̶���ͷ
	TxBuff[TxCount++] = 0x82;// 1000 0010
	
	int data_len = 2+2+strlen(Topic)+1;//ʣ�೤��
	printf("data_len:%d\r\n",data_len);

	int encodedByte = 0;
	//ʣ�೤��(�������̶�ͷ��)
    do
    {
        encodedByte = data_len % 128;// 176 %128 = 48  & 0x7F  0111 1111
        data_len = data_len / 128;//data_len == 1
        //if there are more data to encode, set the top bit of this byte
        if( data_len > 0 )
            encodedByte = encodedByte | 0x80;//encodedByte ==  1011 0000
        TxBuff[TxCount++] = encodedByte;//1011 0000   0000 0001
    } while( data_len > 0 );
	
	
	//�ڿɱ䱨ͷ---2Byte
		//���ı�ʶ��
		TxBuff[TxCount++]= flags>>8 & 0xFF;
		TxBuff[TxCount++]= flags & 0xFF;
		
		
		
	//����Ч�غ�
		//����ĳ���
		TxBuff[TxCount++]= strlen(Topic)>>8 & 0xFF;
		TxBuff[TxCount++]= strlen(Topic) & 0xFF;
		
		//�����������
		memcpy(&TxBuff[TxCount],Topic,strlen(Topic));
		TxCount+=strlen(Topic);
		
		//�ͻ�������������ķ���������Ϣ
		TxBuff[TxCount++] = Qos;
	
	
	
		
	//���͸�MQTT������
	//��մ���3 �Ļ�������ͼ���ֵ
	memset(ESP8266_RxBuff,0,sizeof(ESP8266_RxBuff));
	ESP8266_RxCount = 0;
	
	//���͵�����3----����͸��ģʽ�µ�ESP8266--->MQTT������
	USART3_SendString((char *)TxBuff,TxCount);
	
	int wait=5000;
	while(wait--)
	{
		//����Ҫ�Ż�
		if(ESP8266_RxCount == 5)
		{
			if(ESP8266_RxBuff[0]>>4 == 9) 
			{
				//��������ȷ���Ƕ���ȷ��   ������--->�ͻ���
				if(ESP8266_RxBuff[4] == 0x00)//����ȷ����ֵ��0  �������ӱ�����
				{
					//���ӳɹ�
					printf("���ĳɹ������Qos0\r\n");
					return 0;//������������MQTT�������ɹ�
				}
				else if(ESP8266_RxBuff[4] == 0x01)//����ȷ����ֵ��0  �������ӱ�����
				{
					//���ӳɹ�
					printf("���ĳɹ������Qos1\r\n");
					return 0;//������������MQTT�������ɹ�
				}
				else if(ESP8266_RxBuff[4] == 0x02)//����ȷ����ֵ��0  �������ӱ�����
				{
					//���ӳɹ�
					printf("���ĳɹ������Qos2\r\n");
					return 0;//������������MQTT�������ɹ�
				}
				else if(ESP8266_RxBuff[4] == 0x80)//����ȷ����ֵ��0  �������ӱ�����
				{
					//���ӳɹ�
					printf("����ʧ�ܣ�\r\n");
					return -1;//������������MQTT�������ɹ�
				}
				else
				{
					return -2;//�������
				}
			}
			
		}
		delay_ms(1);
	}

	return -3;//����ʱ�� �Է���û����OK  ����ʧ��
		
}


//��������
int MQTT_PINGREQ(void)
{
	unsigned char TxBuff[2]={0xC0,0x00};
	
	//���͸�MQTT������
	//��մ���3 �Ļ�������ͼ���ֵ
	memset(ESP8266_RxBuff,0,sizeof(ESP8266_RxBuff));
	ESP8266_RxCount = 0;
	
	//���͵�����3----����͸��ģʽ�µ�ESP8266--->MQTT������
	USART3_SendString(TxBuff,2);
	
	int wait=3000;
	while(wait--)
	{
		//����Ҫ�Ż�
		
		if(ESP8266_RxBuff[0] == 0xD0 && ESP8266_RxBuff[1]==0x00)
		{
			//
			return 0;//�����������������Ӧ
		}
		delay_ms(1);
	
	}
	return -1;//��ʾ�޻�Ӧ

}

