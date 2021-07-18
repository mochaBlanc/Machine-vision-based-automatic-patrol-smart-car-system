#ifndef __STM32F10X_PWM_H
#define __STM32F10X_PWM_H
/******************************************************************************
*                                                                             *
* @file     stm32f10x_pwm.c                                                   *
* @version  V1.1.0                                                            *
* @date     24/DEC/2018
* @author   J.F.YUE
* @brief    This file provides PWM drive functions for servo control and 
 producing two complementary PWM wave with dead-time by using advanced timer  *	
*                                                                             *
******************************************************************************/ 

/*------------------ Define to prevent recursive inclusion ------------------*/

/* Includes ------------------------------------------------------------------*/
#include <stm32f10x.h>
#include "string.h"
#include "stm32f10x_tim.h"

typedef enum __PWM_Polarity_ENUM
{
  PWM_OCPolarity_High  = 0x0000,
  PWM_OCPolarity_Low	 = 0x0002,
}PWM_Polarity_ENUM;

typedef enum __PWM_OCmode_ENUM
{
	PWM_OCMode_PWM1         = 0x0060,
	PWM_OCMode_PWM2         = 0x0070,
}PWM_OCmode_ENUM;

//与死区时间分频有关
typedef enum __PWM_ClockDivision_ENUM
{
	PWM_CKD_DIV1         = 0x0000,
	PWM_CKD_DIV2         = 0x0100,
}PWM_ClockDivision_ENUM;
/**
  * @brief Configuration CounterMode enumration
  */
typedef enum __PWM_CounterMode_ENUM
{
	PWM_CounterMode_Up               =	0x0000,
	PWM_CounterMode_Down             =  0x0010,
	PWM_CounterMode_CenterAligned1   =  0x0020,
	PWM_CounterMode_CenterAligned2   =  0x0040,
	PWM_CounterMode_CenterAligned3   =  0x0060,
}PWM_CounterMode_ENUM;



/** 
  * @brief  ServoPWM Memory structure definition  
  */

typedef struct __ServoPWM_Memory_TypeDef_Struct
{
	PWM_Polarity_ENUM			PWM_Pola;
	PWM_OCmode_ENUM				PWM_OCmode;
	PWM_CounterMode_ENUM	PWM_CountM;
	uint16_t          		ServoPWM_Arr;
	uint16_t          		ServoPWM_Psc;
	uint16_t          		ServoPWM_Pulse;
	
}ServoPWM_Memory_TypeDef_Struct;
/** 
  * @brief  ServoPWM Hardware structure definition  
  */

typedef struct __ServoPWM_Hardware_TypeDef_Struct
{
	TIM_TypeDef      *TIMx;
	uint16_t          TIM_Channel;
	GPIO_TypeDef			*GPIOx;
	uint16_t          GPIO_Pin;
	
}ServoPWM_Hardware_TypeDef_Struct;

/** 
  * @brief  ServoPWM Init structure definition  
  */

typedef struct __ServoPWM_InitTypeDef_Struct
{
	PWM_Polarity_ENUM			PWM_Pola;
	PWM_OCmode_ENUM				PWM_OCmode;
	PWM_CounterMode_ENUM	PWM_CountM;
	uint16_t          		ServoPWM_Arr;
	uint16_t          		ServoPWM_Psc;
	uint16_t          		ServoPWM_Pulse;
}ServoPWM_InitTypeDef_Struct;


/** 
  * @brief  ServoPWM Type structure definition  
  */

typedef struct __ServoPWM_TypeDef_Struct
{
	ServoPWM_Hardware_TypeDef_Struct *ServoPWM_Hardware;
	ServoPWM_Memory_TypeDef_Struct   ServoPWM_OperatingParameter;
}ServoPWM_TypeDef_Struct;


/** 
  * @brief  DrivePWM Hardware structure definition  
  */

typedef struct __DrivePWM_Hardware_TypeDef_Struct
{
	TIM_TypeDef      *TIMx;
	uint16_t          TIM_Channel;
	GPIO_TypeDef			*GPIOx;
	uint16_t          GPIO_Pin;
	GPIO_TypeDef			*GPIOxN;
	uint16_t          GPIO_PinN;
		
}DrivePWM_Hardware_TypeDef_Struct;


