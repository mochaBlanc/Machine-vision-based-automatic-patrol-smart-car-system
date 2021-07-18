#include "stm32f10x_pwm.h"

extern DrivePWM_Hardware_TypeDef_Struct DrivePWM_Hardware[];
extern ServoPWM_Hardware_TypeDef_Struct ServoPWM_Hardware[];


ServoPWM_TypeDef_Struct                    ServoPWM1_BASE,ServoPWM2_BASE,ServoPWM3_BASE,ServoPWM4_BASE;

DrivePWM_TypeDef_Struct              		   DrivePWM1_BASE,DrivePWM2_BASE,DrivePWM3_BASE;

/* ------------------------------- Part_ServoPWM for servo & common pwm --------------------------------- */
void ServoPWM_StructInit(ServoPWM_InitTypeDef_Struct* ServoPWMx_InitStruct)
{
	uint16_t          		arr;
	uint16_t          		psc;
	uint16_t          		pulse;
	
	if(ServoPWM_ARR_DEFAULT>9999)
		psc = 71;
	else
		psc	=	7199;
	
	switch(ServoPWM_COUNTERMODE_DEFAULT)
	{
		case PWM_CounterMode_Up:
		case PWM_CounterMode_Down:
			arr=ServoPWM_ARR_DEFAULT;
		break;				
		default:	break;
	}	
	pulse=ServoPWM_PULSE_DEFAULT;
	
	ServoPWMx_InitStruct->PWM_CountM = ServoPWM_COUNTERMODE_DEFAULT ;
	ServoPWMx_InitStruct->PWM_OCmode = ServoPWM_OCMODE_DEFAULT;
	ServoPWMx_InitStruct->PWM_Pola   =	ServoPWM_POLARITY_DEFAULT; 
	ServoPWMx_InitStruct->ServoPWM_Arr		= arr;
	ServoPWMx_InitStruct->ServoPWM_Psc		=	psc;
	ServoPWMx_InitStruct->ServoPWM_Pulse	= pulse;
}


                                     
ErrorStatus ServoPWM_Init(ServoPWM_TypeDef_Struct*  ServoPWMx,	ServoPWM_InitTypeDef_Struct*	ServoPWMx_InitStruct)
{
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef					TIM_OCInitStructure;
	
	if(ServoPWMx	==	NULL)					return ERROR;
	
	/*PWM_BASE->PWM_Hardware Configuration */
	
	if(ServoPWMx==ServoPWM1)
	{
		ServoPWM1_BASE.ServoPWM_Hardware	=	&ServoPWM_Hardware[0];
	}
	else if(ServoPWMx==ServoPWM2)
	{
		ServoPWM2_BASE.ServoPWM_Hardware	=	&ServoPWM_Hardware[1];
	}
	else if(ServoPWMx==ServoPWM3)
	{
		ServoPWM3_BASE.ServoPWM_Hardware	=	&ServoPWM_Hardware[2];
	}
	else
	{
		ServoPWM4_BASE.ServoPWM_Hardware	=	&ServoPWM_Hardware[3];
	}
	

	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period					=			ServoPWMx_InitStruct->ServoPWM_Arr;
	TIM_TimeBaseStructure.TIM_Prescaler				=			ServoPWMx_InitStruct->ServoPWM_Psc;
	TIM_TimeBaseStructure.TIM_CounterMode			=			ServoPWMx_InitStruct->PWM_CountM;
	
	TIM_TimeBaseInit(ServoPWMx->ServoPWM_Hardware->TIMx,&TIM_TimeBaseStructure);
	
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode						=			ServoPWMx_InitStruct->PWM_OCmode;
	TIM_OCInitStructure.TIM_OutputState				=			TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse							=			ServoPWMx_InitStruct->ServoPWM_Pulse;

	
	switch (ServoPWMx_InitStruct->PWM_Pola)
{
	case	PWM_OCPolarity_High:
		TIM_OCInitStructure.TIM_OCPolarity			=			TIM_OCPolarity_High;
		break;
	case	PWM_OCPolarity_Low:
		TIM_OCInitStructure.TIM_OCPolarity			=			TIM_OCPolarity_Low;
		break;
	default:
		break;
}

	switch (ServoPWMx->ServoPWM_Hardware->TIM_Channel)
	{
		case	TIM_Channel_1:
			TIM_OC1Init(ServoPWMx->ServoPWM_Hardware->TIMx,&TIM_OCInitStructure);
			TIM_OC1PreloadConfig(ServoPWMx->ServoPWM_Hardware->TIMx,TIM_OCPreload_Enable);
			break;
		case	TIM_Channel_2:
			TIM_OC2Init(ServoPWMx->ServoPWM_Hardware->TIMx,&TIM_OCInitStructure);
			TIM_OC2PreloadConfig(ServoPWMx->ServoPWM_Hardware->TIMx,TIM_OCPreload_Enable);
			break;
		case	TIM_Channel_3:
			TIM_OC3Init(ServoPWMx->ServoPWM_Hardware->TIMx,&TIM_OCInitStructure);
			TIM_OC3PreloadConfig(ServoPWMx->ServoPWM_Hardware->TIMx,TIM_OCPreload_Enable);
			break;
		case	TIM_Channel_4:
			TIM_OC4Init(ServoPWMx->ServoPWM_Hardware->TIMx,&TIM_OCInitStructure);
			TIM_OC4PreloadConfig(ServoPWMx->ServoPWM_Hardware->TIMx,TIM_OCPreload_Enable);
			break;
		
		default:
			break;
	}
	TIM_ARRPreloadConfig(ServoPWMx->ServoPWM_Hardware->TIMx,ENABLE);
	
	ServoPWM_GPIOConfig(ServoPWMx);
	
	ServoPWMx->ServoPWM_OperatingParameter.ServoPWM_Arr		=		ServoPWMx_InitStruct->ServoPWM_Arr;
	ServoPWMx->ServoPWM_OperatingParameter.ServoPWM_Psc		=		ServoPWMx_InitStruct->ServoPWM_Psc;
	ServoPWMx->ServoPWM_OperatingParameter.ServoPWM_Pulse	=		ServoPWMx_InitStruct->ServoPWM_Pulse;

	
	return SUCCESS;
}

