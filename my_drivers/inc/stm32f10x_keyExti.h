/**
  ******************************************************************************
  * @file    stm32f10x_keyExti.h
  * @author  YUE JUNFENG
  * @version V1.0.0
  * @date    24/DEC/2018
  * @brief   This file provides external interrupt by key
	* @update  
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10X_KEYEXTI_H
#define __STM32F10X_KEYEXTI_H	
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h" 	
#include "delay.h"
#include "stm32f10x_exti.h"
#include <string.h> 

//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
//IO口操作,只对单一的IO口!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 

#define LED0 PAout(8)	// PA8
#define LED1 PDout(2)	// PD2

#define KEY0  	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)//读取按键0
#define KEY1  	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//读取按键1
#define WK_UP  	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键2 
 

#define KEY0_PRES	1		//KEY0  
#define KEY1_PRES	2		//KEY1 
#define WKUP_PRES	3		//WK_UP  




void KEY_Init(void);
void EXTIX_Init(void);
void LED_Init(void);
/**
  * @}
  */

#endif /*__STM32F10X_KEYEXTI_H */