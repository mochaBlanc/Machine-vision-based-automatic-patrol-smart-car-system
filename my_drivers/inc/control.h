/**
  ******************************************************************************
  * @file    control.h
  * @author  YUE JUNFENG
  * @version V1.0.0
  * @date    24/DEC/2018
  * @brief   This file provides smart car control functions
	* @update  TIM1 ETR capture for external PWM signal 
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10X_CONTROL_H
#define __STM32F10X_CONTROL_H	
/* Includes ------------------------------------------------------------------*/


#include "stm32f10x_tim.h"
#include "stm32f10x.h" 	
#include "stm32f10x_encoder.h"
#include "stm32f10x_STMADC.h"
#include "stm32f10x_pwm.h"
#include "stm32f10x_scom.h" 
#include "stm32f10x_keyExti.h"
#include "string.h"

/**
  * @brief Configuration CounterMode enumration
  */
typedef enum __SysTime_Beat_CounterMode_ENUM
{
	SysTime_Beat_CounterMode_Up               =	 0x0000,
	SysTime_Beat_CounterMode_Down             =  0x0010,
}SysTime_Beat_CounterMode_ENUM;

/** 
  * @brief  SysTime_Beat Hardware structure definition (Internal)
  */

typedef struct __SysTime_Beat_Hardware_TypeDef_Struct
{
	TIM_TypeDef*		  TIMx;
	uint8_t           TIMx_IRQn;
	
}SysTime_Beat_Hardware_TypeDef_Struct;

/** 
  * @brief  SysTime_Beat Memory structure definition  
  */

typedef struct __SysTime_Beat_Memory_TypeDef_Struct
{
	SysTime_Beat_CounterMode_ENUM   SysTime_Beat_CountM;
	uint16_t          		SysTime_Beat_Arr;
	uint16_t          		SysTime_Beat_Psc;
	
}SysTime_Beat_Memory_TypeDef_Struct;

/** 
  * @brief  SysTime_Beat Type structure definition  
  */

typedef struct __SysTime_Beat_TypeDef_Struct
{
	SysTime_Beat_Hardware_TypeDef_Struct *SysTime_Beat_Hardware;
	SysTime_Beat_Memory_TypeDef_Struct   SysTime_Beat_OperatingParameter;
}SysTime_Beat_TypeDef_Struct;

/** 
  * @brief  SysTime_Beat Init structure definition  
  */

typedef struct __SysTime_Beat_InitTypeDef_Struct
{
	SysTime_Beat_CounterMode_ENUM		SysTime_Beat_CountM;
	uint16_t          							SysTime_Beat_Arr;
	uint16_t          							SysTime_Beat_Psc;
	
	uint8_t	                    		SysTime_Beat_PreemptionPriority;
	uint8_t	                   			SysTime_Beat_SubPriority;
	
}SysTime_Beat_InitTypeDef_Struct;

/* ----------------------------- SysTime_Beat Parameter_define ------------------------------- */
#define SYSTIME_BEAT_PREEMPTIONPRIORITY_DEFAULT					1
#define	SYSTIME_BEAT_SUBPRIORITY_DEFAULT								1

/* ------------------------------ System_Clock_define ------------------------------- */
#define SYS_CLOCK                          72000000                   /* unit: Hz */

/* ------------------------------- SysTime_Beat_InitTypeDef_Struct Default_Value define --------------------------------- */
#define SYSTIME_BEAT_COUNTERMODE_DEFAULT                SysTime_Beat_CounterMode_Up
#define SYSTIME_BEAT_CLOCKDIVISION_DEFAULT 							0x0000 
#define SYSTIME_BEAT_ARR_DEFAULT                        5000                     /* unit: us */
#define SYSTIME_BEAT_PSC_DEFAULT                        71                     