ErrorStatus ServoPWM_PULSE_Setting(ServoPWM_TypeDef_Struct *ServoPWMx, uint16_t ServoPWM_Pulse)
{
	
	if(ServoPWMx	==NULL)		return ERROR;
	
	if(ServoPWM_Pulse	>	ServoPWM_PULSE_MAX)	ServoPWM_Pulse	=		ServoPWM_PULSE_MAX;
	if(ServoPWM_Pulse <	ServoPWM_PULSE_MIN)	ServoPWM_Pulse	=		ServoPWM_PULSE_MIN;
	
		
	ServoPWMx->ServoPWM_OperatingParameter.ServoPWM_Pulse		=		ServoPWM_Pulse;
		
		switch (ServoPWMx->ServoPWM_Hardware->TIM_Channel)
	{
		case		TIM_Channel_1:
			TIM_SetCompare1(ServoPWMx->ServoPWM_Hardware->TIMx,ServoPWM_Pulse);
			break;
		case		TIM_Channel_2:
			TIM_SetCompare2(ServoPWMx->ServoPWM_Hardware->TIMx,ServoPWM_Pulse);
		case		TIM_Channel_3:
			TIM_SetCompare3(ServoPWMx->ServoPWM_Hardware->TIMx,ServoPWM_Pulse);
		case		TIM_Channel_4:
			TIM_SetCompare4(ServoPWMx->ServoPWM_Hardware->TIMx,ServoPWM_Pulse);
		default:
			break;
	}
		
	return SUCCESS;

}
ErrorStatus ServoPWM_GPIOConfig(ServoPWM_TypeDef_Struct *ServoPWMx)
{
	GPIO_InitTypeDef 		GPIO_InitStructure;
	
	if(ServoPWMx	==	NULL)		return ERROR;
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin		=	ServoPWMx->ServoPWM_Hardware->GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_AF_PP;
	
	GPIO_Init(ServoPWMx->ServoPWM_Hardware->GPIOx,&GPIO_InitStructure);
	
	return SUCCESS;
}

ErrorStatus ServoPWM_On(ServoPWM_TypeDef_Struct* ServoPWMx)
{
	if(ServoPWMx	==	NULL)		return ERROR;
	
	TIM_Cmd(ServoPWMx->ServoPWM_Hardware->TIMx,ENABLE);
	
	TIM_CCxCmd(ServoPWMx->ServoPWM_Hardware->TIMx,ServoPWMx->ServoPWM_Hardware->TIM_Channel,ENABLE);
	
	return SUCCESS;
}

ErrorStatus ServoPWM_Off(ServoPWM_TypeDef_Struct* ServoPWMx)
{
	if(ServoPWMx	==	NULL)		return ERROR;
	
	
	TIM_CCxCmd(ServoPWMx->ServoPWM_Hardware->TIMx,ServoPWMx->ServoPWM_Hardware->TIM_Channel,TIM_CCx_Disable);
	
	return SUCCESS;
}

