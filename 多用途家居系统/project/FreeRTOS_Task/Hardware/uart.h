#ifndef __UART_H
#define __UART_H

#include "stm32f4xx.h"
#include <stdio.h>

//全局变量声明
extern char RxBuff[512]; //Ymodem最大一帧 1029Byte 1+1+1+1024+1+1
extern char RxFlags;
extern int  RxCount;

void UART1_Config(unsigned int BaudRate);
void  USART1_SendString(char *str,int len);



extern unsigned char ESP8266_RxBuff[512];
extern int  ESP8266_RxCount;//记录下标--串口收到的数据字节数
extern char ESP8266_RxFlags;

void UART3_Config(unsigned int BaudRate);
void  USART3_SendString(unsigned char *str,int len);
#endif
