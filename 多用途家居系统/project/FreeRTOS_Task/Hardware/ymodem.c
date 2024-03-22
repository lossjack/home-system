#include "ymodem.h"

//crc16У����㺯��
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
ʹ�ô���1���գ�����Ҫ�����ж��Ƿ��յ�һ������-�������滻Ϊ��������
*/
int Ymodem_Update(void)
{
	unsigned char recive_filename[64]={0};
	unsigned char recive_filesize[32]={0};
	int cur_packet=0;//��¼��ǰ�����к�
	int other_recv_len = 0;//��¼ʣ����Ҫ�����ֽ���
	unsigned int Flash_addr = 0x08010000;
	//У��ͷ�ʽ / CRC16
	char nak = NAK;
	char ack = ACK;
	char can = CAN;
	char crc16_key = CRC16;//'C'
	
	
	//�������ȷ���������Ϣ'C'���ȴ������߻�Ӧ
	unsigned int i=0;
	while(1)
	{
		//ÿ��Լ3s�ȴ������߷��͵�һ֡����
		if(i%3000 == 0)
		{
			//printf("�ȴ��Է���ʼ����...\r\n");
			OLED_ShowString(0,0,"wait...",7,8);
			//��ʼǰ��մ���1��������ȫ������ͼ�¼�±�
			memset(RxBuff,0,sizeof(RxBuff));
			RxCount = 0;
			
			SendTo(&crc16_key,1);
		}	
		delay_ms(1);
		i++;
		if((RxBuff[0] == SOH) && RxBuff[1] == cur_packet && (RxCount == 133))
		{
			//��У��
			if(crc16(&RxBuff[3],128) == (RxBuff[131]<<8 | RxBuff[132]))
			{
				//У��ͨ��
				//�յ��˷����ߵĻ�Ӧ
				strcpy(recive_filename,&RxBuff[3]);//�����ļ���
				*strchr(RxBuff+strlen(&RxBuff[3])+4,' ') = '\0';
				strcpy(recive_filesize,RxBuff+strlen(&RxBuff[3])+4);//�����ļ���С�ַ���
				
				printf("��һ֡����:");
				for(int i=0;i<133;i++)
				{
					printf("0x%x ",RxBuff[i]);
				}
				printf("\r\n");
				
				
				OLED_ShowString(0,0,"name:",5,8);
				OLED_ShowString(40,0,recive_filename,strlen(recive_filename),8);
				OLED_ShowString(0,2,"size:",5,8);
				OLED_ShowString(40,2,recive_filesize,strlen(recive_filesize),8);
				
				
				printf("�ļ���С:%s\r\n",recive_filesize);
				
				//�����ļ���С������Ҫ���ն��ٴ�
				other_recv_len = StrToNum(recive_filesize);
				printf("�ļ���С:%d\r\n",other_recv_len);
				//other_recv_len = file_len;//����ʣ����Ҫ�����ֽ���

				//OLED_ShowString(40,2,recive_filesize,strlen(recive_filesize),8);
				
				//��մ���1�����ж�ȫ������ͼ����±�
				memset(RxBuff,0,sizeof(RxBuff));
				RxCount = 0;
				
				cur_packet++;
				SendTo(&ack,1);//����ACK�źŻ�ȥ��������
				SendTo(&crc16_key,1);//����C��ȥ��������
				break;//����ѭ��
			}
			else
			{
				//printf("У��ʧ��,�������߷�����Ӧ��\r\n");
				memset(RxBuff,0,sizeof(RxBuff));
				RxCount = 0;
				i=1;
				SendTo(&nak,1);//����NAK�źŻ�ȥ��������
			}
			
		}
	}
	
	
	//Flash��������һ�μ���  ֻ��������4---APP
	//STMFLASH_EraseSector(4);
	
	int EOT_num=0;
	while(1)
	{
		if(RxBuff[0] == EOT && other_recv_len==0)
		{
			memset(RxBuff,0,sizeof(RxBuff));
			RxCount = 0;
			if(EOT_num == 0)//��һ���յ�EOT
			{
				printf("�յ��˵�һ��ֹͣ�����ź�\r\n");
				EOT_num++;
				SendTo(&nak,1);//����NAK�źŻ�ȥ��������
			}
			else //�ڶ����յ�EOT
			{
				printf("�յ��˵ڶ���ֹͣ�����ź�\r\n");
				SendTo(&ack,1);//����ACK�źŻ�ȥ��������
				SendTo(&crc16_key,1);//����C�źŻ�ȥ��������
			}
		}
		
		if(RxBuff[0] == STX && RxBuff[1] == cur_packet && RxCount==1029)
		{
			//�յ�1024�ļ����� ͬʱ���������ȡ�����ļ���С���������������Ҫ���ٴ�
			//��У��
			if(crc16(&RxBuff[3],1024) == (RxBuff[1027]<<8 | RxBuff[1028]))
			{
				//У��ɹ�
				if(other_recv_len<1024)
				{
					//��other_recv_len��������д��flash
					STMFLASH_Write(Flash_addr,&RxBuff[3],other_recv_len,cur_packet);
					
					other_recv_len=0;//��������
					printf("�ɹ��յ���%d������,ʣ�����С��:%d\r\n",cur_packet,other_recv_len);
					
					memset(RxBuff,0,sizeof(RxBuff));
					RxCount = 0;
					
				}
				else
				{
					//��1024�ֽ�д��flash
					if(0 != STMFLASH_Write(Flash_addr,&RxBuff[3],1024,cur_packet))
					{
						printf("Flashдʧ��\r\n");
					}
					other_recv_len -= 1024;//ʣ�೤��������
					printf("�ɹ��յ���%d������,ʣ�����С��:%d\r\n",cur_packet,other_recv_len);
					Flash_addr+=1024;//Flashд���ַƫ��
					printf("Flash_addr:0x%x  ������һ�����ǣ�0x%x\r\n",Flash_addr,*((unsigned int *)&RxBuff[3]));
				}
				
				memset(RxBuff,0,sizeof(RxBuff));
				RxCount = 0;
				cur_packet++;
				SendTo(&ack,1);//����ACK�źŻ�ȥ��������
			}
			else //У��ʧ��
			{
				memset(RxBuff,0,sizeof(RxBuff));
				RxCount = 0;
				SendTo(&nak,1);//����ACK�źŻ�ȥ��������
			}
		}
		
		else if(RxBuff[0] == SOH && RxCount==133)
		{
			if(RxBuff[1] == cur_packet)
			{
				//�յ�����<=128�ֽ��ļ���������֡
				if(crc16(&RxBuff[3],128) == (RxBuff[131]<<8 | RxBuff[132]))
				{
					//У��ɹ�
					//��other_recv_len��������д��flash
					STMFLASH_Write(Flash_addr,&RxBuff[3],other_recv_len,cur_packet);
					
					other_recv_len=0;//��������
					printf("�ɹ��յ���%d������,ʣ�����С��:%d\r\n",cur_packet,other_recv_len);
					memset(RxBuff,0,sizeof(RxBuff));
					RxCount = 0;	

					cur_packet++;
					SendTo(&ack,1);//����ACK�źŻ�ȥ��������
					delay_s(1);
					SendTo(&can,1);//����NAK�źŻ�ȥ���ӻ�
					break;//��������
				}
				else //У��ʧ��
				{
					memset(RxBuff,0,sizeof(RxBuff));
					RxCount = 0;
					SendTo(&nak,1);//����NAK�źŻ�ȥ��������
				}
			}
			else if(RxBuff[1] == 0x00)
			{
				//�յ���ֹͣ֡�������ٷ����������ļ����ݷ�����
				printf("�Է�������ֹͣ֡\r\n");
				SendTo(&ack,1);//����NAK�źŻ�ȥ���ӻ�
				delay_s(1);
				SendTo(&can,1);//����NAK�źŻ�ȥ���ӻ�
				break;//��������
			}
		
		}

	}
	
	return 0;//�ɹ��������
}










