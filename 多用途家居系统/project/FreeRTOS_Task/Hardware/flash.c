#include "flash.h"


//解锁 STM32 的 FLASH
void STMFLASH_Unlock(void)
{
	FLASH->KEYR=FLASH_KEY111;//写入解锁序列.
	FLASH->KEYR=FLASH_KEY222; 
}


//flash 上锁
void STMFLASH_Lock(void)
{
	FLASH->CR|=(u32)1<<31;//上锁
}


//得到 FLASH 状态
//返回值:
//0,操作完成
//1,忙
//2,操作异常
u8 STMFLASH_GetStatus(void)
{
	u32 res=0;
	res=FLASH->SR; 
	if(res&(1<<16))return 1; //忙
	else if(res&(1<<4))return 2; //操作异常
	else if(res&(1<<5))return 2; //操作异常
	else if(res&(1<<6))return 2; //操作异常
	else if(res&(1<<7))return 2; //操作异常
	return 0; //没有任何状态/操作完成.
} 



//等待操作完成
//time:要延时的长短(单位:10us)
//返回值:
//0,完成
//2,操作异常
//0XFF,超时 
u8 STMFLASH_WaitDone(u32 time)
{
	u8 res=0xFF;
	do
	{
		res=STMFLASH_GetStatus();
		if(res!=1)
			break;//非忙,无需等待了,直接退出.
		delay_us(10);
		time--;
	}while(time);
	if(time==0)res=0xff;//TIMEOUT
	return res;
}



//擦除扇区
//sectoraddr:扇区地址,范围是:0~11.
//0~3,16K 扇区;4,64K 扇区;5~11,128K 扇区.
//返回值:执行情况
u8 STMFLASH_EraseSector(u32 sectoraddr)
{
	u8 res=0;
	
	
	res=STMFLASH_WaitDone(200000);//等待上次操作结束,最大 2s 
	//FLASH->ACR &= ~(1<<10); //FLASH 擦除期间,必须禁止数据缓存!!!
	
	
	//STMFLASH_Unlock(); //解锁
	if(res==0)
	{ 
		FLASH->CR&=~(3<<8); //清除 PSIZE 原来的设置
		FLASH->CR|=2<<8; //设置为 32bit 宽,确保 VCC=2.7~3.6V 之间!!
		
		FLASH->CR &= ~(0X1F<<3);//清除原来设置的扇区编号
		FLASH->CR |= sectoraddr<<3;//设置要擦除的扇区
		
		FLASH->CR|=1<<1; //激活扇区擦除
		
		FLASH->CR|=1<<16; //开始擦除 
		
		res=STMFLASH_WaitDone(200000); //等待操作结束,最大 2s 
		
		if(res!=1) 
			FLASH->CR&=~(1<<1); // 非忙，激活扇区擦除 关闭
	}
	//STMFLASH_Lock();//上锁
	//FLASH->ACR|=1<<10; //FLASH 擦除结束,开启数据 fetch
	
	return res;
}


//在 FLASH 指定地址写一个字word 4Byte
//faddr:指定地址(此地址必须为 4 的倍数!!)
//dat:要写入的数据
//返回值:0,写入成功
// 其他,写入失败
u8 STMFLASH_WriteWord(u32 faddr, u32 dat)
{
	u8 res; 
	res=STMFLASH_WaitDone(0XFF);
	if(res==0)//OK
	{
		FLASH->CR&=~(3<<8); //清除 PSIZE 原来的设置
		FLASH->CR|=2<<8; //设置为 32bit 宽,确保 VCC=2.7~3.6V 之间!!
		FLASH->CR|=1<<0; //编程使能
		*(vu32*)faddr=dat; //写入数据
		
		res=STMFLASH_WaitDone(0XFF);//等待操作完成,一个字编程,最多 100us.
		
		if(res!=1) 
			FLASH->CR&=~(1<<0);//操作成功，清除 PG 位. 
	} 
	return res;
} 

