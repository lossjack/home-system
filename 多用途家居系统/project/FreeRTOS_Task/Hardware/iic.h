#ifndef __IIC_H
#define __IIC_H

#include "stm32f4xx.h"
#include "delay.h"
#include "sys.h"

#define SCL 	PBout(8)
#define SDA 	PBout(9)
#define SDA_R 	PBin(9)

void IIC_Config(void);

void IIC_Start(void);
void IIC_Stop(void);
void  IIC_SendByte(unsigned char  WData);
unsigned char  IIC_ReadByte(void);
unsigned char  IIC_ReadACK(void);
void  IIC_WriteACK(unsigned char ack);


#endif