/** 
  * @brief  DrivePWM Memory structure definition  
  */

typedef struct __DrivePWM_Memory_TypeDef_Struct
{
	PWM_Polarity_ENUM			PWM_Pola;
	PWM_OCmode_ENUM				PWM_OCmode;
	PWM_CounterMode_ENUM	PWM_CountM;
	PWM_ClockDivision_ENUM PWM_CKD_DIV;
	uint16_t          		DrivePWM_Arr;
	uint16_t          		DrivePWM_Psc;
	uint16_t          		DrivePWM_Pulse;
	
}DrivePWM_Memory_TypeDef_Struct;

/** 
  * @brief  DrivePWM Type structure definition  
  */

typedef struct __DrivePWM_TypeDef_Struct
{
	DrivePWM_Hardware_TypeDef_Struct *DrivePWM_Hardware;
	DrivePWM_Memory_TypeDef_Struct   DrivePWM_OperatingParameter;
}DrivePWM_TypeDef_Struct;

/** 
  * @brief  DrivePWM Init structure definition  
  */

typedef struct __DrivePWM_InitTypeDef_Struct
{
	PWM_Polarity_ENUM			PWM_Pola;
	PWM_OCmode_ENUM				PWM_OCmode;
	PWM_CounterMode_ENUM	PWM_CountM;
	PWM_ClockDivision_ENUM PWM_CKD_DIV;
	uint16_t          		DrivePWM_Arr;
	uint16_t          		DrivePWM_Psc;
	uint16_t          		DrivePWM_Pulse;
}DrivePWM_InitTypeDef_Struct;




/* ------------------------------- ServoPWM_nums_define --------------------------------- */
/* It is not permitted to modify ServoPWM_NUM_MAX greater than 4 */
#define ServoPWM_NUM_MAX                           4 
/* It is not permitted to modify ServoPWM_NUM_USED greater than ServoPWM_NUM_MAX */
#define ServoPWM_NUM_USED                          4

#if(ServoPWM_NUM_MAX>4)
#error ServoPWM_NUM_MAX GREATER THAN 4
#endif

#if(ServoPWM_NUM_USED>ServoPWM_NUM_MAX)
#error ServoPWM_NUM_USED GREATER THAN ServoPWM_NUM_MAX
#endif

#if(ServoPWM_NUM_USED>=1)
extern ServoPWM_TypeDef_Struct ServoPWM1_BASE;
#define ServoPWM1                       							(&ServoPWM1_BASE)      /* ServoPWM 1 selected */
#else
#define ServoPWM1                        		 					NULL             /* ServoPWM 1 not selected */
#endif

#if(ServoPWM_NUM_USED>=2)
extern ServoPWM_TypeDef_Struct ServoPWM2_BASE;
#define ServoPWM2                       							(&ServoPWM2_BASE)      /* ServoPWM 2 selected */
#else
#define ServoPWM2                        		 					NULL             /* ServoPWM 2 not selected */
#endif

#if(ServoPWM_NUM_USED>=3)
extern ServoPWM_TypeDef_Struct ServoPWM3_BASE;
#define ServoPWM3                      							(&ServoPWM3_BASE)      /* ServoPWM 3 selected */
#else
#define ServoPWM3                       		 					NULL             /* ServoPWM 3 not selected */
#endif

#if(ServoPWM_NUM_USED>=4)
extern ServoPWM_TypeDef_Struct ServoPWM4_BASE;
#define ServoPWM4                      							(&ServoPWM4_BASE)      /* ServoPWM 4 selected */
#else
#define ServoPWM4                       		 					NULL             /* ServoPWM 4 not selected */
#endif



/* ------------------------------- DrivePWM_nums_define --------------------------------- */
/* It is not permitted to modify DrivePWM_NUM_MAX greater than 4 */
#define DrivePWM_NUM_MAX                           3 
/* It is not permitted to modify DrivePWM_NUM_USED greater than DrivePWM_NUM_MAX */
#define DrivePWM_NUM_USED                          2

