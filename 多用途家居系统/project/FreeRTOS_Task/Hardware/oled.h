#ifndef __OLED_H
#define __OLED_H

#include "stm32f4xx.h"
#include "delay.h"
#include "sys.h"
#include "spi.h"


#define OLED_CMD  0	//Ð´ÃüÁî
#define OLED_DATA 1	//Ð´Êý¾Ý


void OLED_Config(void);

void OLED_Clear(void);
void OLED_Set_Pos(unsigned char x, unsigned char y) ;
void OLED_ShowChar(u8 x,u8 y,u8 chr,int size);
void OLED_ShowString(u8 x,u8 y,u8 *chr,int len,int size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);


#endif
