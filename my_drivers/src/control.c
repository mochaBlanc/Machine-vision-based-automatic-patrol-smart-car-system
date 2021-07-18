#include "control.h"
/*-----------------------------Hardware_Table_Derition--------------------------------
*	TIMx;
*	TIMx_IRQn;
*------------------------------------------------------------------------------------*/
/* --------------------------------- SysTime_Beat Hardware ----------------------------------- */

SysTime_Beat_Hardware_TypeDef_Struct SYSTIME_BEAT_Hardware[] = 
{
	#ifdef SysTime_Beat
			{ SysTime_Beat_TIM,SysTime_Beat_TIM_IRQn}, 
	#endif
};

SysTime_Beat_TypeDef_Struct    SysTime_Beat_BASE;

ENC_DATA_Struct 															Encoder_left;
ENC_DATA_Struct 															Encoder_right;
CURRENT_OPENMVDATA_Struct                     OpenmvData;

float Velocity,Velocity_Set,Angle,Angle_Set;
int Motor_A=200,Motor_B=200,Servo=1550,Target_A,Target_B;
uint8_t Flag_Way=0,Flag_Stop=0;

uint32_t  Frequency1=0;
uint32_t  sum=0;
uint16_t  count=0;
uint32_t  CAPTURE1;

/**
  * @brief  TIM2 interrupt service routine
  * @param  None.
  * @retval None.
  */
void TIM2_IRQHandler(void)
{
	SysTime_Beat_IRQHandler(SysTime_Beat);
}  

void SysTime_Beat_StructInit(SysTime_Beat_InitTypeDef_Struct* SysTime_Beatx_InitStruct)
{
	SysTime_Beatx_InitStruct->SysTime_Beat_CountM							=	SYSTIME_BEAT_COUNTERMODE_DEFAULT;
	SysTime_Beatx_InitStruct->SysTime_Beat_Arr								=	SYSTIME_BEAT_ARR_DEFAULT;	
	SysTime_Beatx_InitStruct->SysTime_Beat_Psc								=	SYSTIME_BEAT_PSC_DEFAULT;
	SysTime_Beatx_InitStruct->SysTime_Beat_PreemptionPriority = SYSTIME_BEAT_PREEMPTIONPRIORITY_DEFAULT;
	SysTime_Beatx_InitStruct->SysTime_Beat_SubPriority				= SYSTIME_BEAT_SUBPRIORITY_DEFAULT;
}

