#include "stm32f10x_encoder.h"

/* --------------------------- ENC_BASE --------------------------- */
extern ENC_Hardware_TypeDef_Struct ENC_Hardware[];
ENC_TypeDef_Struct                    ENC1_BASE,ENC2_BASE,ENC3_BASE,ENC4_BASE;

void ENC_StructInit(ENC_InitTypeDef_Struct* ENCx_InitStruct)
{
	
	ENCx_InitStruct->ENC_ICpola			=		 ENC_IC_POLARITY_DEFAULT;                
  ENCx_InitStruct->ENC_CountM			=  	 ENC_COUNTERMODE_DEFAULT;                
  ENCx_InitStruct->ENC_mode				=  	 ENC_MODE_DEFAULT;   
	ENCx_InitStruct->ENC_Arr				=			0xFFFF;
	ENCx_InitStruct->ENC_Psc				=     0x0;
	ENCx_InitStruct->ENC_CKD				= 	 ENC_CKD_DIV1;
	ENCx_InitStruct->ENC_ICFilter   =			10;
}

ErrorStatus ENC_Init(ENC_TypeDef_Struct*	ENCx,	ENC_InitTypeDef_Struct*	ENCx_InitStruct)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_ICInitTypeDef 			TIM_ICInitStructure;
	
	if(ENCx	==	NULL)					return ERROR;
	
	/*ENC_BASE->ENC_Hardware Configuration */
	
	if(ENCx==ENC1)
	{
		ENC1_BASE.ENC_Hardware	=	&ENC_Hardware[0];
	}
	else if(ENCx==ENC2)
	{
		ENC2_BASE.ENC_Hardware	=	&ENC_Hardware[1];
	}
	else if(ENCx==ENC3)
	{
		ENC3_BASE.ENC_Hardware	=	&ENC_Hardware[2];
	}
	else
	{
		ENC4_BASE.ENC_Hardware	=	&ENC_Hardware[3];
	}
	TIM_DeInit(ENCx->ENC_Hardware->TIMx);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period 				=	ENCx_InitStruct->ENC_Arr;
	TIM_TimeBaseStructure.TIM_Prescaler 		= ENCx_InitStruct->ENC_Psc;
	TIM_TimeBaseStructure.TIM_ClockDivision =	ENCx_InitStruct->ENC_CKD;
	TIM_TimeBaseStructure.TIM_CounterMode		= ENCx_InitStruct->ENC_CountM	;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(ENCx->ENC_Hardware->TIMx, &TIM_TimeBaseStructure);  
	TIM_EncoderInterfaceConfig(ENCx->ENC_Hardware->TIMx, ENCx_InitStruct->ENC_mode,ENCx_InitStruct->ENC_ICpola,ENCx_InitStruct->ENC_ICpola);
	
	TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter =ENCx_InitStruct->ENC_ICFilter;//ICx_FILTER;
  TIM_ICInit(ENCx->ENC_Hardware->TIMx, &TIM_ICInitStructure);
	
	TIM_ClearFlag(ENCx->ENC_Hardware->TIMx, TIM_FLAG_Update);
//	TIM_ITConfig(ENCx->ENC_Hardware->TIMx, TIM_IT_Update, ENABLE);
	ENCx->ENC_Hardware->TIMx->CNT = 0;	
	
	ENC_GPIOConfig(ENCx);
	
	ENCx->ENC_OperatingParameter.ENC_CountM	=		ENCx_InitStruct->ENC_CountM;
	ENCx->ENC_OperatingParameter.ENC_Psc		=		ENCx_InitStruct->ENC_Psc;
	ENCx->ENC_OperatingParameter.ENC_ICpola	=		ENCx_InitStruct->ENC_ICpola;
	ENCx->ENC_OperatingParameter.ENC_mode	  =		ENCx_InitStruct->ENC_mode;
	
	return SUCCESS;
}	

ErrorStatus ENC_GPIOConfig(ENC_TypeDef_Struct *ENCx)
{
	GPIO_InitTypeDef 		GPIO_InitStructure;
	
	if(ENCx	==	NULL)		return ERROR;
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin		=	ENCx->ENC_Hardware->GPIO_PinA|ENCx->ENC_Hardware->GPIO_PinB;
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ENCx->ENC_Hardware->GPIOx,&GPIO_InitStructure);
	
	return SUCCESS;
}

ErrorStatus ENC_On(ENC_TypeDef_Struct* ENCx)
{
	if(ENCx	==	NULL)		return ERROR;	
	TIM_Cmd(ENCx->ENC_Hardware->TIMx,ENABLE);
	TIM_CCxCmd( ENCx->ENC_Hardware->TIMx,ENCx->ENC_Hardware->TIM_Channel1,ENABLE);
	TIM_CCxCmd( ENCx->ENC_Hardware->TIMx,ENCx->ENC_Hardware->TIM_Channel2,ENABLE);
	return SUCCESS;
}
//void ENC_Init(void) 
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//  TIM_ICInitTypeDef TIM_ICInitStructure;
//  GPIO_InitTypeDef GPIO_InitStructure;
//	
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//	
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOB,&GPIO_InitStructure);
//	

//	
//	
//    
//    TIM_DeInit(TIM4);
//    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
//    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
//    TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
//		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
//    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);  
//	  TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);

//    TIM_ICStructInit(&TIM_ICInitStructure);
//    TIM_ICInitStructure.TIM_ICFilter = 10;//ICx_FILTER;
//    TIM_ICInit(TIM4, &TIM_ICInitStructure);
//		
//		TIM_ClearFlag(TIM4, TIM_FLAG_Update);
//    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); 
//		
//		TIM4->CNT = 0;
//    TIM_Cmd(TIM4, ENABLE);                           
//}


ErrorStatus Read_Encoder(ENC_TypeDef_Struct *ENCx,ENC_DATA_Struct *pdata)
{
    int Encoder_TIM;  
		if(ENCx	==	NULL||pdata==NULL)		return ERROR;  
	
   if(ENCx->ENC_Hardware->TIMx==TIM2)
	 {
		 Encoder_TIM	= (short)TIM2->CNT;  
		 TIM2->CNT=0;
	 }
	 else if(ENCx->ENC_Hardware->TIMx==TIM3)
	 {
		 Encoder_TIM	= (short)TIM3->CNT; 
		 TIM3->CNT=0;
	 }
	 else if(ENCx->ENC_Hardware->TIMx==TIM4)
	 {
		 Encoder_TIM	= (short)TIM4->CNT; 
		 TIM4->CNT=0;
	 }
	 else if(ENCx->ENC_Hardware->TIMx==TIM5)
	 {
		 Encoder_TIM	= (short)TIM5->CNT; 
		 TIM5->CNT=0;
	 }
		else return ERROR;  
	 
	 if(Encoder_TIM<0)pdata->Encoder_data=-Encoder_TIM;
	 else pdata->Encoder_data=Encoder_TIM;
	 pdata->Velocity=Encoder_TIM;
	 
	 return SUCCESS;  
}



