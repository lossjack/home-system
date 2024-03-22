#include "oled.h"
#include "oledfont.h"//字库头文件

void OLED_WR_Byte(u8 dat,u8 cmd)
{				  
	if(cmd)
	  OLED_DC = 1;
	else 
	  OLED_DC = 0;	

	SPI_CS = 0; //CS = 0
	SPI0_ReadWriteByte(dat);
	SPI_CS = 1;//CS = 1
	
	OLED_DC = 1;//无论如何最后OLED都会回归为写图像数据模式   	  
} 

void OLED_Config(void)
{
	//配置OLED屏对应SPI引脚和DC  RES引脚
	SPI_Config();
	
	
	//复位OLED屏
	OLED_RES = 1;
	delay_ms(100);
	OLED_RES = 0;//复位
	delay_ms(100);
	OLED_RES = 1;
	
	//发送指令配置OLED显示参数
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x02,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0x81,OLED_CMD); //对比度设置
	OLED_WR_Byte(0xfF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
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
	
	//清屏
	OLED_Clear();
	
	//设置起始位置
	OLED_Set_Pos(0,0);
}


//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址  0~127
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   0000 0000~0111 1111 
		
		//在当前页 写图像数据，会自动偏移。但不能跨页
		for(n=0;n<128;n++)
			OLED_WR_Byte(0x00,OLED_DATA); 
	} //更新显示
}

/*
x：0~127
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
	c=chr-' ';//得到偏移后的值			
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
		//自动换页
		if(x+size>128)
		{
			if(size == 8)
				y+=2;
			else if(size == 6)
				y+=1;
			x=0;
		}
		if(y>7)
			return;//超出页范围
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