#if(DrivePWM_NUM_MAX>3)
#error DrivePWM_NUM_MAX GREATER THAN 3
#endif

#if(DrivePWM_NUM_USED>DrivePWM_NUM_MAX)
#error DrivePWM_NUM_USED GREATER THAN DrivePWM_NUM_MAX
#endif

#if(DrivePWM_NUM_USED>=1)
extern DrivePWM_TypeDef_Struct DrivePWM1_BASE;
#define DrivePWM1                       							(&DrivePWM1_BASE)      /* DrivePWM 1 selected */
#else
#define DrivePWM1                        		 					NULL             /* DrivePWM 1 not selected */
#endif

#if(DrivePWM_NUM_USED>=2)
extern DrivePWM_TypeDef_Struct DrivePWM2_BASE;
#define DrivePWM2                       							(&DrivePWM2_BASE)      /* DrivePWM 2 selected */
#else
#define DrivePWM2                        		 					NULL             /* DrivePWM 2 not selected */
#endif

#if(DrivePWM_NUM_USED>=3)
extern DrivePWM_TypeDef_Struct DrivePWM3_BASE;
#define DrivePWM3                      							(&DrivePWM3_BASE)      /* DrivePWM 3 selected */
#else
#define DrivePWM3                       		 					NULL             /* DrivePWM 3 not selected */
#endif



/* ------------------------------ System_Clock_define ------------------------------- */
#define PWM_SYS_CLOCK                          72000000                   /* unit: Hz */



/* ------------------------------ServoPWM_InitTypeDef_Struct Default_Value define ------------------------------- */
  /* servo pwm 50HZ 20ms
	 servo control table  
	 0.5ms--------------0;          

   1.0ms------------45;           

   1.5ms------------90;          

   2.0ms-----------135;           

   2.5ms-----------180*/

#define ServoPWM_POLARITY_DEFAULT                   PWM_OCPolarity_High
#define ServoPWM_COUNTERMODE_DEFAULT                PWM_CounterMode_Up
#define ServoPWM_OCMODE_DEFAULT                     PWM_OCMode_PWM1
#define ServoPWM_PULSE_DEFAULT                      1500											/* unit: us */
#define ServoPWM_ARR_DEFAULT                        20000                     /* unit: us */

/* ------------------------------DrivePWM by Advanced timer_InitTypeDef_Struct Default_Value define ------------------------------- */
#define DrivePWM_POLARITY_DEFAULT                   PWM_OCPolarity_High
#define DrivePWM_COUNTERMODE_DEFAULT                PWM_CounterMode_Up
#define DrivePWM_OCMODE_DEFAULT                     PWM_OCMode_PWM1
#define DrivePWM_DUTY_DEFAULT                       20											   /* unit: Hz */
#define DrivePWM_FREQ_DEFAULT                       1000                     /* unit: Hz */
#define DrivePWM_CLOCKDIVISION_DEFAULT 							PWM_CKD_DIV1 
#define DrivePWM_DEADTIME_DEFAULT 									205 // 72:1us 172:3us 205:5us

/* ----------------------------- PWM_InitParameter_Limitation ------------------------------- */

#define DrivePWM_MAXDUTY												80
#define DrivePWM_MINDUTY												1
#define DrivePWM_FREQ_MIN											  1						/*unit:Hz */
#define DrivePWM_FREQ_MAX											  1100			  /*unit:Hz */

#define ServoPWM_PULSE_MAX 											2000         /* unit: us */
#define ServoPWM_PULSE_MIN                      1000					/* unit: us */

#if(DrivePWM_FREQ_DEFAULT>DrivePWM_FREQ_MAX)
#error DrivePWM_FREQ IS GREATER THAN DrivePWM_FREQ_MAX
#endif

#if(DrivePWM_FREQ_DEFAULT<DrivePWM_FREQ_MIN)
#error DrivePWM_FREQ IS LESS THAN DrivePWM_FREQ_MIN
#endif

#if(DrivePWM_DUTY_DEFAULT>DrivePWM_MAXDUTY)
#error DrivePWM_DUTY IS GREATER THAN DrivePWM_MAXDUTY
#endif

