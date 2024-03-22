#include "mqtt.h"

//mode: 1station   2AP   3sta+AP
int ESP8266_CWModeSet(char mode)
{
	char ATBuffer[512]={0};
	sprintf(ATBuffer,"AT+CWMODE_CUR=%d\r\n",mode);
	
	memset(ESP8266_RxBuff,0,sizeof(ESP8266_RxBuff));
	ESP8266_RxCount = 0;
	
	USART3_SendString(ATBuffer,strlen(ATBuffer));
	
	//等待一定时间---等ESP8266 返回一个OK
	int wait=3000;
	while(wait--)
	{
		if(strstr(ESP8266_RxBuff,"OK") != NULL)
		{
			return 0;//代表设置ESP8266的模式成功
		}
		delay_ms(1);
	}
	
	return -1;//代表超时了 对方都没返回OK  设置失败
}

int ESP8266_ATE(char mode)
{
	char ATBuffer[512]={0};
	sprintf(ATBuffer,"ATE%d\r\n",mode);
	
	memset(ESP8266_RxBuff,0,sizeof(ESP8266_RxBuff));
	ESP8266_RxCount = 0;
	
	USART3_SendString(ATBuffer,strlen(ATBuffer));
	
	//等待一定时间---等ESP8266 返回一个OK
	int wait=3000;
	while(wait--)
	{
		if(strstr(ESP8266_RxBuff,"OK") != NULL)
		{
			return 0;//代表设置ESP8266的模式成功
		}
		delay_ms(1);
	}
	
	return -1;//代表超时了 对方都没返回OK  设置失败
}


int ESP8266_ConnectWifi(char *SSID,char *Passwd)
{
	char ATBuffer[512]={0};
	sprintf(ATBuffer,"AT+CWJAP_CUR=\"%s\",\"%s\"\r\n",SSID,Passwd);
	
	//清空串口3 的缓冲数组和计数值
	memset(ESP8266_RxBuff,0,sizeof(ESP8266_RxBuff));
	ESP8266_RxCount = 0;
	
	USART3_SendString(ATBuffer,strlen(ATBuffer));
	
	//等待一定时间---等ESP8266 返回一个OK
	int wait=10000;
	while(wait--)
	{
		if(strstr(ESP8266_RxBuff,"OK") != NULL)
		{
			return 0;//代表设置ESP8266的模式成功
		}
		delay_ms(1);
	}
	
	return -1;//代表超时了 对方都没返回OK  设置失败
}


int ESP8266_ConnectTCP(char *ip,unsigned short port)
{
	char ATBuffer[512]={0};
	sprintf(ATBuffer,"AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",ip,port);
	
	//清空串口3 的缓冲数组和计数值
	memset(ESP8266_RxBuff,0,sizeof(ESP8266_RxBuff));
	ESP8266_RxCount = 0;
	
	USART3_SendString(ATBuffer,strlen(ATBuffer));
	
	//等待一定时间---等ESP8266 返回一个OK
	int wait=10000;
	while(wait--)
	{
		if(strstr(ESP8266_RxBuff,"OK") != NULL)
		{
			return 0;//代表设置ESP8266的模式成功
		}
		delay_ms(1);
	}
	
	return -1;//代表超时了 对方都没返回OK  设置失败
}

//mode 0:普通模式  1：透传模式
int ESP8266_SetCIPMODE(char mode)
{
	char ATBuffer[512]={0};
	sprintf(ATBuffer,"AT+CIPMODE=%d\r\n",mode);
	
	//清空串口3 的缓冲数组和计数值
	memset(ESP8266_RxBuff,0,sizeof(ESP8266_RxBuff));
	ESP8266_RxCount = 0;
	
	USART3_SendString(ATBuffer,strlen(ATBuffer));
	
	//等待一定时间---等ESP8266 返回一个OK
	int wait=3000;
	while(wait--)
	{
		if(strstr(ESP8266_RxBuff,"OK") != NULL)
		{
			return 0;//代表设置ESP8266的模式成功
		}
		delay_ms(1);
	}
	
	return -1;//代表超时了 对方都没返回OK  设置失败
}

