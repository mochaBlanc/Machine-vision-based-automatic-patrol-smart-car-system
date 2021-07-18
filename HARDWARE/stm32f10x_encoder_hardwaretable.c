/**
  ******************************************************************************
  * @file    stm32f10x_scom_hardwaretable.c
  * @author  J.F.YUE 
  * @version V1.0.0
  * @date    1/1/2019
  * @brief   This file provides all the encoder channel io configuration.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, HANGZHOU NORMAL UNIVERSITY YUEJUNFENG SHALL NOT BE HELD LIABLE
  * FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS 
  * ARISING FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF
  * THE CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2014 HANGZHOU NORMAL UNIVERSITY </center></h2>
  */
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_encoder.h"

/*-----------------------------Hardware_Table_Derition--------------------------------
*				ENC_TIMx;
*				ENC_TIM_Channel1;
*				ENC_TIM_Channel2;
*				GPIOx;
*				GPIO_PinA;
*				GPIO_PinB;
 *------------------------------------------------------------------------------------*/
 
 /* --------------------------------- Encoder Hardware ----------------------------------- */
 ENC_Hardware_TypeDef_Struct ENC_Hardware[] = 
{
	#ifdef ENC1
			{ TIM4,TIM_Channel_1,TIM_Channel_2,GPIOB,GPIO_Pin_6,GPIO_Pin_7}, 
	#endif

	#ifdef ENC2
			{ TIM5,TIM_Channel_1,TIM_Channel_2,GPIOA,GPIO_Pin_0,GPIO_Pin_1}, 
	#endif

	#ifdef ENC3
			{ TIM2,TIM_Channel_1,TIM_Channel_2,GPIOA,GPIO_Pin_0,GPIO_Pin_1}, 
	#endif

	#ifdef ENC4
			{ TIM3,TIM_Channel_1,TIM_Channel_2,GPIOA,GPIO_Pin_6,GPIO_Pin_7}, 
	#endif

};
 