/* ------------------------------- SysTime_Beat_nums_define --------------------------------- */
/* It is not permitted to modify SYSTIME_BEAT_NUM_MAX greater than 2 */
#define SYSTIME_BEAT_NUM_MAX                           2
/* It is not permitted to modify SYSTIME_BEAT_NUM_USED greater than SCOM_NUM_MAX */
#define SYSTIME_BEAT_NUM_USED                          1

#if(SYSTIME_BEAT_NUM_USED>=1)
extern SysTime_Beat_TypeDef_Struct    SysTime_Beat_BASE;
#define SysTime_Beat                      (&SysTime_Beat_BASE)     
#define SysTime_Beat_TIM                   TIM2 
#define SysTime_Beat_TIM_IRQn		           28   
#else
#define SysTime_Beat                      NULL     
#define SysTime_Beat_TIM                  NULL 
#define SysTime_Beat_TIM_IRQn		          NULL 

#endif



/**
  * @brief  Fills each Systime_Beat_InitStruct member with its default value.
  * @param Systime_Beat_InitStruct : pointer to a Systime_Beat_InitTypeDef
  *   structure which will be initialized.
  * @retval : None
  */
void SysTime_Beat_StructInit(SysTime_Beat_InitTypeDef_Struct* SysTime_Beatx_InitStruct);

/**
  * @brief  Initializes the Systime_Beatx  according to the specified
  *   parameters in the Systime_Beat_InitStruct.
  * @param Systime_Beatx: where x can be  1 to Systime_Beat_NUM_USED  to select the Systime_Beat.
  *        Systime_Beat_NUM_USED is defined at stm32f10x_Systime_Beat.h, can be 1 to Systime_Beat_NUM_MAX, 
	*        and Systime_Beat_NUM_MAX is also defined at control.h, can be 1 to 2.
  * @param Systime_Beat_InitStruct : pointer to a Systime_Beat_InitTypeDef_Struct
  *   structure which will be initialized.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus SysTime_Beat_Init(SysTime_Beat_TypeDef_Struct* SysTime_Beatx, SysTime_Beat_InitTypeDef_Struct* SysTime_Beatx_InitStruct);

/**
  * @brief  Enable the specified Systime_Beat peripheral.
  * @param Systime_Beatx: where x can be  1 to Systime_Beat_NUM_USED  to select the Systime_Beat.
  *        Systime_Beat_NUM_USED is defined at control.h, can be 1 to Systime_Beat_NUM_MAX, 
	*        and Systime_Beat_NUM_MAX is also defined at control.h, can be 1 to 2.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus Systime_Beat_On(SysTime_Beat_TypeDef_Struct* SysTime_Beatx);

/**
  * @brief Systime_Beat interrupt service routine 
	         should be called by specific TIM interrupt service function.
  * @param SysTime_Beatx: where x can be  1 to SysTime_Beat_NUM_USED  to select the SysTime_Beat.
  *        SysTime_Beat_NUM_USED is defined at control.h, can be 1 to Systime_Beat_NUM_MAX, 
	*        and Systime_Beat_NUM_MAX is also defined at control.h, can be 1 to 2.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus SysTime_Beat_IRQHandler(SysTime_Beat_TypeDef_Struct* SysTime_Beatx);

/**************************************************************************
函数功能：增量PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_A (int Encoder,int Target);
int Incremental_PI_B (int Encoder,int Target);

/**************************************************************************
函数功能：位置式PID控制器

返回  值：servo control value
根据位置式离散PID公式 
Angle=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  
∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;

**************************************************************************/
float Position_PID (CURRENT_OPENMVDATA_Struct *pdata);

/**************************************************************************
函数功能：小车运动数学模型
入口参数：速度和转角
返回  值：无
**************************************************************************/
void Kinematic_Analysis(float velocity,float angle);

void Xianfu_Pwm(void);

void Set_Pwm(int Motor_A,int Motor_B,int Servo);

void Drive_GPIO_Init(void);
/**
  * @}
  */

#endif /*__STM32F10X_CONTROL_H */