//读取指定地址的一个字(32 位数据) 
//faddr:读地址
//返回值:对应数据.
u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(vu32*)faddr; 
} 

//获取某个地址所在的 flash 扇区
//addr:flash 地址
//0x08002312
//返回值:0~11,即 addr 所在的扇区
u8 STMFLASH_GetFlashSector(u32 addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return 0;
	else if(addr<ADDR_FLASH_SECTOR_2)return 1;
	else if(addr<ADDR_FLASH_SECTOR_3)return 2;
	else if(addr<ADDR_FLASH_SECTOR_4)return 3;
	else if(addr<ADDR_FLASH_SECTOR_5)return 4;
	else if(addr<ADDR_FLASH_SECTOR_6)return 5;
	else if(addr<ADDR_FLASH_SECTOR_7)return 6;
	else if(addr<ADDR_FLASH_SECTOR_8)return 7;
	else if(addr<ADDR_FLASH_SECTOR_9)return 8;
	else if(addr<ADDR_FLASH_SECTOR_10)return 9;
	else if(addr<ADDR_FLASH_SECTOR_11)return 10;
	return 11;
}





//从指定地址开始写入指定长度的数据
//特别注意:因为 STM32F4 的扇区实在太大,没办法本地保存扇区数据,所以本函数
// 写地址如果非 0XFF,那么会先擦除整个扇区且不保存扇区数据.所以
// 写非 0XFF 的地址,将导致整个扇区数据丢失.建议写之前确保扇区里
// 没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写. 
//该函数对 OTP 区域也有效!可以用来写 OTP 区!
//OTP 区域地址范围:0X1FFF7800~0X1FFF7A0F(注意：最后 16 字节，用于 OTP 数据块锁
//别乱写！！)
//WriteAddr:起始地址(此地址必须为 4 的倍数!!)
//pBuffer:数据指针
//NumToWrite:字节(32 位)数(就是要写入的 32 位数据的个数.) 
int STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite,int num)
{ 
	u8 status=0;
	u32 addrx=0;
	u32 endaddr=0;
	
	if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)
		return -1; //非法地址
	
	
	STMFLASH_Unlock(); //解锁
	FLASH->ACR&=~(1<<10); //FLASH 擦除结束,开启数据 fetch
	
	addrx=WriteAddr; //写入的起始地址
	
	endaddr=WriteAddr+NumToWrite; //写入的结束地址
	
	if(addrx<0X1FFF0000) //只有主存储区,才需要执行擦除操作!!
	{
//		while(addrx<endaddr) //扫清一切障碍.(对非 FFFFFFFF 的地方,先擦除)
//		{
//			//有非 0XFFFFFFFF 的地方,要擦除这个扇区
//			if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)
//			{ 
		if(!(num-1))
			status=STMFLASH_EraseSector(STMFLASH_GetFlashSector(addrx));
//		if(status)
//			break; //发生错误了
//			}
//			else 
//				addrx+=4;
//		} 
	}

	if(status==0)
	{
		while(WriteAddr<endaddr)//写数据
		{
			if(STMFLASH_WriteWord(WriteAddr,*pBuffer)) 
				return -2;;//写入数据异常？
			WriteAddr+=4;
			pBuffer++;
		}
	}
	FLASH->ACR|=1<<10; //FLASH 擦除结束,开启数据 fetch
	STMFLASH_Lock();//上锁
	return 0;
} 

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToRead:字(32 位)数
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead) 
{
	u32 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//读取 4 个字节.
		ReadAddr+=4;//偏移 4 个字节.
	}
}

//////////////////////////////////////////测试用///////////////////////////////////////////
//WriteAddr:起始地址
//WriteData:要写入的数据
void Test_Write(u32 WriteAddr,u32 WriteData) 
{
	//STMFLASH_Write(WriteAddr,&WriteData,1);//写入一个字
}




