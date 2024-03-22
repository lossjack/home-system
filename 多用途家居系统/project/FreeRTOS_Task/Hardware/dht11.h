#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"

//DQ-->PG9
#define DHT11_DQ_OUT  PGout(9)
#define DHT11_DQ_IN   PGin(9)


u8 DHT11_Read_Data(u8 *temph,u8 *humih,u8 *templ,u8 *humil);
u8 DHT11_init(void);
void DHT11_Rst(void);
u8 DHT11_Check(void);
u8 DHT11_Read_Bit(void);
u8 DHT11_Read_Byte(void);



#endif

