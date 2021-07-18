


#include "stm32f10x_STMADC.h"

/******************************************************************************
*                                                                             *
* @defgroup SENSOR_Import_Variables                                  *
* @{                                                                          *
*                                                                             *
******************************************************************************/ 
extern  STMADC_Hardware_TypeDef_Struct STMADC_Hardware[];


/******************************************************************************
*                                                                             *
* @defgroup SENSOR_Private_Variables                                 *
* @{                                                                          *
*                                                                             *
******************************************************************************/ 
typedef struct __STMADC_Inner_Struct
{
	FlagStatus          Infreshed;
	FlagStatus          Outfreshed;	
  uint16_t            AD_data; 
} STMADC_Inner_Struct;




/* ------------------------- SENSOR_BASE -------------------------- */
STMADC_TypeDef_Struct         STMADC1_BASE,STMADC2_BASE,STMADC3_BASE,STMADC4_BASE;
STMADC_Inner_Struct           Inner_STMADC1,Inner_STMADC2,Inner_STMADC3,Inner_STMADC4;
                             


uint16_t AD_data[2];
                              

/******************************************************************************
* @brief  Initializes the GPIO of SENSORx                                     *
* @param  SENSORx: where x can be 1 to SENSOR_NUM_USED  to                    * 
*         select the SENSOR.SENSOR_NUM_USED is defined at                     *
*         SENSORx.h, can be 1 to SENSOR_NUM_MAX, and                          *
*         SENSOR_NUM_MAX is also defined at SENSORx.h,                        *
*         can be 1 to 4.                                                      *
* @param    
*          																			                              *
* @retval : ERROR or SUCCESS.                                                 *
******************************************************************************/ 

ErrorStatus STMADC_GPIOConfig(STMADC_TypeDef_Struct *STMADCx);

/******************************************************************************
* @brief  judge the validity  of SENSORx                                      *
* @param  SENSORx: where x can be 1 to SENSOR_NUM_USED  to                    * 
*         select the SENSOR.SENSOR_NUM_USED is defined at                     *
*         SENSORx.h, can be 1 to SENSOR_NUM_MAX, and                          *
*         SENSOR_NUM_MAX is also defined at SENSORx.h,                        *
*         can be 1 to 4.                                                      *
* @param    
*          																			                              *
* @retval : ERROR or SUCCESS.                                                 *
******************************************************************************/ 

ErrorStatus STMADC_VALID(STMADC_TypeDef_Struct* STMADCx);



/******************************************************************************
* @brief  start convert SENSORx                                     					*
* @param  SENSORx: where x can be 1 to SENSOR_NUM_USED  to                    * 
*         select the SENSOR.SENSOR_NUM_USED is defined at                     *
*         SENSORx.h, can be 1 to SENSOR_NUM_MAX, and                          *
*         SENSOR_NUM_MAX is also defined at SENSORx.h,                        *
*         can be 1 to 4.                                                      *
* @param    
*          																			                              *
* @retval : ERROR or SUCCESS.                                                 *
******************************************************************************/
void STMADC_CMD(STMADC_TypeDef_Struct* STMADCx);


/******************************************************************************
* @brief  ADC Convert Tick, users must call this function in fixed time       *
*         interval to start AD convert, the fixed time interval is no less    *
*         than 100us .              																			    *
*         The switching sequence:  STMADC1  to STMADC4                				*
*                                 																						*
* @param  void                                                      					*
*         																		                                *
* @retval : void                                              								*
******************************************************************************/ 
void STMADC_Convert_Tick(void);


static uint32_t __STMADC_CPU_PRIMASK_REG_SAVE_VALUE;

static __INLINE void __STMADC_Disable_IRQ(void);
static __INLINE void __STMADC_Restore_IRQ(void);

