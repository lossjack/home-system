#ifndef __MQTT_H
#define __MQTT_H

#include "stm32f4xx.h"
#include <string.h>
#include <stdio.h>
#include "uart.h"
#include "delay.h"


//ESP8266 AT指令测试
int ESP8266_CWModeSet(char mode);
int ESP8266_ATE(char mode);
int ESP8266_ConnectWifi(char *SSID,char *Passwd);
int ESP8266_ConnectTCP(char *ip,unsigned short port);
int ESP8266_SetCIPMODE(char mode);
int ESP8266_SendData(void);
int ESP8266_ExitSend(void);//退出透传模式


//MQTT协议函数
int MQTT_Connect(char *ClientID,char *UserName,char *Passwd,unsigned short KeepAlive);
int MQTT_Subscribe(char *Topic,unsigned char Qos,unsigned short flags);
int MQTT_PINGREQ(void);
#endif
