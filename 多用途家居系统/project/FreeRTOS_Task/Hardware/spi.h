#ifndef __SPI_H
#define __SPI_H

#include "stm32f4xx.h"
#include "delay.h"
#include "sys.h"

#define SPI_SCK 	PBout(15)
#define SPI_MOSI 	PDout(10)
#define SPI_CS		PEout(12)

#define OLED_RES   	PDout(8)
#define OLED_DC		PEout(14)

void SPI_Config(void);

void SPI0_ReadWriteByte(unsigned char WData);

#endif