/******************************************************************************
* @brief  Fills each SENSOR_InitStruct member with its default value.*
* @param  SENSOR_InitStruct:pointer to a SENSOR_InitTypeDef *
*         structure which will be initialized.                                *
* @retval : None                                                              *
******************************************************************************/ 
void STMADC_StructInit(STMADC_InitTypeDef_Struct* STMADCx_InitStruct)
{ 	
	/*---------------------- Set the default configuration ----------------------*/
  STMADCx_InitStruct->ADCenable                                        = STMADC_ENABLE_DEFAULT;
	STMADCx_InitStruct->DMAenable																				=STMADC_DMA_ENABLE_DEFAULT;

}
/******************************************************************************
* @brief  Initializes the SENSORx according to the specified.        *
* @param  SENSORx: where x can be 1 to SENSOR_NUM_USED  to  * 
*         select the SENSOR.SENSOR_NUM_USED is defined at   *
*         SENSORx.h, can be 1 to SENSOR_NUM_MAX, and        *
*         SENSOR_NUM_MAX is also defined at SENSORx.h,      *
*         can be 1 to 4.                                                      *
* @param  SENSOR_InitStruct: pointer to a SENSOR_InitTypeDef*
*         structure which will be initialized.                                *
* @retval : ERROR or SUCCESS.                                                 *
******************************************************************************/ 
ErrorStatus STMADCx_Init(STMADC_TypeDef_Struct* STMADCx,STMADC_InitTypeDef_Struct* STMADCx_InitStruct)
{
  ErrorStatus         err;
	uint16_t            ADC1_enable,ADC2_enable;
  uint16_t            ii = 300;	
  ADC_InitTypeDef     ADC_InitStructure;
	DMA_InitTypeDef     DMA_InitStructure;	
	
  if  ((STMADCx == NULL) || (STMADCx_InitStruct == NULL)) return ERROR;
  
/*------ SENSOR_BASE->SENSOR_Hardware Configuration -------*/
  if (STMADCx == STMADC1)
     STMADC1_BASE.STMADC_Hardware = &STMADC_Hardware[0];
  else if (STMADCx == STMADC2)
     STMADC2_BASE.STMADC_Hardware = &STMADC_Hardware[1];
  else if (STMADCx == STMADC3)
     STMADC3_BASE.STMADC_Hardware = &STMADC_Hardware[2];
  else if (STMADCx == STMADC4)
     STMADC4_BASE.STMADC_Hardware = &STMADC_Hardware[3];	
  else 
    return ERROR;

  
/*------------------------ SENSOR_InitConfig ---------------------- */  

	STMADCx->STMADC_Mem.ADCenable        = STMADCx_InitStruct->ADCenable;
	if(STMADCx ->STMADC_Hardware->STMADCx_DMA_Channelx==NULL)
		STMADCx ->STMADC_Mem.DMAenable=DISABLE;
	else 
	STMADCx->STMADC_Mem.DMAenable=STMADCx_InitStruct->DMAenable;
	
  if (STMADCx == STMADC1)
	{		
		 if(STMADC1->STMADC_Mem.ADCenable == ENABLE)
		 {
        if(STMADC1->STMADC_Hardware->STMADCx == ADC1)  
			    ADC1_enable ++;
        if(STMADC1->STMADC_Hardware->STMADCx == ADC2)  
			    ADC2_enable ++;				
     }	
		 
		 Inner_STMADC1.Infreshed                = RESET;
		 Inner_STMADC1.Outfreshed               = RESET;		 

  }	
  else if (STMADCx == STMADC2)
	{
     STMADC2->STMADC_Mem.ADCenable        = STMADCx_InitStruct->ADCenable;
		 if(STMADC2->STMADC_Mem.ADCenable == ENABLE)
		 {
        if(STMADC2->STMADC_Hardware->STMADCx == ADC1)  
			    ADC1_enable ++;
        if(STMADC2->STMADC_Hardware->STMADCx == ADC2)  
			    ADC2_enable ++;		
			}	
			Inner_STMADC2.Infreshed                = RESET;
			Inner_STMADC2.Outfreshed               = RESET;		 

  }	
  else if (STMADCx == STMADC3)
	{
     STMADC3->STMADC_Mem.ADCenable        = STMADCx_InitStruct->ADCenable;
		 if(STMADC3->STMADC_Mem.ADCenable == ENABLE)
		 {
        if(STMADC3->STMADC_Hardware->STMADCx == ADC1)  
			    ADC1_enable ++;
        if(STMADC3->STMADC_Hardware->STMADCx == ADC2)  
			    ADC2_enable ++;				
     }
		 Inner_STMADC3.Infreshed                = RESET;
		 Inner_STMADC3.Outfreshed               = RESET;		 
  }	
  else if (STMADCx == STMADC4)
	{
     STMADC4->STMADC_Mem.ADCenable        = STMADCx_InitStruct->ADCenable;
		 if(STMADC4->STMADC_Mem.ADCenable == ENABLE)
		 {
        if(STMADC4->STMADC_Hardware->STMADCx == ADC1)  
			    ADC1_enable ++;
        if(STMADC4->STMADC_Hardware->STMADCx == ADC2)  
			    ADC2_enable ++;				
     }
		 Inner_STMADC4.Infreshed                = RESET;
		 Inner_STMADC4.Outfreshed               = RESET;		 
  }	
	
	
	 err = STMADC_GPIOConfig(STMADCx);
	
  if(ADC1_enable == 0x01)
  {
    /* ------------¡ª---------------- ADC configuration ------------------------- */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  
   	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
  
    ADC_DeInit(STMADCx->STMADC_Hardware->STMADCx);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(STMADCx->STMADC_Hardware->STMADCx, &ADC_InitStructure);
		  

    ADC_Cmd(STMADCx->STMADC_Hardware->STMADCx, ENABLE);
		
    ADC_ResetCalibration(STMADCx->STMADC_Hardware->STMADCx);
    while(ADC_GetResetCalibrationStatus(STMADCx->STMADC_Hardware->STMADCx));
    ADC_StartCalibration(STMADCx->STMADC_Hardware->STMADCx);
    while(ADC_GetCalibrationStatus(STMADCx->STMADC_Hardware->STMADCx));
		
		if(STMADC1->STMADC_Hardware->STMADCx == ADC1)
		{
         STMADC_CMD(STMADC1); 
    }
		
		/*Init_DMA*/
	if((STMADCx->STMADC_Hardware->STMADC_Channel!=NULL)&&(STMADCx->STMADC_Hardware->STMADCx_DMA_Channelx!=NULL))
	{
		if(STMADCx->STMADC_Mem.DMAenable==ENABLE)
		{
		DMA_DeInit(STMADCx->STMADC_Hardware->STMADCx_DMA_Channelx);    /* DMA1 Channel1 Config */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&AD_data;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 2;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		
    DMA_Init(STMADCx->STMADC_Hardware->STMADCx_DMA_Channelx, &DMA_InitStructure);
		}
	}
	
	
		if((STMADCx->STMADC_Hardware->STMADC_Channel!=NULL)&&(STMADCx->STMADC_Hardware->STMADCx_DMA_Channelx!=NULL))
		{		 
			if(STMADCx->STMADC_Mem.DMAenable==ENABLE)
			{/* Enable ADC1 DMA */ 
				ADC_DMACmd(STMADCx->STMADC_Hardware->STMADCx, ENABLE);  
				DMA_Cmd(STMADCx->STMADC_Hardware->STMADCx_DMA_Channelx, ENABLE);			
			}
		}
	
		while(ii--);	
  }
	
	if(ADC2_enable == 0x01)
  {
    /* ------------¡ª---------------- ADC configuration ------------------------- */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);       			
   	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
  
    ADC_DeInit(STMADCx->STMADC_Hardware->STMADCx);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(STMADCx->STMADC_Hardware->STMADCx, &ADC_InitStructure);

    ADC_Cmd(STMADCx->STMADC_Hardware->STMADCx, ENABLE);
    ADC_ResetCalibration(STMADCx->STMADC_Hardware->STMADCx);
    while(ADC_GetResetCalibrationStatus(STMADCx->STMADC_Hardware->STMADCx));
    ADC_StartCalibration(STMADCx->STMADC_Hardware->STMADCx);
    while(ADC_GetCalibrationStatus(STMADCx->STMADC_Hardware->STMADCx));
		
		if(STMADC1->STMADC_Hardware->STMADCx == ADC2)
		{
         STMADC_CMD(STMADC1); 
    }
		while(ii--);
  }	

		
  return err;
}

