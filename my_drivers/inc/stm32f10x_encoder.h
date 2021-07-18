#ifndef __STM32F10X_ENCODER_H
#define __STM32F10X_ENCODER_H
/******************************************************************************
*                                                                             *
* @file     stm32f10x_encoder.c                                               *
* @version  V1.0.0                                                            *
* @date     24/DEC/2018
* @author   YUE JUNFENG
* @brief    This file provides all the counter functions related to basic TIM *
*           channel 1|2 for ENCODER.																					*	
*                                                                             *
******************************************************************************/ 

/*------------------ Define to prevent recursive inclusion ------------------*/
	
/* Includes ------------------------------------------------------------------*/
#include <stm32f10x.h>
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include <math.h>
#include <string.h>
#include <misc.h>
/******************************************************************************
*                                                                             
* @brief  ENC_DATA_Struct structure definition                    
*        
*         Velocity: current vehicle velocity
*					Encoder_data
*
******************************************************************************/
typedef struct __ENC_DATA_Struct
{
   int          						Velocity; 
	uint16_t  								Encoder_data;	
} ENC_DATA_Struct;

/** 
  * @brief  ENC Hardware structure definition  
  */

typedef struct __ENC_Hardware_TypeDef_Struct
{
	TIM_TypeDef      *TIMx;
	uint16_t          TIM_Channel1;
	uint16_t          TIM_Channel2;
	GPIO_TypeDef			*GPIOx;
	uint16_t          GPIO_PinA;
	uint16_t          GPIO_PinB;
	
}ENC_Hardware_TypeDef_Struct;


/**
  * @brief Configuration Encoder_Clock_Division_CKD 
  */	
typedef enum __ENC_CKD_ENUM
{
	ENC_CKD_DIV1        =               0x0000,
	ENC_CKD_DIV2        =               0x0100,
	ENC_CKD_DIV4        =               0x0200,
}ENC_CKD_ENUM;

/**
  * @brief Configuration Encoder_Input_Capture_Polarity 
  */	
typedef enum __ENC_IC_Polarity_ENUM
{
  ENC_ICPolarity_Rising         =    0x0000,
  ENC_ICPolarity_Falling        =    0x0002,
  ENC_ICPolarity_BothEdge       =    0x000A,
}ENC_IC_Polarity_ENUM;

/**
  * @brief Configuration Encoder_Mode 
  */	
typedef enum __ENC_Mode_ENUM
{
	ENC_EncoderMode_TI1  = 0x0001,
	ENC_EncoderMode_TI2  = 0x0002,
	ENC_EncoderMode_TI12 = 0x0003,	
}ENC_Mode_ENUM;

/**
  * @brief Configuration CounterMode enumration
  */
typedef enum __ENC_CounterMode_ENUM
{	
	ENC_CounterMode_up               =	0x0000,
	ENC_CounterMode_Down             =  0x0010,
	ENC_CounterMode_CenterAligned1   =  0x0020,
	ENC_CounterMode_CenterAligned2   =  0x0040,
	ENC_CounterMode_CenterAligned3   =  0x0060,
}ENC_CounterMode_ENUM;


/** 
  * @brief  Encoder Memory structure definition  
  */

typedef struct __ENC_Memory_TypeDef_Struct
{
	ENC_IC_Polarity_ENUM			ENC_ICpola;
	ENC_Mode_ENUM							ENC_mode;
	ENC_CounterMode_ENUM			ENC_CountM;
	uint16_t          				ENC_Arr;
	uint16_t          				ENC_Psc;
	uint16_t                  ENC_ICFilter;
	ENC_CKD_ENUM              ENC_CKD;
	
}ENC_Memory_TypeDef_Struct;



/** 
  * @brief  ENC Type structure definition  
  */

typedef struct __ENC_TypeDef_Struct
{
	ENC_Hardware_TypeDef_Struct *ENC_Hardware;
	ENC_Memory_TypeDef_Struct   ENC_OperatingParameter;
}ENC_TypeDef_Struct;

/** 
  * @brief  ENC Init structure definition  
  */

typedef struct __ENC_InitTypeDef_Struct
{
	ENC_IC_Polarity_ENUM			ENC_ICpola;
	ENC_Mode_ENUM							ENC_mode;
	ENC_CounterMode_ENUM			ENC_CountM;
	uint16_t          				ENC_Arr;
	uint16_t          				ENC_Psc;
	uint16_t                  ENC_ICFilter;
	ENC_CKD_ENUM              ENC_CKD;
	
}ENC_InitTypeDef_Struct;



