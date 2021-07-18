#include  "stm32f10x_pwm.h"

/*-----------------------------Hardware_Table_Derition--------------------------------
* Servo Drive: 
*				PWM_TIMx;
*				PWM_TIM_Channel;
*				GPIOx;
*				GPIO_Pin;
* Motor Drive:
*       PWM supported by Advanced Timer : TIM1 and TIM8 
*       function: 
*				deadtime control and two ways relatively complemented channel pwm
*				PWM_TIMx;
*				PWM_TIM_Channel *related to PWM_TIM_ChannelN;
*				GPIOx;;				
*				*GPIOx; 				*mapping ChannelN
*				GPIO_Pin1;
*				GPIO_Pin2;
*	      brake channel can stop a vehicle by applying the brake;
*------------------------------------------------------------------------------------*/
 
/* --------------------------------- DrivePWM Hardware ----------------------------------- */
DrivePWM_Hardware_TypeDef_Struct DrivePWM_Hardware[] = 
{
	#ifdef DrivePWM1
			{ TIM8,TIM_Channel_1,GPIOC,GPIO_Pin_6,GPIOA,GPIO_Pin_7}, 
	#endif

	#ifdef DrivePWM2
			{ TIM8,TIM_Channel_2,GPIOC,GPIO_Pin_7,GPIOB,GPIO_Pin_0}, 
	#endif

	#ifdef DrivePWM3
			{ TIM8,TIM_Channel_3,GPIOC,GPIO_Pin_8,GPIOB,GPIO_Pin_1}, 
	#endif
			
//	#ifdef BrakePWM4
//			{ TIM8,GPIOA,GPIO_Pin_6}, 
//	#endif

};

/* --------------------------------- ServoPWM Hardware ----------------------------------- */

ServoPWM_Hardware_TypeDef_Struct ServoPWM_Hardware[] = 
{
	#ifdef ServoPWM1
			{ TIM3,TIM_Channel_1,GPIOA,GPIO_Pin_6,}, 
	#endif

	#ifdef ServoPWM2
			{ TIM3,TIM_Channel_2,GPIOA,GPIO_Pin_7,}, 
	#endif

	#ifdef ServoPWM3
			{ TIM3,TIM_Channel_3,GPIOB,GPIO_Pin_0,}, 
	#endif

	#ifdef ServoPWM4
			{ TIM3,TIM_Channel_4,GPIOB,GPIO_Pin_1,}, 
	#endif

};