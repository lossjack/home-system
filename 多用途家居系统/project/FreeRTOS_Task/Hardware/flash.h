#ifndef __FLASH_H
#define __FLASH_H

#include <stm32f4xx.h>
#include "delay.h"
#include "sys.h"

//FLASH 起始地址
#define STM32_FLASH_BASE 0x08000000 //STM32 FLASH 的起始地址

//FLASH 解锁键值
#define FLASH_KEY111 0X45670123
#define FLASH_KEY222 0XCDEF89AB

//FLASH 扇区的起始地址  512KByte  0x08000000 ~ 0x0807FFFF
#define ADDR_FLASH_SECTOR_0 ((u32)0x08000000) //扇区 0 起始地址, 16 Kbytes 
#define ADDR_FLASH_SECTOR_1 ((u32)0x08004000) //扇区 1 起始地址, 16 Kbytes 
#define ADDR_FLASH_SECTOR_2 ((u32)0x08008000) //扇区 2 起始地址, 16 Kbytes 
#define ADDR_FLASH_SECTOR_3 ((u32)0x0800C000) //扇区 3 起始地址, 16 Kbytes 
#define ADDR_FLASH_SECTOR_4 ((u32)0x08010000) //扇区 4 起始地址, 64 Kbytes 
#define ADDR_FLASH_SECTOR_5 ((u32)0x08020000) //扇区 5 起始地址, 128 Kbytes 
#define ADDR_FLASH_SECTOR_6 ((u32)0x08040000) //扇区 6 起始地址, 128 Kbytes 
#define ADDR_FLASH_SECTOR_7 ((u32)0x08060000) //扇区 7 起始地址, 128 Kbytes

//后面没有
#define ADDR_FLASH_SECTOR_8 ((u32)0x08080000) //扇区 8 起始地址, 128 Kbytes 
#define ADDR_FLASH_SECTOR_9 ((u32)0x080A0000) //扇区 9 起始地址, 128 Kbytes 
#define ADDR_FLASH_SECTOR_10 ((u32)0x080C0000) //扇区 10 起始地址,128 Kbytes 
#define ADDR_FLASH_SECTOR_11 ((u32)0x080E0000) //扇区 11 起始地址,128 Kbytes 

void STMFLASH_Unlock(void); //FLASH 解锁
void STMFLASH_Lock(void); //FLASH 上锁
u8 STMFLASH_GetStatus(void); //获得状态
u8 STMFLASH_WaitDone(u32 time); //等待操作结束
u8 STMFLASH_ErasePage(u32 sectoraddr); //擦除页
u8 STMFLASH_EraseSector(u32 sectoraddr);//扇区擦除操作
u8 STMFLASH_WriteWord(u32 faddr, u32 dat); //写入字
u32 STMFLASH_ReadWord(u32 faddr); //读出字 


//从指定地址开始写入指定长度的数据
int STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite,int num);

//从指定地址开始读出指定长度的数据 
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead); 



#endif