/* ------------------------------- ENC_nums_define --------------------------------- */
/* It is not permitted to modify ENC_NUM_MAX greater than 4 */
#define ENC_NUM_MAX                           4 
/* It is not permitted to modify ENC_NUM_USED greater than ENC_NUM_MAX */
#define ENC_NUM_USED                          2

#if(ENC_NUM_MAX>4)
#error ENC_NUM_MAX GREATER THAN 4
#endif

#if(ENC_NUM_USED>=1)
extern ENC_TypeDef_Struct ENC1_BASE;
#define ENC1                       							(&ENC1_BASE)      /* ENC 1 selected */
#else
#define ENC1                        		 					NULL             /* ENC 1 not selected */
#endif

#if(ENC_NUM_USED>=2)
extern ENC_TypeDef_Struct ENC2_BASE;
#define ENC2                       							(&ENC2_BASE)      /* ENC 2 selected */
#else
#define ENC2                        		 					NULL             /* ENC 2 not selected */
#endif

#if(ENC_NUM_USED>=3)
extern ENC_TypeDef_Struct ENC3_BASE;
#define ENC3                       							(&ENC3_BASE)      /* ENC 3 selected */
#else
#define ENC3                        		 					NULL             /* ENC 3 not selected */
#endif

#if(ENC_NUM_USED>=4)
extern ENC_TypeDef_Struct PWM4_BASE;
#define ENC4                      							(&ENC4_BASE)      /* ENC 4 selected */
#else
#define ENC4                       		 					NULL             /* ENC 4 not selected */
#endif

/* ------------------------------ENC_InitTypeDef_Struct Default_Value define ------------------------------- */
#define ENC_IC_POLARITY_DEFAULT                ENC_ICPolarity_Rising
#define ENC_COUNTERMODE_DEFAULT                ENC_CounterMode_up 
#define ENC_MODE_DEFAULT                     	 ENC_EncoderMode_TI12

/**
  * @brief  Fills each ENC_InitStruct member with its default value.
  * @param ENC_InitStruct : pointer to a ENC_InitTypeDef
  *   structure which will be initialized.
  * @retval : None
  */
void ENC_StructInit(ENC_InitTypeDef_Struct* ENCx_InitStruct);

/**
  * @brief  Initializes the ENCx  according to the specified
  *   parameters in the ENC_OutInitStruct.
  * @param ENCx: where x can be  1 2 to ENC_NUM_USED  to select the ENC.
  *        ENC_NUM_USED is defined at stm32f10x_encoder.h, can be 1 to ENC_NUM_MAX, 
	*        and ENC_NUM_MAX is also defined at stm32f10x_encoder.h, can be 1 to 4.
  * @param ENC_InitStruct : pointer to a ENC_InitTypeDef
  *   structure which will be initialized.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus ENC_Init(ENC_TypeDef_Struct*	ENCx,	ENC_InitTypeDef_Struct*	ENCx_InitStruct);

/**
  * @brief GPIOconfiguration
  */ 	
ErrorStatus ENC_GPIOConfig(ENC_TypeDef_Struct *ENCx);

/**
  * @brief  Enable the specified Advance ENC peripheral.
  * @param ENCx: where x can be 1 to 4 to select the ENCx peripheral.
  *        And at this time, the ENC_NUM_MAX and ENC_NUM_USED which is defined at stm32f10x_encoder.h
  *        must equal to x or be greater than x.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus ENC_On(ENC_TypeDef_Struct* ENCx);

/******************************************************************************
* @brief  Read the float value counter of TIMx encoder mode. 
*
*                                                                  					  *
* @param  ENCx: where x can be 1 to ENC_NUM_USED to  									        * 
*         select the ENC. ENC_NUM_USED is defined at 									        *
*         stm32f10x_enocder.h,can be 1 to ENC_NUM_MAX,and          			  		*
*         ENC_NUM_MAX is also defined at stm32f10x_enocder.h,can   						*
*         be 1 to 4.  
*@param   ENC_DATA_Struct  ENC:change opening of the ENCx.   									*
*         ENC.data: counter of the TIMx,This parameter can be:								*
*					0 to 65536(0xFFFF)            																			*
*                                                      											  *
*        ENC.velocity  This parameter can reflect the current speed of vehicle  
*
            	 																															  *
 * @retval :	ERROR or SUCCESS.                            										*
******************************************************************************/
ErrorStatus Read_Encoder(ENC_TypeDef_Struct *ENCx,ENC_DATA_Struct *pdata);

//void ENC_Init(void) ;

void Timer1_Init(void);

#endif /*__STM32F10X_PWMCAP_H */


/**
  * @}
  */