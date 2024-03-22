#include "ymodem.h"

//crc16校验计算函数
unsigned short crc16(const unsigned char *addr, unsigned int num)
{
	uint16_t crc = 0;
    for( ; num > 0; num--)
    {
    	crc = crc ^ (*addr++ << 8);
    	for(int i=0; i < 8; i++)
    	{
    		if(crc & 0x8000)
    			crc = (crc << 1) ^ 0x1021;
    		else
    			crc <<= 1;
    	}
    	crc &= 0xFFFF;
    }
    return crc;
}

//M^N
int MxN(int m,int n)
{
	int num = 1;
	while(n--)
	{
		num*=m;
	}
	return num;
}

unsigned int StrToNum(char *str)
{
	int len = strlen(str);
	int num = 0;
	for(int i=len-1;i>=0;i--)
	{
		num+=(*str-'0')*MxN(10,i);
		str++;//!!!!!
	}
	return num;
}



/*
使用串口1接收，故需要不断判断是否收到一包数据-可自行替换为其它串口
*/
int Ymodem_Update(void)
{
	unsigned char recive_filename[64]={0};
	unsigned char recive_filesize[32]={0};
	int cur_packet=0;//记录当前包序列号
	int other_recv_len = 0;//记录剩余需要接收字节数
	unsigned int Flash_addr = 0x08010000;
	//校验和方式 / CRC16
	char nak = NAK;
	char ack = ACK;
	char can = CAN;
	char crc16_key = CRC16;//'C'
	
	
	//接收者先发送启动信息'C'，等待发送者回应
	unsigned int i=0;
	while(1)
	{
		//每隔约3s等待发送者发送第一帧数据
		if(i%3000 == 0)
		{
			//printf("等待对方开始发送...\r\n");
			OLED_ShowString(0,0,"wait...",7,8);
			//开始前清空串口1接收数据全局数组和记录下标
			memset(RxBuff,0,sizeof(RxBuff));
			RxCount = 0;
			
			SendTo(&crc16_key,1);
		}	
		delay_ms(1);
		i++;
		if((RxBuff[0] == SOH) && RxBuff[1] == cur_packet && (RxCount == 133))
		{
			//先校验
			if(crc16(&RxBuff[3],128) == (RxBuff[131]<<8 | RxBuff[132]))
			{
				//校验通过
				//收到了发送者的回应
				strcpy(recive_filename,&RxBuff[3]);//储存文件名
				*strchr(RxBuff+strlen(&RxBuff[3])+4,' ') = '\0';
				strcpy(recive_filesize,RxBuff+strlen(&RxBuff[3])+4);//储存文件大小字符串
				
				printf("第一帧内容:");
				for(int i=0;i<133;i++)
				{
					printf("0x%x ",RxBuff[i]);
				}
				printf("\r\n");
				
				
				OLED_ShowString(0,0,"name:",5,8);
				OLED_ShowString(40,0,recive_filename,strlen(recive_filename),8);
				OLED_ShowString(0,2,"size:",5,8);
				OLED_ShowString(40,2,recive_filesize,strlen(recive_filesize),8);
				
				
				printf("文件大小:%s\r\n",recive_filesize);
				
				//根据文件大小计算需要接收多少次
				other_recv_len = StrToNum(recive_filesize);
				printf("文件大小:%d\r\n",other_recv_len);
				//other_recv_len = file_len;//更新剩余需要接收字节数

				//OLED_ShowString(40,2,recive_filesize,strlen(recive_filesize),8);
				
				//清空串口1接收中断全局数组和计数下表
				memset(RxBuff,0,sizeof(RxBuff));
				RxCount = 0;
				
				cur_packet++;
				SendTo(&ack,1);//发送ACK信号回去给发送者
				SendTo(&crc16_key,1);//发送C回去给发送者
				break;//跳出循环
			}
			else
			{
				//printf("校验失败,给发送者发送无应答\r\n");
				memset(RxBuff,0,sizeof(RxBuff));
				RxCount = 0;
				i=1;
				SendTo(&nak,1);//发送NAK信号回去给发送者
			}
			
		}
	}
	
	
	//Flash扇区擦除一次即可  只擦除扇区4---APP
	//STMFLASH_EraseSector(4);
	
	int EOT_num=0;
	while(1)
	{
		if(RxBuff[0] == EOT && other_recv_len==0)
		{
			memset(RxBuff,0,sizeof(RxBuff));
			RxCount = 0;
			if(EOT_num == 0)//第一次收到EOT
			{
				printf("收到了第一个停止传输信号\r\n");
				EOT_num++;
				SendTo(&nak,1);//发送NAK信号回去给发送者
			}
			else //第二次收到EOT
			{
				printf("收到了第二个停止传输信号\r\n");
				SendTo(&ack,1);//发送ACK信号回去给发送者
				SendTo(&crc16_key,1);//发送C信号回去给发送者
			}
		}
		
		if(RxBuff[0] == STX && RxBuff[1] == cur_packet && RxCount==1029)
		{
			//收到1024文件内容 同时根据上面获取的总文件大小来计算这个过程需要多少次
			//先校验
			if(crc16(&RxBuff[3],1024) == (RxBuff[1027]<<8 | RxBuff[1028]))
			{
				//校验成功
				if(other_recv_len<1024)
				{
					//将other_recv_len长度数据写入flash
					STMFLASH_Write(Flash_addr,&RxBuff[3],other_recv_len,cur_packet);
					
					other_recv_len=0;//代表收完
					printf("成功收到第%d包数据,剩余包大小是:%d\r\n",cur_packet,other_recv_len);
					
					memset(RxBuff,0,sizeof(RxBuff));
					RxCount = 0;
					
				}
				else
				{
					//将1024字节写入flash
					if(0 != STMFLASH_Write(Flash_addr,&RxBuff[3],1024,cur_packet))
					{
						printf("Flash写失败\r\n");
					}
					other_recv_len -= 1024;//剩余长度做减法
					printf("成功收到第%d包数据,剩余包大小是:%d\r\n",cur_packet,other_recv_len);
					Flash_addr+=1024;//Flash写入地址偏移
					printf("Flash_addr:0x%x  本包第一个字是：0x%x\r\n",Flash_addr,*((unsigned int *)&RxBuff[3]));
				}
				
				memset(RxBuff,0,sizeof(RxBuff));
				RxCount = 0;
				cur_packet++;
				SendTo(&ack,1);//发送ACK信号回去给发送者
			}
			else //校验失败
			{
				memset(RxBuff,0,sizeof(RxBuff));
				RxCount = 0;
				SendTo(&nak,1);//发送ACK信号回去给发送者
			}
		}
		
		else if(RxBuff[0] == SOH && RxCount==133)
		{
			if(RxBuff[1] == cur_packet)
			{
				//收到不足<=128字节文件内容数据帧
				if(crc16(&RxBuff[3],128) == (RxBuff[131]<<8 | RxBuff[132]))
				{
					//校验成功
					//将other_recv_len长度数据写入flash
					STMFLASH_Write(Flash_addr,&RxBuff[3],other_recv_len,cur_packet);
					
					other_recv_len=0;//代表收完
					printf("成功收到第%d包数据,剩余包大小是:%d\r\n",cur_packet,other_recv_len);
					memset(RxBuff,0,sizeof(RxBuff));
					RxCount = 0;	

					cur_packet++;
					SendTo(&ack,1);//发送ACK信号回去给发送者
					delay_s(1);
					SendTo(&can,1);//发送NAK信号回去给从机
					break;//结束传输
				}
				else //校验失败
				{
					memset(RxBuff,0,sizeof(RxBuff));
					RxCount = 0;
					SendTo(&nak,1);//发送NAK信号回去给发送者
				}
			}
			else if(RxBuff[1] == 0x00)
			{
				//收到了停止帧，代表再发送者再无文件数据发送了
				printf("对方发送了停止帧\r\n");
				SendTo(&ack,1);//发送NAK信号回去给从机
				delay_s(1);
				SendTo(&can,1);//发送NAK信号回去给从机
				break;//结束传输
			}
		
		}

	}
	
	return 0;//成功更新完成
}