/******************************************************************************
* @brief  Open STMADCx                         	      		              	    *
* @param  STMADCx: where x can be  1 to STMADC_NUM_MAX   				  					  *
*         to select the STMADC_USE_DEFAULT is defined at                      *
*         SENSOR.h,can be 1 to SENSOR_NUM_MAX,and                             *
*         SENSOR_NUM_MAX is also defined at SENSOR.h,                         *
*         can be 1 to 4.                                                      *
* @param                                                                      *
*                                                                             *
* @retval : ERROR or SUCCESS.                                                 *
******************************************************************************/ 
ErrorStatus STMADC_Open(STMADC_TypeDef_Struct* STMADCx)
{
  if (STMADC_VALID(STMADCx) == ERROR) return ERROR;
		
  if (STMADCx == STMADC1)
     STMADC1->STMADC_Mem.ADCenable      = ENABLE;
  else if (STMADCx == STMADC2)
     STMADC2->STMADC_Mem.ADCenable      = ENABLE;
  else if (STMADCx == STMADC3)
     STMADC3->STMADC_Mem.ADCenable      = ENABLE;
  else if (STMADCx == STMADC4)
     STMADC4->STMADC_Mem.ADCenable      = ENABLE;
  else 
    return ERROR;	

 return SUCCESS;
}

