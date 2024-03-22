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
	return 0;//�ɹ�
}

//��AT24C02��ȡһ���ֽ�
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
	
	IIC_Start();//�������� ���¶�д����
	IIC_SendByte(AT24C02_ADDR | 0x01);//��Ϊ��ģʽ
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
		return -1;//��Ӧ��
	}
	
	IIC_SendByte(Addr);
	if(0!= IIC_ReadACK())
	{
		return -2;//��Ӧ��
	}
	
	while(len--)
	{
		IIC_SendByte(*Data);
		if(0!= IIC_ReadACK())
		{
			return -3;//��Ӧ��
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
	
	IIC_Start();//��������
	
	IIC_SendByte(AT24C02_ADDR | 0x01);//����Ϊ��ȡģʽ
	if(IIC_ReadACK() != 0)
	{
		return -3;
	}
	
	len--;
	while(len--)
	{
		*Data = IIC_ReadByte();
		IIC_WriteACK(0);//len-1���ֽڶ�ȡ���� ����������Ӧ��
		Data++;
	}
	*Data = IIC_ReadByte();
	IIC_WriteACK(1);//���һ���ֽڶ�ȡ�꣬����������Ӧ��
	
	IIC_Stop();
	return 0;//�ɹ�
}



