/* ------------------------------- Part_DrivePWM for motor & deadtime control--------------------------------- */
void DrivePWM_StructInit(DrivePWM_InitTypeDef_Struct* DrivePWMx_InitStruct)
{
	uint16_t          		arr;
	uint16_t          		psc;
	uint16_t          		pulse;
	
	if(DrivePWM_FREQ_DEFAULT>=2000)
		psc	=	1;
	else 
		psc	=	71;
	
	switch(DrivePWM_COUNTERMODE_DEFAULT)
	{
		case PWM_CounterMode_Up:
		case PWM_CounterMode_Down:
			arr=(int)(PWM_SYS_CLOCK/DrivePWM_FREQ_DEFAULT/(psc+1)+0.5);
		break;
		
		case PWM_CounterMode_CenterAligned1:
		case PWM_CounterMode_CenterAligned2:
		case PWM_CounterMode_CenterAligned3:
			arr=(int)(PWM_SYS_CLOCK/(2*DrivePWM_FREQ_DEFAULT)/(psc+1)+0.5);
		break;
		
		default:	break;
	}
	
	pulse=(int)(DrivePWM_DUTY_DEFAULT*arr/100+0.5);
	
	DrivePWMx_InitStruct->PWM_CountM			=		DrivePWM_COUNTERMODE_DEFAULT;
	DrivePWMx_InitStruct->PWM_OCmode			=		DrivePWM_OCMODE_DEFAULT;
	DrivePWMx_InitStruct->PWM_Pola				=		DrivePWM_POLARITY_DEFAULT;
	DrivePWMx_InitStruct->PWM_CKD_DIV			=		DrivePWM_CLOCKDIVISION_DEFAULT;
	DrivePWMx_InitStruct->DrivePWM_Arr		=   arr-1;
	DrivePWMx_InitStruct->DrivePWM_Psc		=		psc;
	DrivePWMx_InitStruct->DrivePWM_Pulse	=		pulse;
}


ErrorStatus DrivePWM_Init(DrivePWM_TypeDef_Struct*  DrivePWMx,	DrivePWM_InitTypeDef_Struct*	DrivePWMx_InitStruct)
{
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef					TIM_OCInitStructure;
	TIM_BDTRInitTypeDef       TIM_BDTRInitStruct;
	
	if(DrivePWMx	==	NULL)					return ERROR;
	
	/*PWM_BASE->PWM_Hardware Configuration */
	
	if(DrivePWMx==DrivePWM1)
	{
		DrivePWM1_BASE.DrivePWM_Hardware	=	&DrivePWM_Hardware[0];
	}
	else if(DrivePWMx==DrivePWM2)
	{
		DrivePWM2_BASE.DrivePWM_Hardware	=	&DrivePWM_Hardware[1];
	}
	else
	{
		DrivePWM3_BASE.DrivePWM_Hardware	=	&DrivePWM_Hardware[2];
	}
	

	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period					=			DrivePWMx_InitStruct->DrivePWM_Arr;
	TIM_TimeBaseStructure.TIM_Prescaler				=			DrivePWMx_InitStruct->DrivePWM_Psc;
	TIM_TimeBaseStructure.TIM_CounterMode			=			DrivePWMx_InitStruct->PWM_CountM;
	TIM_TimeBaseStructure.TIM_ClockDivision   = 		DrivePWMx_InitStruct->PWM_CKD_DIV;
	
	TIM_TimeBaseInit(DrivePWMx->DrivePWM_Hardware->TIMx,&TIM_TimeBaseStructure);
	
	TIM_BDTRStructInit(&TIM_BDTRInitStruct);
	TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Disable;
  TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Disable;
  TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
  TIM_BDTRInitStruct.TIM_DeadTime  = DrivePWM_DEADTIME_DEFAULT; 
  TIM_BDTRConfig(DrivePWMx->DrivePWM_Hardware->TIMx,&TIM_BDTRInitStruct);
	
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode						=			DrivePWMx_InitStruct->PWM_OCmode;
	TIM_OCInitStructure.TIM_Pulse							=			DrivePWMx_InitStruct->DrivePWM_Pulse;
	TIM_OCInitStructure.TIM_OutputState				=			TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState 			= 		TIM_OutputNState_Enable;

  TIM_OCInitStructure.TIM_OCIdleState 			= 		TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState 			= 		TIM_OCIdleState_Reset;

	
	switch (DrivePWMx_InitStruct->PWM_Pola)
 {
	case	PWM_OCPolarity_High:
		TIM_OCInitStructure.TIM_OCPolarity			=			TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCNPolarity 		= 		TIM_OCNPolarity_High;
		break;
	case	PWM_OCPolarity_Low:
		TIM_OCInitStructure.TIM_OCPolarity			=			TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCNPolarity 		= 		TIM_OCNPolarity_Low;
		break;
	default:
		break;
 }
      
	switch (DrivePWMx->DrivePWM_Hardware->TIM_Channel)
	{
		case	TIM_Channel_1:
			TIM_OC1Init(DrivePWMx->DrivePWM_Hardware->TIMx,&TIM_OCInitStructure);
			TIM_OC1PreloadConfig(DrivePWMx->DrivePWM_Hardware->TIMx,TIM_OCPreload_Enable);
			break;
		case	TIM_Channel_2:
			TIM_OC2Init(DrivePWMx->DrivePWM_Hardware->TIMx,&TIM_OCInitStructure);
			TIM_OC2PreloadConfig(DrivePWMx->DrivePWM_Hardware->TIMx,TIM_OCPreload_Enable);
			break;
		case	TIM_Channel_3:
			TIM_OC3Init(DrivePWMx->DrivePWM_Hardware->TIMx,&TIM_OCInitStructure);
			TIM_OC3PreloadConfig(DrivePWMx->DrivePWM_Hardware->TIMx,TIM_OCPreload_Enable);
			break;
		
		default:
			break;
	}	
	TIM_ARRPreloadConfig(DrivePWMx->DrivePWM_Hardware->TIMx,ENABLE);
	
	DrivePWM_GPIOConfig(DrivePWMx);

	
	DrivePWMx->DrivePWM_OperatingParameter.DrivePWM_Arr		=		DrivePWMx_InitStruct->DrivePWM_Arr;
	DrivePWMx->DrivePWM_OperatingParameter.DrivePWM_Psc		=		DrivePWMx_InitStruct->DrivePWM_Psc;
	DrivePWMx->DrivePWM_OperatingParameter.DrivePWM_Pulse	=		DrivePWMx_InitStruct->DrivePWM_Pulse;
	DrivePWMx->DrivePWM_OperatingParameter.PWM_CKD_DIV		=		DrivePWMx_InitStruct->PWM_CKD_DIV;
	DrivePWMx->DrivePWM_OperatingParameter.PWM_Pola				=  	DrivePWMx_InitStruct->PWM_Pola;	

	
	return SUCCESS;
}