/******************************************************************************
* @brief  Close STMADCx                         	      		              	  *
* @param  STMADCx: where x can be  1 to STMADC_NUM_MAX   				  					  *
*         to select the STMADC_USE_DEFAULT is defined at                      *
*         SENSOR.h,can be 1 to SENSOR_NUM_MAX,and                             *
*         SENSOR_NUM_MAX is also defined at SENSOR.h,                         *
*         can be 1 to 4.                                                      *
* @param                                                                      *
*                                                                             *
* @retval : ERROR or SUCCESS.                                                 *
******************************************************************************/ 
ErrorStatus STMADC_Close(STMADC_TypeDef_Struct* STMADCx)
{
  if (STMADC_VALID(STMADCx) == ERROR) return ERROR;
		
  if (STMADCx == STMADC1)
	{
		STMADC1->STMADC_Mem.ADCenable   = DISABLE;
		
	}
  else if (STMADCx == STMADC2)
     STMADC2->STMADC_Mem.ADCenable      = DISABLE;
  else if (STMADCx == STMADC3)
     STMADC3->STMADC_Mem.ADCenable      = DISABLE;
  else if (STMADCx == STMADC4)
     STMADC4->STMADC_Mem.ADCenable      = DISABLE;
  else 
    return ERROR;	

 return SUCCESS;
}

ErrorStatus STMADC_Read_data(STMADC_TypeDef_Struct* STMADCx,STMADC_DATA_Struct* Adcdata)
{
	if(STMADCx == NULL)    return  ERROR;
	
	if (STMADCx  == STMADC1)
	{
      if(Inner_STMADC1.Outfreshed == SET)
      {
          Adcdata->freshed  = SET;			
					Adcdata->AD_data 	=Inner_STMADC1.AD_data;	
          Inner_STMADC1.Outfreshed = RESET;
      }
			else
			    Adcdata->freshed  = RESET;
  }
	else if (STMADCx  == STMADC2)
	{
      if(Inner_STMADC2.Outfreshed == SET)
      {
          Adcdata->freshed  = SET;	
					Adcdata->AD_data 	=Inner_STMADC2.AD_data;					
          Inner_STMADC2.Outfreshed = RESET;
      }
			else
			    Adcdata->freshed  = RESET;
  }
	else if (STMADCx  == STMADC3)
	{
      if(Inner_STMADC3.Outfreshed == SET)
      {
          Adcdata->freshed  = SET;
					Adcdata->AD_data 	=Inner_STMADC3.AD_data;					
          Inner_STMADC3.Outfreshed = RESET;
      }
			else
			    Adcdata->freshed  = RESET;
  }
	else if (STMADCx  == STMADC4)
	{
      if(Inner_STMADC4.Outfreshed == SET)
      {
          Adcdata->freshed  = SET;
					Adcdata->AD_data 	=Inner_STMADC4.AD_data;					
          Inner_STMADC4.Outfreshed = RESET;
      }
			else
			    Adcdata->freshed  = RESET;
  }
}

ErrorStatus  STMADC_Read_vol_data(STMADC_TypeDef_Struct* STMADCx,uint16_t* voltage)
{
  if(STMADCx == NULL)   return  ERROR;
	if (STMADCx  == STMADC1)
	{
    *voltage=(AD_data[0]*3.3*11/4096)*100; 
  }
	else return  ERROR;
	
} 

