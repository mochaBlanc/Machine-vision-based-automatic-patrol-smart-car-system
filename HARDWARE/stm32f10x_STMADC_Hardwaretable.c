#include "stm32f10x_STMADC.h"

/******************************************************************************
* @file  THROTTLE_SENSOR_hardwaretable.c                                      *
* @brief  This file provides all the THROTTLE_SENSOR firmware functions.      *
******************************************************************************/
/*-------------------------------- Includes ---------------------------------*/
#include "stm32f10x_STMADC.h"
#include "stm32f10x_gpio.h"

/*------------------------- Hardware_Table_Derition -------------------------- *
* stm32f10x_STMADCX                                                            *
* stm32f10x_STMADC1_GPIOx;                                                     *
* stm32f10x_STMADC1_GPIO_Pin;                                                  *
* stm32f10x_STMADC1_ADC_Channel; 																							 *
*	stm32f10x_STMADC1_DMA_Channel;																							 *
* stm32f10x_STMADC2_GPIOx;                                                     *
* stm32f10x_STMADC2_GPIO_Pin;                                                  *
* stm32f10x_STMADC2_ADC_Channel; 
*	stm32f10x_STMADC2_DMA_Channel

* stm32f10x_STMADC3_GPIOx;                                                     *
* stm32f10x_STMADC3_GPIO_Pin;                                                  *
* stm32f10x_STMADC3_ADC_Channel;                                               *
* stm32f10x_STMADC4_GPIOx;                                                     *
* stm32f10x_STMADC4_GPIO_Pin;                                                  *
* stm32f10x_STMADC4_ADC_Channel;                                               *
*-----------------------------------------------------------------------------*/
 
/* ----------------------- THROTTLE_SENSOR Hardware ------------------------- */
STMADC_Hardware_TypeDef_Struct STMADC_Hardware[] = 
{
   
  {STMADC1_ADC,STMADC1_GPIO,STMADC1_GPIO_Pin,STMADC1_Channel, STMADC1_DMA_Channel},

  {STMADC2_ADC,STMADC2_GPIO,STMADC2_GPIO_Pin,STMADC2_Channel,STMADC2_DMA_Channel},

  {STMADC3_ADC,STMADC3_GPIO,STMADC3_GPIO_Pin,STMADC3_Channel,},

  {STMADC4_ADC,STMADC4_GPIO,STMADC4_GPIO_Pin,STMADC4_Channel,},


};  