//透传模式下设置直接数据发送
int ESP8266_SendData(void)
{
	char ATBuffer[512]={0};
	sprintf(ATBuffer,"AT+CIPSEND\r\n");
	
	//清空串口3 的缓冲数组和计数值
	memset(ESP8266_RxBuff,0,sizeof(ESP8266_RxBuff));
	ESP8266_RxCount = 0;
	
	USART3_SendString(ATBuffer,strlen(ATBuffer));
	
	//等待一定时间---等ESP8266 返回一个OK
	int wait=3000;
	while(wait--)
	{
		if(strstr(ESP8266_RxBuff,">") != NULL)
		{
			return 0;//代表设置ESP8266的模式成功
		}
		delay_ms(1);
	}
	
	return -1;//代表超时了 对方都没返回OK  设置失败
}

int ESP8266_ExitSend(void)
{
	USART3_SendString((unsigned char *)"+++",3);
}


//MQTT连接报文的数据打包
// ClientID ： "Client1"
int MQTT_Connect(char *ClientID,char *UserName,char *Passwd,unsigned short KeepAlive)
{
	unsigned char TxBuff[512]={0};
	int           TxCount    = 0;
	
	//①固定报头
	TxBuff[TxCount++] = 0x10;// 0001 0000
	
	int data_len = 10+2+strlen(ClientID)+2+strlen(UserName)+2+strlen(Passwd);//剩余长度
	printf("data_len:%d\r\n",data_len);
//	TxBuff[TxCount++] = data_len;
	int encodedByte = 0;
	//剩余长度(不包括固定头部)
    do
    {
        encodedByte = data_len % 128;// 176 %128 = 48  & 0x7F  0111 1111
        data_len = data_len / 128;//data_len == 1
        //if there are more data to encode, set the top bit of this byte
        if( data_len > 0 )
            encodedByte = encodedByte | 0x80;//encodedByte ==  1011 0000
        TxBuff[TxCount++] = encodedByte;//1011 0000   0000 0001
    } while( data_len > 0 );
	
	
	//②可变报头---10Byte
		//协议长度+协议名  6Byte
		TxBuff[TxCount++]= 0x00;
		TxBuff[TxCount++]= 0x04;
		TxBuff[TxCount++]= 'M';
		TxBuff[TxCount++]= 'Q';
		TxBuff[TxCount++]= 'T';
		TxBuff[TxCount++]= 'T';
		//协议级别---协议版本 3.1.1    4
		TxBuff[TxCount++]= 0x04;
		//连接标志
		TxBuff[TxCount++]= 0xC2;//1100 0010  只开了用户名和密码标志  及清理会话标志位  遗嘱没要
		
		//保持连接时长设置--Keep Alive 2bYTE
		TxBuff[TxCount++]= KeepAlive>>8 & 0xFF;//keep alive 高8bit   
		TxBuff[TxCount++]= KeepAlive & 0xFF;//keep alive 低8bit   0000 0001 0000 0100
		
		
	//③有效载荷
		//客户端标识符---ClientID
		TxBuff[TxCount++]= strlen(ClientID)>>8 & 0xFF;
		TxBuff[TxCount++]= strlen(ClientID) & 0xFF;
		
		memcpy(&TxBuff[TxCount],ClientID,strlen(ClientID));
		TxCount += strlen(ClientID);//让下标计数值TxCount加到新的空位
		
		//用户名长度2Byte+具体用户名具体内容NByte
		TxBuff[TxCount++]= strlen(UserName)>>8 & 0xFF;
		TxBuff[TxCount++]= strlen(UserName) & 0xFF;
		memcpy(&TxBuff[TxCount],UserName,strlen(UserName));
		TxCount += strlen(UserName);//让下标计数值TxCount加到新的空位
		
		//密码长度2Byte+具体密码内容 NByte
		TxBuff[TxCount++]= strlen(Passwd)>>8 & 0xFF;
		TxBuff[TxCount++]= strlen(Passwd) & 0xFF;
		memcpy(&TxBuff[TxCount],Passwd,strlen(Passwd));
		TxCount += strlen(Passwd);//让下标计数值TxCount加到新的空位
		
		
		//发送给MQTT服务器
		//清空串口3 的缓冲数组和计数值
		memset(ESP8266_RxBuff,0,sizeof(ESP8266_RxBuff));
		ESP8266_RxCount = 0;
		
		//发送到串口3----发给透传模式下的ESP8266--->MQTT服务器
		USART3_SendString((char *)TxBuff,TxCount);
		
		int wait=5000;
		while(wait--)
		{
			//这里要优化
			if(ESP8266_RxCount == 4)
			{
				if(ESP8266_RxBuff[0]>>4 == 2)
				{
					//报文类型确定是连接确认   服务器--->客户端
					if(ESP8266_RxBuff[3] == 0)//连接确认码值是0  代码连接被接受
					{
						//连接成功
						return 0;//代表设置连接MQTT服务器成功
					}
					else
					{
						return -1;
					}
				}
				
			}
			delay_ms(1);
		}
	
		return -1;//代表超时了 对方都没返回OK  设置失败
		
}