#if(DrivePWM_DUTY_DEFAULT>DrivePWM_MAXDUTY)
#error DrivePWM_DUTY IS LESS THAN DrivePWM_MINDUTY
#endif

#if(ServoPWM_PULSE_DEFAULT<ServoPWM_PULSE_MIN)
#error ServoPWM_PULSE IS LESS THAN ServoPWM_PULSE_MIN
#endif

#if(ServoPWM_PULSE_DEFAULT>ServoPWM_PULSE_MAX)
#error ServoPWM_PULSE IS GREATER ServoPWM_PULSE_MAX
#endif



/**
  * @}
  */

/**
  * @brief  Fills each ServoPWM_InitStruct member with its default value.
  * @param ServoPWM_InitStruct : pointer to a ServoPWM_InitTypeDef
  *   structure which will be initialized.
  * @retval : None
  */
void ServoPWM_StructInit(ServoPWM_InitTypeDef_Struct* ServoPWMx_InitStruct);


/**
  * @brief  Initializes the ServoPWMx  according to the specified
  *   parameters in the PWM_OutInitStruct.
  * @param ServoPWMx: where x can be  1 to ServoPWM_NUM_USED  to select the ServoPWM.
  *        ServoPWM_NUM_USED is defined at stm32f10x_pwm.h, can be 1 to ServoPWM_NUM_MAX, 
	*        and ServoPWM_NUM_MAX is also defined at stm32f10x_pwm.h, can be 1 to 4.
  * @param PWM_InitStruct : pointer to a PWM_InitTypeDef
  *   structure which will be initialized.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus ServoPWM_Init(ServoPWM_TypeDef_Struct* ServoPWMx, ServoPWM_InitTypeDef_Struct* ServoPWMx_InitStruct);


/**
  * @brief  Enable the specified Advance ServoPWM peripheral.
  * @param PWMx: where x can be  1 to ServoPWM_NUM_USED to select the ServoPWMx peripheral.
  *        And at this time, the ServoPWM_NUM_MAX and ServoPWM_NUM_USED which is defined at stm32f10x_pwm.h
  *        must equal to x or be greater than x.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus ServoPWM_On(ServoPWM_TypeDef_Struct* ServoPWMx);

/**
  * @brief  Disables the specified Advance ServoPWM peripheral.
  * @param ServoPWMx: where x can be  1 to ServoPWM_NUM_USED to select the ServoPWMx peripheral.
  *        And at this time, the ServoPWM_NUM_MAX and ServoPWM_NUM_USED which is defined at stm32f10x_pwm.h
  *        must equal to x or be greater than x.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus ServoPWM_Off(ServoPWM_TypeDef_Struct* ServoPWMx);

/**
  * @brief   Setting the PULSE of ServoPWMx.
  * @param ServoPWMx: where x can be  1 to ServoPWM_NUM_USED to select the PWM.
	*        ServoPWM_NUM_USED is defined at stm32f10x_pwm.h, can be 1 to PWM_NUM_MAX, 
	*        and ServoPWM_NUM_MAX is also defined at stm32f10x_pwm.h, can be 1 to 4.
  * @param PWM_Frequency: frequency of ServoPWMx (unit: Hz).
	* @param Prescaler: Prescaler of PWMx->TIMx.
	* @param PWM_CounterMode: CounterMode of PWMx->TIMx.
	*        This parameter can be PWM_CounterMode_Up, PWM_CounterMode_Down, PWM_CounterMode_CenterAligned1,
	*        PWM_CounterMode_CenterAligned2, PWM_CounterMode_CenterAligned3.
  * @retval : ERROR or SUCCESS.
  */	
ErrorStatus ServoPWM_PULSE_Setting(ServoPWM_TypeDef_Struct *ServoPWMx, uint16_t ServoPWM_Pulse);

/**
  * @brief ServoGPIOconfiguration
  */ 	
ErrorStatus ServoPWM_GPIOConfig(ServoPWM_TypeDef_Struct *ServoPWMx);



/**
  * @brief  Fills each DrivePWM_InitStruct member with its default value.
  * @param DrivePWM_InitStruct : pointer to a DrivePWM_InitTypeDef
  *   structure which will be initialized.
  * @retval : None
  */
