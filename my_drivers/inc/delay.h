#ifndef _DELAY_H_
#define _DELAY_H_

#include "stm32f10x.h"
void delay_init(void);
void delay_us(int32_t nus);
void delay_ms(int16_t nms);
//void delay_us(u16 time);


//void delay_ms(u16 time);

//void Delay(uint16_t  Time);

#endif