ErrorStatus DrivePWM_On(DrivePWM_TypeDef_Struct* DrivePWMx)
{	
	if(DrivePWMx	==	NULL)		return ERROR;
	
	TIM_Cmd(DrivePWMx->DrivePWM_Hardware->TIMx,ENABLE);
	TIM_CtrlPWMOutputs(DrivePWMx->DrivePWM_Hardware->TIMx,ENABLE);  
	TIM_CCxCmd(DrivePWMx->DrivePWM_Hardware->TIMx,DrivePWMx->DrivePWM_Hardware->TIM_Channel,ENABLE);
	
	return SUCCESS;
}




ErrorStatus DrivePWM_Off(DrivePWM_TypeDef_Struct* DrivePWMx)
{
	if(DrivePWMx	==	NULL)		return ERROR;
	TIM_CCxCmd(DrivePWMx->DrivePWM_Hardware->TIMx,DrivePWMx->DrivePWM_Hardware->TIM_Channel,DISABLE);
	
	return SUCCESS;
}


ErrorStatus DrivePWM_GPIOConfig(DrivePWM_TypeDef_Struct *DrivePWMx)
{
	GPIO_InitTypeDef 		GPIO_InitStructure;
	
	if(DrivePWMx	==	NULL)		return ERROR;	
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin		=	DrivePWMx->DrivePWM_Hardware->GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DrivePWMx->DrivePWM_Hardware->GPIOx,&GPIO_InitStructure);
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin		=	DrivePWMx->DrivePWM_Hardware->GPIO_PinN;
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DrivePWMx->DrivePWM_Hardware->GPIOxN,&GPIO_InitStructure);
		
	return SUCCESS;

}

