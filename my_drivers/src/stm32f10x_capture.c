 #include "stm32f10x_capture.h"
 #include "stm32f10x_encoder.h"


 void Tim1_Configuration(void) //ETR capture frequency
{  
 		
	GPIO_InitTypeDef GPIO_InitStructure; 
 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
 GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
 TIM_TimeBaseStructure.TIM_Prescaler =0; 
 TIM_TimeBaseStructure.TIM_Period = 0xFFFF; 
 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
 TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);  // Time base configuration 

 TIM_ETRClockMode2Config(TIM1, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);  
 //TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);   
 TIM_SetCounter(TIM1, 0);    

 TIM_Cmd(TIM1, ENABLE); 
}