// 发布报文
int MQTT_Publish(char *ClientID,char *UserName,char *Passwd,unsigned short KeepAlive)
{
	unsigned char TxBuff[512]={0};
	int           TxCount    = 0;
	
	//①固定报头
	TxBuff[TxCount++] = 0x30;// 0011 0 00 0
	
	int data_len = 10+2+strlen(ClientID)+2+strlen(UserName)+2+strlen(Passwd);//剩余长度
	printf("data_len:%d\r\n",data_len);

	
	int encodedByte = 0;
	//剩余长度(不包括固定头部)
    do
    {
        encodedByte = data_len % 128;// 176 %128 = 48  & 0x7F  0111 1111
        data_len = data_len / 128;//data_len == 1
        //if there are more data to encode, set the top bit of this byte
        if( data_len > 0 )
            encodedByte = encodedByte | 0x80;//encodedByte ==  1011 0000
        TxBuff[TxCount++] = encodedByte;//1011 0000   0000 0001
    } while( data_len > 0 );
	
	
	//②可变报头---10Byte
		//主题长度
		TxBuff[TxCount++]= 0x00;
		TxBuff[TxCount++]= 0x04;
		
		//主题名具体内容  xxxx/xxx/xxxx
		
	
		//报文标识符 Qos1  Qos2才需要报文标识符
//		TxBuff[TxCount++]= KeepAlive>>8 & 0xFF;//keep alive 高8bit   
//		TxBuff[TxCount++]= KeepAlive & 0xFF;//keep alive 低8bit   0000 0001 0000 0100
		
		
	//③有效载荷
		//具体要发送的消息
	
	
	
		
	//发送给MQTT服务器
	//清空串口3 的缓冲数组和计数值
	memset(ESP8266_RxBuff,0,sizeof(ESP8266_RxBuff));
	ESP8266_RxCount = 0;
	
	//发送到串口3----发给透传模式下的ESP8266--->MQTT服务器
	USART3_SendString(TxBuff,TxCount);
	
	int wait=5000;
	while(wait--)
	{
		//这里要优化
		if(ESP8266_RxCount == 4)
		{
			if(ESP8266_RxBuff[0]>>4 == 2)
			{
				//报文类型确定是连接确认   服务器--->客户端
				if(ESP8266_RxBuff[3] == 0)//连接确认码值是0  代码连接被接受
				{
					//连接成功
					return 0;//代表设置连接MQTT服务器成功
				}
				else
				{
					return -1;
				}
			}
			
		}
		delay_ms(1);
	}

	return -1;//代表超时了 对方都没返回OK  设置失败
		
}