void DrivePWM_StructInit(DrivePWM_InitTypeDef_Struct* DrivePWMx_InitStruct);


/**
  * @brief  Initializes the DrivePWMx  according to the specified
  *   parameters in the PWM_OutInitStruct.
  * @param DrivePWMx: where x can be  1 to DrivePWM_NUM_USED  to select the DrivePWM.
  *        DrivePWM_NUM_USED is defined at stm32f10x_pwm.h, can be 1 to DrivePWM_NUM_MAX, 
	*        and DrivePWM_NUM_MAX is also defined at stm32f10x_pwm.h, can can be 1 to 3.
  * @param PWM_InitStruct : pointer to a PWM_InitTypeDef
  *   structure which will be initialized.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus DrivePWM_Init(DrivePWM_TypeDef_Struct* DrivePWMx, DrivePWM_InitTypeDef_Struct* DrivePWMx_InitStruct);


/**
  * @brief  Enable the specified Advance DrivePWM peripheral.
  * @param DrivePWMPWMx: where x can be  1 to DrivePWM_NUM_USED to select the DrivePWMx peripheral.
  *        And at this time, the DrivePWM_NUM_MAX and DrivePWM_NUM_USED which is defined at stm32f10x_pwm.h
  *        must equal to x or be greater than x.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus DrivePWM_On(DrivePWM_TypeDef_Struct* DrivePWMx);

/**
  * @brief  Disables the specified Advance DrivePWM peripheral.
  * @param DrivePWMx: where x can be  1 to DrivePWM_NUM_USED to select the DrivePWMx peripheral.
  *        And at this time, the DrivePWM_NUM_MAX and DrivePWM_NUM_USED which is defined at stm32f10x_pwm.h
  *        must equal to x or be greater than x.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus DrivePWM_Off(DrivePWM_TypeDef_Struct* DrivePWMx);
/**
  * @brief  Control the polarity of DrivePWMx.
  * @param DrivePWMx: where x can be 1 to DrivePWM_NUM_MAX to select the DrivePWMx peripheral.
  *        And at this time, the DrivePWM_NUM_MAX and DrivePWM_NUM_USED which is defined at stm32f10x_pwm.h
  *        must equal to x or be greater than x.
  * @param DrivePWM_Polarity: new polarity of the DrivePWMx .
  *   This parameter can be: DrivePWM_OCPolarity_High or DrivePWM_OCPolarity_Low.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus DrivePWM_PolarityCtrl(DrivePWM_TypeDef_Struct* DrivePWMx, PWM_Polarity_ENUM PWM_Polarity);

/**
  * @brief   Setting the duty of DrivePWMx.
  * @param DrivePWMx: where x can be  1 to DrivePWM_NUM_USED to select the PWM.
	*        DrivePWM_NUM_USED is defined at stm32f10x_pwm.h, can be 1 to PWM_NUM_MAX, 
	*        and DrivePWM_NUM_MAX is also defined at stm32f10x_pwm.h, can be 1 to 3.
  * @param PWM_Frequency: frequency of DrivePWMx (unit: Hz).
	* @param Prescaler: Prescaler of PWMx->TIMx.
	* @param PWM_CounterMode: CounterMode of PWMx->TIMx.
	*        This parameter can be PWM_CounterMode_Up, PWM_CounterMode_Down, PWM_CounterMode_CenterAligned1,
	*        PWM_CounterMode_CenterAligned2, PWM_CounterMode_CenterAligned3.
  * @retval : ERROR or SUCCESS.
  */	
ErrorStatus DrivePWM_Duty_Setting(DrivePWM_TypeDef_Struct *DrivePWMx, uint16_t DrivePWM_Duty);

/**
  * @brief DrivePWM GPIOconfiguration
  */ 	
ErrorStatus DrivePWM_GPIOConfig(DrivePWM_TypeDef_Struct *DrivePWMx);


	void TIM8_Mode_Config(void);
	void TIM8_GPIO_Config(void) ;

/**
  * @}
  */ 	

#endif /*__STM32F10X_PWM_H */


/**
  * @}
  */