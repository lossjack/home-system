#include "at24c02.h"

//0x00~0xFF

int AT24C02_SavaByte(unsigned char Addr,unsigned char Data)
{
	IIC_Start();
	IIC_SendByte(AT24C02_ADDR);
	if(0 != IIC_ReadACK())
	{
		return -1;
	}
	
	IIC_SendByte(Addr);
	if(0 != IIC_ReadACK())
	{
		return -2;
	}
	
	IIC_SendByte(Data);
	if(0 != IIC_ReadACK())
	{
		return -3;
	}
	
	IIC_Stop();
	return 0;//成功
}

//从AT24C02读取一个字节
int AT24C02_ReadByte(unsigned char Addr,unsigned char *Data)
{
	IIC_Start();
	IIC_SendByte(AT24C02_ADDR);
	if(0 != IIC_ReadACK())
	{
		return -1;
	}

	IIC_SendByte(Addr);
	if(0 != IIC_ReadACK())
	{
		return -2;
	}
	
	IIC_Start();//重新启动 更新读写方向
	IIC_SendByte(AT24C02_ADDR | 0x01);//改为读模式
	if(0 != IIC_ReadACK())
	{
		return -3;
	}
	
	*Data = IIC_ReadByte();
	IIC_WriteACK(1);
	
	IIC_Stop();
	return 0;
}



int AT24C02_SavaNByte(unsigned char Addr,unsigned char *Data,int len)
{
	IIC_Start();
	
	IIC_SendByte(AT24C02_ADDR);
	if(0!= IIC_ReadACK())
	{
		return -1;//无应答
	}
	
	IIC_SendByte(Addr);
	if(0!= IIC_ReadACK())
	{
		return -2;//无应答
	}
	
	while(len--)
	{
		IIC_SendByte(*Data);
		if(0!= IIC_ReadACK())
		{
			return -3;//无应答
		}
		Data++;
	}
	
	IIC_Stop();
	return 0;
}	


int AT24C02_ReadNByte(unsigned char Addr,unsigned char *Data,int len)
{
	IIC_Start();
	
	IIC_SendByte(AT24C02_ADDR);
	if(IIC_ReadACK() != 0)
	{
		return -1;
	}
	
	IIC_SendByte(Addr);
	if(IIC_ReadACK() != 0)
	{
		return -2;
	}
	
	IIC_Start();//重新启动
	
	IIC_SendByte(AT24C02_ADDR | 0x01);//更换为读取模式
	if(IIC_ReadACK() != 0)
	{
		return -3;
	}
	
	len--;
	while(len--)
	{
		*Data = IIC_ReadByte();
		IIC_WriteACK(0);//len-1个字节读取都是 主机发送有应答
		Data++;
	}
	*Data = IIC_ReadByte();
	IIC_WriteACK(1);//最后一个字节读取完，主机发送无应答
	
	IIC_Stop();
	return 0;//成功
}



