// 订阅主题报文
/*
Topic:要订阅的主题  支持单层+和多层#通配符
Qos:客户端订阅的主题，允许接收消息的最大服务质量
flags:客户端标识符  可以随意  用熟后再认真研究
*/
int MQTT_Subscribe(char *Topic,unsigned char Qos,unsigned short flags)
{
	unsigned char TxBuff[512]={0};
	int           TxCount    = 0;
	
	//①固定报头
	TxBuff[TxCount++] = 0x82;// 1000 0010
	
	int data_len = 2+2+strlen(Topic)+1;//剩余长度
	printf("data_len:%d\r\n",data_len);

	int encodedByte = 0;
	//剩余长度(不包括固定头部)
    do
    {
        encodedByte = data_len % 128;// 176 %128 = 48  & 0x7F  0111 1111
        data_len = data_len / 128;//data_len == 1
        //if there are more data to encode, set the top bit of this byte
        if( data_len > 0 )
            encodedByte = encodedByte | 0x80;//encodedByte ==  1011 0000
        TxBuff[TxCount++] = encodedByte;//1011 0000   0000 0001
    } while( data_len > 0 );
	
	
	//②可变报头---2Byte
		//报文标识符
		TxBuff[TxCount++]= flags>>8 & 0xFF;
		TxBuff[TxCount++]= flags & 0xFF;
		
		
		
	//③有效载荷
		//主题的长度
		TxBuff[TxCount++]= strlen(Topic)>>8 & 0xFF;
		TxBuff[TxCount++]= strlen(Topic) & 0xFF;
		
		//主题具体内容
		memcpy(&TxBuff[TxCount],Topic,strlen(Topic));
		TxCount+=strlen(Topic);
		
		//客户端允许接收最大的服务质量消息
		TxBuff[TxCount++] = Qos;
	
	
	
		
	//发送给MQTT服务器
	//清空串口3 的缓冲数组和计数值
	memset(ESP8266_RxBuff,0,sizeof(ESP8266_RxBuff));
	ESP8266_RxCount = 0;
	
	//发送到串口3----发给透传模式下的ESP8266--->MQTT服务器
	USART3_SendString((char *)TxBuff,TxCount);
	
	int wait=5000;
	while(wait--)
	{
		//这里要优化
		if(ESP8266_RxCount == 5)
		{
			if(ESP8266_RxBuff[0]>>4 == 9) 
			{
				//报文类型确定是订阅确认   服务器--->客户端
				if(ESP8266_RxBuff[4] == 0x00)//连接确认码值是0  代码连接被接受
				{
					//连接成功
					printf("订阅成功，最大Qos0\r\n");
					return 0;//代表设置连接MQTT服务器成功
				}
				else if(ESP8266_RxBuff[4] == 0x01)//连接确认码值是0  代码连接被接受
				{
					//连接成功
					printf("订阅成功，最大Qos1\r\n");
					return 0;//代表设置连接MQTT服务器成功
				}
				else if(ESP8266_RxBuff[4] == 0x02)//连接确认码值是0  代码连接被接受
				{
					//连接成功
					printf("订阅成功，最大Qos2\r\n");
					return 0;//代表设置连接MQTT服务器成功
				}
				else if(ESP8266_RxBuff[4] == 0x80)//连接确认码值是0  代码连接被接受
				{
					//连接成功
					printf("订阅失败！\r\n");
					return -1;//代表设置连接MQTT服务器成功
				}
				else
				{
					return -2;//特殊情况
				}
			}
			
		}
		delay_ms(1);
	}

	return -3;//代表超时了 对方都没返回OK  设置失败
		
}


//心跳报文
int MQTT_PINGREQ(void)
{
	unsigned char TxBuff[2]={0xC0,0x00};
	
	//发送给MQTT服务器
	//清空串口3 的缓冲数组和计数值
	memset(ESP8266_RxBuff,0,sizeof(ESP8266_RxBuff));
	ESP8266_RxCount = 0;
	
	//发送到串口3----发给透传模式下的ESP8266--->MQTT服务器
	USART3_SendString(TxBuff,2);
	
	int wait=3000;
	while(wait--)
	{
		//这里要优化
		
		if(ESP8266_RxBuff[0] == 0xD0 && ESP8266_RxBuff[1]==0x00)
		{
			//
			return 0;//代表服务器有心跳回应
		}
		delay_ms(1);
	
	}
	return -1;//表示无回应

}

