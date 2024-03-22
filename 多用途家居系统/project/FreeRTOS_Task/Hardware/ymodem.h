#ifndef __YMODEM_H
#define __YMODEM_H

#include <stm32f4xx.h>
#include "delay.h"
#include "sys.h"
#include "uart.h"
#include <string.h>
#include "flash.h"
#include "led.h"
#include "oled.h"


#define SOH 0x01
#define STX 0x02
#define EOT 0x04
#define ACK 0x06
#define NAK 0x15
#define CAN 0x18
#define CRC16 0x43

#define SendTo USART1_SendString


int Ymodem_Update(void);//¸üÐÂº¯Êý


#endif