/******************************************************************************
*                                                                             *
* @defgroup SENSOR_Private_Function                                           *
* @{                                                                          *
*                                                                             *
******************************************************************************/ 
void ADC_GetData(STMADC_TypeDef_Struct* STMADCx)
{
	  uint16_t    Adc_data;
	
    if( ADC_GetFlagStatus(STMADCx->STMADC_Hardware->STMADCx, ADC_FLAG_EOC) == 1)
		{
				Adc_data = ADC_GetConversionValue(STMADCx->STMADC_Hardware->STMADCx);

				if(Adc_data      >= STMADC_DATA_MAX)
				{
					Adc_data        = STMADC_DATA_MAX;
				}
				else if(Adc_data <= STMADC_DATA_MIN)
				{
					Adc_data        = STMADC_DATA_MIN;  
				}
				
	      __STMADC_Disable_IRQ();
        if(STMADCx == STMADC1)
				{
            Inner_STMADC1.Infreshed   = SET;
            Inner_STMADC1.Outfreshed  = SET;					
					  Inner_STMADC1.AD_data     = Adc_data;
        }
        else if(STMADCx == STMADC2)
				{
            Inner_STMADC2.Infreshed     = SET;
            Inner_STMADC2.Outfreshed  = SET;						
					  Inner_STMADC2.AD_data     = Adc_data;
        }
        else if(STMADCx == STMADC3)
				{
            Inner_STMADC3.Infreshed     = SET;
            Inner_STMADC3.Outfreshed  = SET;						
					  Inner_STMADC3.AD_data     = Adc_data;
        }				
				else if(STMADCx == STMADC4)
				{
            Inner_STMADC4.Infreshed     = SET;
            Inner_STMADC4.Outfreshed  = SET;						
					  Inner_STMADC4.AD_data     = Adc_data;
        }
        __STMADC_Restore_IRQ();
				
				ADC_ClearFlag(STMADCx->STMADC_Hardware->STMADCx, ADC_FLAG_EOC);
		}	
		else
		{
        if(STMADCx == STMADC1)
				{
            Inner_STMADC1.Infreshed   = RESET;
        }
        else if(STMADCx == STMADC2)
				{
            Inner_STMADC2.Infreshed   = RESET;
        }
        else if(STMADCx == STMADC3)
				{
            Inner_STMADC3.Infreshed   = RESET;
        }				
				else if(STMADCx == STMADC4)
				{
            Inner_STMADC4.Infreshed   = RESET;
        }	
		}				
				
}
/******************************************************************************
*                                                                             *
* @defgroup SENSOR_Private_Function                                           *
* @{                                                                          *
*                                                                             *
******************************************************************************/ 

ErrorStatus STMADC_VALID(STMADC_TypeDef_Struct* STMADCx)
{	
	if ((STMADCx == NULL) ||(STMADCx->STMADC_Hardware->STMADCx == STMADC_NULL) ||
		                  (STMADCx->STMADC_Hardware->STMADC_GPIOx == STMADC_GPIO_NULL) ||
	                (STMADCx->STMADC_Hardware->STMADC_GPIO_Pin == STMADC_GPIO_Pin_NULL))
      return ERROR;
	else 
    return SUCCESS;

}	
void STMADC_CMD(STMADC_TypeDef_Struct* STMADCx)
{
  if(STMADCx == STMADC1)
	{
		if((STMADC_VALID(STMADC1)!= ERROR)&&(STMADC1->STMADC_Mem.ADCenable == ENABLE))
		{
			 ADC_RegularChannelConfig(STMADC1->STMADC_Hardware->STMADCx, 
																								 STMADC1->STMADC_Hardware->STMADC_Channel,
																								 1, ADC_SampleTime_239Cycles5);
			 ADC_SoftwareStartConvCmd(STMADC1->STMADC_Hardware->STMADCx,ENABLE);
			
			
		
  }
}
	else if(STMADCx == STMADC2)
	{
		if((STMADC_VALID(STMADC2)!= ERROR)&&(STMADC2->STMADC_Mem.ADCenable == ENABLE))
		{
			 ADC_RegularChannelConfig(STMADC2->STMADC_Hardware->STMADCx, 
																								 STMADC2->STMADC_Hardware->STMADC_Channel,
																								 1, ADC_SampleTime_239Cycles5);
			 ADC_SoftwareStartConvCmd(STMADC2->STMADC_Hardware->STMADCx,ENABLE);
		}
  }
	else if(STMADCx == STMADC3)
	{
		if((STMADC_VALID(STMADC3)!= ERROR)&&(STMADC3->STMADC_Mem.ADCenable == ENABLE))
		{
			 ADC_RegularChannelConfig(STMADC3->STMADC_Hardware->STMADCx, 
																								 STMADC3->STMADC_Hardware->STMADC_Channel,
																								 1, ADC_SampleTime_239Cycles5);
			 ADC_SoftwareStartConvCmd(STMADC3->STMADC_Hardware->STMADCx,ENABLE);
		}
  }	
	else if(STMADCx == STMADC4)
	{
		if((STMADC_VALID(STMADC4)!= ERROR)&&(STMADC4->STMADC_Mem.ADCenable == ENABLE))
		{
			 ADC_RegularChannelConfig(STMADC4->STMADC_Hardware->STMADCx, 
																								 STMADC4->STMADC_Hardware->STMADC_Channel,
																								 1, ADC_SampleTime_239Cycles5);
			 ADC_SoftwareStartConvCmd(STMADC4->STMADC_Hardware->STMADCx,ENABLE);
		}
  }	
}	




