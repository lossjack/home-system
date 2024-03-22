#ifndef __AT24C02_H
#define __AT24C02_H

#include "stm32f4xx.h" 
#include "sys.h"
#include "iic.h"

#define AT24C02_ADDR 0xA0

int AT24C02_SavaByte(unsigned char Addr,unsigned char Data);
int AT24C02_ReadByte(unsigned char Addr,unsigned char *Data);
int AT24C02_SavaNByte(unsigned char Addr,unsigned char *Data,int len);
int AT24C02_ReadNByte(unsigned char Addr,unsigned char *Data,int len);

#endif