ErrorStatus DrivePWM_Duty_Setting(DrivePWM_TypeDef_Struct *DrivePWMx, uint16_t DrivePWM_Duty)
{
	uint16_t pulse;
	
	if(DrivePWMx	==NULL)		return ERROR;
	
	if(DrivePWM_Duty	>	DrivePWM_MAXDUTY)	DrivePWM_Duty		=		DrivePWM_MAXDUTY;
	if(DrivePWM_Duty	<	DrivePWM_MINDUTY)	DrivePWM_Duty		=		DrivePWM_MINDUTY;
	
	pulse		=		(int)(DrivePWM_Duty*DrivePWMx->DrivePWM_OperatingParameter.DrivePWM_Arr/100+0.5);
		
	DrivePWMx->DrivePWM_OperatingParameter.DrivePWM_Pulse		=		pulse;
		
		switch (DrivePWMx->DrivePWM_Hardware->TIM_Channel)
	{
		case		TIM_Channel_1:
			TIM_SetCompare1(DrivePWMx->DrivePWM_Hardware->TIMx,pulse);
			break;
		case		TIM_Channel_2:
			TIM_SetCompare2(DrivePWMx->DrivePWM_Hardware->TIMx,pulse);
		case		TIM_Channel_3:
			TIM_SetCompare3(DrivePWMx->DrivePWM_Hardware->TIMx,pulse);
		case		TIM_Channel_4:
			TIM_SetCompare4(DrivePWMx->DrivePWM_Hardware->TIMx,pulse);
		default:
			break;
	}
		
	return SUCCESS;

}

ErrorStatus DrivePWM_PolarityCtrl(DrivePWM_TypeDef_Struct* DrivePWMx, PWM_Polarity_ENUM DrivePWM_Polarity)
{
	if(DrivePWMx	==	NULL)		return ERROR;
	
	DrivePWMx->DrivePWM_OperatingParameter.PWM_Pola	=	DrivePWM_Polarity;
	
		switch (DrivePWMx->DrivePWM_Hardware->TIM_Channel)
	{
		
		case	TIM_Channel_1:
			TIM_OC1PolarityConfig(DrivePWMx->DrivePWM_Hardware->TIMx,DrivePWM_Polarity);
			break;
		case	TIM_Channel_2:
			TIM_OC2PolarityConfig(DrivePWMx->DrivePWM_Hardware->TIMx,DrivePWM_Polarity);
			break;
		case	TIM_Channel_3:
			TIM_OC3PolarityConfig(DrivePWMx->DrivePWM_Hardware->TIMx,DrivePWM_Polarity);
			break;
		case	TIM_Channel_4:
			TIM_OC4PolarityConfig(DrivePWMx->DrivePWM_Hardware->TIMx,DrivePWM_Polarity);
			break;
		default:
			break;
	}
	return SUCCESS;
}
//   void TIM8_GPIO_Config(void) 
//  {
//    GPIO_InitTypeDef GPIO_InitStructure;
//  
//   /* TIM1 clock enable */
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
//  
//    /* GPIOA and GPIOB clock enable */
//   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);
//     
//   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 
//   GPIO_Init(GPIOC, &GPIO_InitStructure);
// 
//   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 
//   GPIO_Init(GPIOA, &GPIO_InitStructure);
// } 
//void TIM8_Mode_Config(void)
//{
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//    TIM_BDTRInitTypeDef      TIM8_BDTRInitStruct;
//    TIM_OCInitTypeDef        TIM_OCInitStructure;

//	
//	
//    /* Time base configuration */
//    TIM_TimeBaseStructure.TIM_Period = 1000-1; 
//    TIM_TimeBaseStructure.TIM_Prescaler = 72-1;
//    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
//    
//    /****** ??BDTR???,??????****************/
//    /*
//       ????? 72M   TIM_ClockDivision = TIM_CKD_DIV1?,  Tdts = 13.89ns
//       0 - 1.764us  ????
//       1.778us - 3.505us  ????
//       3.556us - 7.000us  ???? 
//       7.1117us - 14us    ????
//       ??????,??TIM_ClockDivision??
//    */
//    TIM8_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Disable;
//    TIM8_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Disable;
//    TIM8_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
//    TIM8_BDTRInitStruct.TIM_DeadTime = 205; // 72:1us 172:3us 205:5us
//    TIM_BDTRConfig(TIM8,&TIM8_BDTRInitStruct);
// 
////    TIM1->BDTR |= 72;   //????  ?:?????????,??????
//    
//     /* PWM1 Mode configuration: Channel2 */
//     TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
//     TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
//     TIM_OCInitStructure.TIM_Pulse =200-1;   //???,????
//     TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
//     TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
//     TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
//     TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
//     TIM_OC1Init(TIM8, &TIM_OCInitStructure);   
//     TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);      


//    TIM_ARRPreloadConfig(TIM8, ENABLE);

//    /* TIM1 enable counter */
//    TIM_Cmd(TIM8, ENABLE);
//    
//    TIM_CtrlPWMOutputs(TIM8, ENABLE);  
//}