ErrorStatus STMADC_GPIOConfig(STMADC_TypeDef_Struct *STMADCx)
{    
  GPIO_InitTypeDef GPIO_InitStructure;  

  if (STMADC_VALID(STMADCx) == ERROR) return ERROR;  
	
  /* SENSOR GPIO Configuration */
  GPIO_InitStructure.GPIO_Pin  = STMADCx->STMADC_Hardware->STMADC_GPIO_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;;	
  GPIO_Init(STMADCx->STMADC_Hardware->STMADC_GPIOx,&GPIO_InitStructure);
	
	

  return SUCCESS;
}


		




void STMADC_Convert_Tick(void)
{  
	static    uint64_t time = 0;

	
  if(time == 0xffffffffffffffff)
  {
    time = 0x00;
  }    

  switch (STMADC_USED)
	{
    case 1:
	    if(time%STMADC_USED == 0x00)
	    {
					ADC_GetData(STMADC1);					
					STMADC_CMD(STMADC1);     //start ADC1									
			}			
		  break;
    case 2:
	    if(time%STMADC_USED == 0x00)
	    {		
					ADC_GetData(STMADC1);					
					STMADC_CMD(STMADC2);     //start ADC2		
									
			}		
	    if(time%STMADC_USED == 0x01)
	    {
				  ADC_GetData(STMADC2);
				  STMADC_CMD(STMADC1);     //start ADC1						
			}								
		  break;
    case 3:
	    if(time%STMADC_USED == 0x00)
	    {		
					ADC_GetData(STMADC1);
				  STMADC_CMD(STMADC2);     //start ADC2				
			}		
	    if(time%STMADC_USED == 0x01)
	    {
				  ADC_GetData(STMADC2);
				  STMADC_CMD(STMADC3);     //start ADC3						
			}	
	    if(time%STMADC_USED == 0x02)
	    {
					ADC_GetData(STMADC3);
				  STMADC_CMD(STMADC1);     //start ADC1					
			}			
		  break;
    case 4:
	    if(time%STMADC_USED == 0x00)
	    {	
					ADC_GetData(STMADC1);
				  STMADC_CMD(STMADC2);     //start ADC2					
			}		
	    if(time%STMADC_USED == 0x01)
	    {
					ADC_GetData(STMADC2);
				  STMADC_CMD(STMADC3);     //start ADC3					
			}	
	    if(time%STMADC_USED == 0x02)
	    {
					ADC_GetData(STMADC3);
				  STMADC_CMD(STMADC4);     //start ADC4					
			}	
	    if(time%STMADC_USED == 0x03)
	    {
				ADC_GetData(STMADC4);
				STMADC_CMD(STMADC1);     //start ADC1				
			}				
		  break;
			    default: 
			break;		
  }
  time++;	

}	



static __INLINE void __STMADC_Disable_IRQ(void)
{
	  __STMADC_CPU_PRIMASK_REG_SAVE_VALUE = __get_PRIMASK();
	  __disable_irq();
}
static __INLINE void __STMADC_Restore_IRQ(void)
{
	  __set_PRIMASK(__STMADC_CPU_PRIMASK_REG_SAVE_VALUE);
}
/****************************  end   **************************************************/