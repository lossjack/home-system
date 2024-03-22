#include "oled.h"
#include "oledfont.h"//�ֿ�ͷ�ļ�

void OLED_WR_Byte(u8 dat,u8 cmd)
{				  
	if(cmd)
	  OLED_DC = 1;
	else 
	  OLED_DC = 0;	

	SPI_CS = 0; //CS = 0
	SPI0_ReadWriteByte(dat);
	SPI_CS = 1;//CS = 1
	
	OLED_DC = 1;//����������OLED����ع�Ϊдͼ������ģʽ   	  
} 

void OLED_Config(void)
{
	//����OLED����ӦSPI���ź�DC  RES����
	SPI_Config();
	
	
	//��λOLED��
	OLED_RES = 1;
	delay_ms(100);
	OLED_RES = 0;//��λ
	delay_ms(100);
	OLED_RES = 1;
	
	//����ָ������OLED��ʾ����
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x02,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0x81,OLED_CMD); //�Աȶ�����
	OLED_WR_Byte(0xfF,OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	
	//����
	OLED_Clear();
	
	//������ʼλ��
	OLED_Set_Pos(0,0);
}


//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ  0~127
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   0000 0000~0111 1111 
		
		//�ڵ�ǰҳ дͼ�����ݣ����Զ�ƫ�ơ������ܿ�ҳ
		for(n=0;n<128;n++)
			OLED_WR_Byte(0x00,OLED_DATA); 
	} //������ʾ
}

/*
x��0~127
y: 0~7
*/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	
	OLED_WR_Byte(((x&0x70)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}  


void OLED_ShowChar(u8 x,u8 y,u8 chr,int size)
{      	
	unsigned char c=0,i=0;	
	c=chr-' ';//�õ�ƫ�ƺ��ֵ			
	if(x>128-1)
	{	
		x=0;
		y=y+2;
	}
	
	if(size ==8)
	{
		OLED_Set_Pos(x,y);	
		for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
		
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
	}
	else if(size == 6)
	{	
		OLED_Set_Pos(x,y+1);
		
		for(i=0;i<6;i++)
			OLED_WR_Byte(F6x8[c][i],OLED_DATA);
		
	}
}



//size:8---8*16  6---6*8
void OLED_ShowString(u8 x,u8 y,u8 *chr,int len,int size)
{
	while(len--)
	{
		//�Զ���ҳ
		if(x+size>128)
		{
			if(size == 8)
				y+=2;
			else if(size == 6)
				y+=1;
			x=0;
		}
		if(y>7)
			return;//����ҳ��Χ
		OLED_ShowChar(x,y,*chr,size);
		chr++;
		x+=size;
	}
	return ;
}

int oled_pow(int m,int n)
{
	int num = 1;
	while(n--)
	{
		num*=m;
	}
	return num;
}
    
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;//1
		
		if(enshow==0 && t <(len-1))
		{
			if(temp==0)
			{
				//6*8  8*16
				OLED_ShowChar(x+size*t,y,' ',size);
				continue;
			}
			else 
				enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+size*t,y,temp+'0',size); 
	}
} 