ErrorStatus SysTime_Beat_Init(SysTime_Beat_TypeDef_Struct* SysTime_Beatx, SysTime_Beat_InitTypeDef_Struct* SysTime_Beatx_InitStruct)
{
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
  NVIC_InitTypeDef 					NVIC_InitStructure;  
	
	if(SysTime_Beatx	==	NULL)					return ERROR;
	
	/*SysTime_Beat_BASE->SysTime_Beat_Hardware Configuration */
	
	if(SysTime_Beatx==SysTime_Beat)
	{
		SysTime_Beat_BASE.SysTime_Beat_Hardware=&SYSTIME_BEAT_Hardware[0];
	}
	
	TIM_DeInit(SysTime_Beatx->SysTime_Beat_Hardware->TIMx);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period					=			SysTime_Beatx_InitStruct->SysTime_Beat_Arr-1;
	TIM_TimeBaseStructure.TIM_Prescaler				=			SysTime_Beatx_InitStruct->SysTime_Beat_Psc;
	TIM_TimeBaseStructure.TIM_CounterMode			=			SysTime_Beatx_InitStruct->SysTime_Beat_CountM;
	TIM_TimeBaseStructure.TIM_ClockDivision   = 		SYSTIME_BEAT_CLOCKDIVISION_DEFAULT;
	
	TIM_TimeBaseInit(SysTime_Beatx->SysTime_Beat_Hardware->TIMx,&TIM_TimeBaseStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel 									 = SysTime_Beatx->SysTime_Beat_Hardware->TIMx_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SysTime_Beatx_InitStruct->SysTime_Beat_PreemptionPriority;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority				 = SysTime_Beatx_InitStruct->SysTime_Beat_SubPriority;
  NVIC_InitStructure.NVIC_IRQChannelCmd 							 = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
	
	TIM_ClearFlag(SysTime_Beatx->SysTime_Beat_Hardware->TIMx,TIM_FLAG_Update);
  TIM_ITConfig(SysTime_Beatx->SysTime_Beat_Hardware->TIMx,TIM_IT_Update,ENABLE ); 


	SysTime_Beatx->SysTime_Beat_OperatingParameter.SysTime_Beat_Arr			=	SysTime_Beatx_InitStruct->SysTime_Beat_Arr-1;
	SysTime_Beatx->SysTime_Beat_OperatingParameter.SysTime_Beat_CountM	=	SysTime_Beatx_InitStruct->SysTime_Beat_CountM;
	SysTime_Beatx->SysTime_Beat_OperatingParameter.SysTime_Beat_Psc			=	SysTime_Beatx_InitStruct->SysTime_Beat_Psc;
	
	return SUCCESS;
	
}

ErrorStatus Systime_Beat_On(SysTime_Beat_TypeDef_Struct* SysTime_Beatx)
{
	if(SysTime_Beatx	==	NULL)					return ERROR;
	
	TIM_Cmd(SysTime_Beatx->SysTime_Beat_Hardware->TIMx,ENABLE);
	
	return SUCCESS;
}
ErrorStatus SysTime_Beat_IRQHandler(SysTime_Beat_TypeDef_Struct* SysTime_Beatx)
{
	uint8_t temp;
	 if(SysTime_Beatx==NULL)return ERROR;
	 
	 if (TIM_GetITStatus(SysTime_Beatx->SysTime_Beat_Hardware->TIMx, TIM_IT_Update) != RESET) 
   {
            TIM_ClearITPendingBit(SysTime_Beatx->SysTime_Beat_Hardware->TIMx, TIM_IT_Update);
            CAPTURE1=(u32)TIM_GetCounter(TIM1);
						
		 
		 
				   
						Read_Encoder(ENC1,&Encoder_left);
						Read_Encoder(ENC2,&Encoder_right);
						Read_OpenmvProcessingData(SCOM3,&OpenmvData);
				if(Flag_Way==1)
				{		
		 
						Velocity=25;	   //巡线模式的速度
		 	 
						Angle=Position_PID (&OpenmvData);
				    Kinematic_Analysis(Velocity,Angle);


				}
				else
				{
						Target_A=0;
						Target_B=0;
						Servo=1550;
						
				}
						
				if(Flag_Stop==0)
				{
			
						Motor_A=Incremental_PI_A(Encoder_left.Encoder_data/38,Target_A);
		        Motor_B=Incremental_PI_B(Encoder_right.Encoder_data/38,Target_B);
						GPIO_SetBits(GPIOC,GPIO_Pin_8);
		
						GPIO_ResetBits(GPIOC,GPIO_Pin_9);
					
						GPIO_SetBits(GPIOB,GPIO_Pin_2);
						
						GPIO_ResetBits(GPIOB,GPIO_Pin_1);
					
						Xianfu_Pwm();
						Set_Pwm(Motor_A,Motor_B,Servo);
				}
				else
				{
					GPIO_SetBits(GPIOC,GPIO_Pin_9);
					GPIO_SetBits(GPIOB,GPIO_Pin_1);
					Set_Pwm(0,0,1650);
				}
				
						
						 					
    }
                        sum+=CAPTURE1;
                        count+=1;
                        if(count==20)
                        {
                            Frequency1=sum/0.005/20;
                            sum=0;
                            count=0;
												}
					TIM_SetCounter(TIM1,0);
												
		return SUCCESS;
}
 


#define T 0.156f
#define L 0.1445f
#define K 632.8f
u8 Flag_Target;

void Kinematic_Analysis(float velocity,float angle)
{
		Target_A=velocity*(1+T*tan(angle)/2/L); 
		Target_B=velocity*(1-T*tan(angle)/2/L);      //后轮差速
		Servo=ServoPWM_PULSE_DEFAULT+angle*K;        //舵机转向   
}

int Incremental_PI_A (int Encoder,int Target)
{ 	
   float Kp=0.14,Ki=0.14;	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //计算偏差
	 Pwm+=Kp*(Bias-Last_bias)+Ki*Bias;   //增量式PI控制器
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}

int Incremental_PI_B (int Encoder,int Target)
{ 	
   float Kp=0.14,Ki=0.14;	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //计算偏差
	 Pwm+=Kp*(Bias-Last_bias)+Ki*Bias;   //增量式PI控制器
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}

float Position_PID (CURRENT_OPENMVDATA_Struct *pdata)
{ 	
	 float Position_KP=0.015,Position_KI=0.0001,Position_KD=0.085;
	 static float Angle,Bias,Integral_bias,Last_Bias;
	 Bias=pdata->rho_err+0.2*pdata->theta_err;                              //计算偏差
//	 Integral_bias+=Bias;	                                 //求出偏差的积分
//	 Angle=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);       //位置式PID控制器
	 Angle=Position_KP*Bias+Position_KD*(Bias-Last_Bias);
	 Last_Bias=Bias; 	//保存上一次偏差    
   return Angle;	
}

void Xianfu_Pwm(void)
{	
		int Amplitude=700;    //===PWM满幅是7200 限制在6900
    if(Motor_A<-Amplitude) Motor_A=-Amplitude;	
		if(Motor_A>Amplitude)  Motor_A=Amplitude;	
	  if(Motor_B<-Amplitude) Motor_B=-Amplitude;	
		if(Motor_B>Amplitude)  Motor_B=Amplitude;	
		if(Servo<(ServoPWM_PULSE_DEFAULT-400))     Servo=ServoPWM_PULSE_DEFAULT-400;	  //舵机限幅
		if(Servo>(ServoPWM_PULSE_DEFAULT+400))     Servo=ServoPWM_PULSE_DEFAULT+400;		  //舵机限幅
}
void Set_Pwm(int motor_a,int motor_b,int servo)
{
	
	
	
	if(motor_a<0)motor_a=-motor_a;
	if(motor_b<0)motor_b=-motor_b;

	ServoPWM_PULSE_Setting(ServoPWM1, servo);
	
	DrivePWM_Duty_Setting(DrivePWM1,(motor_a/10));
	
	DrivePWM_Duty_Setting(DrivePWM2,(motor_b/10));

     
}
void Drive_GPIO_Init(void)
{
	GPIO_InitTypeDef 		GPIO_InitStructure;
	
	
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_8);
		
	GPIO_SetBits(GPIOC,GPIO_Pin_9);
	
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_2);
		
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
}

 void EXTI9_5_IRQHandler(void)
{			
	delay_ms(10);   //消抖	
	if(KEY0==0)	{
		LED0=!LED0;
		Flag_Way=!Flag_Way;
	}
 	 EXTI_ClearITPendingBit(EXTI_Line5);    //清除LINE5上的中断标志位  
}


void EXTI15_10_IRQHandler(void)
{
  delay_ms(10);    //消抖			 
  if(KEY1==0)	{
		LED1=!LED1;
		Flag_Stop=!Flag_Stop;
	}
	 EXTI_ClearITPendingBit(EXTI_Line15);  //清除LINE15线路挂起位
}
