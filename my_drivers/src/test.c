#include "stm32f10x_scom.h" 
#include "delay.h"
#include "stm32f10x_OLED_I2C.h"
#include "stm32f10x_STMADC.h"
#include "stm32f10x_pwm.h"
#include "stm32f10x_capture.h"
#include "stm32f10x_encoder.h"
#include "control.h"
#include "stm32f10x_keyExti.h"
static uint16_t voltage;

int main()
{

	
	extern uint32_t Frequency1;

	extern ENC_DATA_Struct                    Encoder_left;
	extern ENC_DATA_Struct 										Encoder_right;
	extern CURRENT_OPENMVDATA_Struct          OpenmvData;
	extern CURRENT_OPENMVDATA_Struct       		OpenMV_DataBuffer;
	
	extern SCOM_READ_DATA_Struct							SCOM3_rxDataBuffer;	
	extern SCOM_WRITE_DATA_Struct							SCOM3_txDataBuffer;	
	extern SCOM_READ_DATA_Struct								read_data;
  extern SCOM_FrameByte_Struct 								FrameByteNum;

	
	ServoPWM_InitTypeDef_Struct								ServoPWM_InitStruct;
	DrivePWM_InitTypeDef_Struct								DrivePWM_InitStruct;
	
	ENC_InitTypeDef_Struct										ENC_InitStruct;
	SysTime_Beat_InitTypeDef_Struct 					SysTime_Beatx_InitStruct;

	STMADC_DATA_Struct                             Adcdata;
	SCOM_WRITE_DATA_Struct											write_data;

	
	SCOM_InitTypeDef_Struct								SCOM3_InitStruct;
	
	STMADC_InitTypeDef_Struct  								 SENSOR_ADC_init;
	
	uint8_t temp[]={0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x40};
	write_data.len=sizeof(temp);	
	
	memcpy(write_data.data,temp,sizeof(temp));

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB, ENABLE);	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM4|RCC_APB1Periph_TIM5|RCC_APB1Periph_TIM3, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_TIM8, ENABLE);	
	
	delay_init();	 
  
	STMADC_StructInit(&SENSOR_ADC_init);
 	STMADCx_Init(STMADC1,&SENSOR_ADC_init);
	STMADC_Open(STMADC1);
	
	SCOM_StructInit(&SCOM3_InitStruct);
	SCOM_Init(SCOM3,&SCOM3_InitStruct);
	SCOM_On(SCOM3);
	SCOM3->SCOM_Operating.USART_txStartFlag=SET;

	 IIC_Init();
	 OLED_Init();
	 OLED_CLS();
	 
	 
	 ServoPWM_StructInit(&ServoPWM_InitStruct);
	 ServoPWM_Init(ServoPWM1,&ServoPWM_InitStruct);
	 ServoPWM_On(ServoPWM1);


	 DrivePWM_StructInit(&DrivePWM_InitStruct);
	 DrivePWM_Init(DrivePWM1,&DrivePWM_InitStruct);
	 DrivePWM_On(DrivePWM1);

	 DrivePWM_Init(DrivePWM2,&DrivePWM_InitStruct);
	 DrivePWM_On(DrivePWM2);

	 ENC_StructInit(&ENC_InitStruct);
	 ENC_Init(ENC1,&ENC_InitStruct);
	 ENC_On(ENC1);

	 ENC_Init(ENC2,&ENC_InitStruct);
	 ENC_On(ENC2);

	 Tim1_Configuration();
	 
	 SysTime_Beat_StructInit(&SysTime_Beatx_InitStruct);
	 SysTime_Beat_Init(SysTime_Beat, &SysTime_Beatx_InitStruct);
	 Systime_Beat_On(SysTime_Beat);
	 

	 LED_Init();		  	 
	 KEY_Init();
	 EXTIX_Init();
	 Drive_GPIO_Init()	; 

	
	
	while(1)
	{
		
//		ServoPWM_PULSE_Setting(ServoPWM1, 1600);


//			STMADC_Convert_Tick();	 
//		STMADC_Read_vol_data(STMADC1,&voltage);
//		 STMADC_Read_data(STMADC1, &Adcdata);
		
		OLED_ShowString(0,0,"vol:",8);
		OLED_ShowNum(48,0,voltage,4,16);
		OLED_ShowNum(80,0,Frequency1,6,16);

		
																	OLED_ShowString(0,3,"EL",8);    
		if(Encoder_left.Velocity<0)		OLED_ShowString(16,3,"-",8),
																	OLED_ShowNum(32,3,-Encoder_left.Velocity,5,8);
		else                 					OLED_ShowString(16,3,"+",8),
		                              OLED_ShowNum(32,3,Encoder_left.Velocity,5,8);
		
																	OLED_ShowString(64,3,"ER",8);    
		if(Encoder_right.Velocity<0)	OLED_ShowString(80,3,"-",8),
																	OLED_ShowNum(96,3,-Encoder_right.Velocity,5,8);
		else                 					OLED_ShowString(80,3,"+",8),
		                              OLED_ShowNum(96,3,Encoder_right.Velocity,5,8);
		
																	OLED_ShowString(0,5,"rhoerr",8); 
		if(OpenmvData.rho_err<0)			OLED_ShowString(48,5,"-",8),
																	OLED_ShowNum(64,5,-OpenmvData.rho_err,3,8);
		else                 					OLED_ShowString(48,5,"+",8),
		                              OLED_ShowNum(64,5,OpenmvData.rho_err,3,8);
		
																		OLED_ShowString(0,7,"theta",8); 
		if(OpenmvData.theta_err<0)			OLED_ShowString(48,7,"-",8),
																	OLED_ShowNum(64,7,-OpenmvData.theta_err,3,8);
		else                 					OLED_ShowString(48,7,"+",8),
		                              OLED_ShowNum(64,7,OpenmvData.theta_err,3,8);
		
		
//		SCOM_WriteData(SCOM3, &write_data);
//		SCOM_ReadData(SCOM3, &read_data);
		
		delay_ms(10);
	}
	
}
	


	
	
	