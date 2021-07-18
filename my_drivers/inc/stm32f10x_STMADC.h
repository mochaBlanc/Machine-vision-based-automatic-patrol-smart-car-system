/******************************************************************************
*                                                                             *
* @file     stm32f10x_STMADC.c                                                *
* @version  V1.0.1                                                            *
* @date     24/DEC/2018
* @author   YUE JUNFENG
* @brief    This file provides all the SENSOR firmware functions.         		*
*                                                                             *
******************************************************************************/ 

	/*------------------ Define to prevent recursive inclusion ------------------*/

#ifndef __STM32F10X_STMADC_H
#define __STM32F10X_STMADC_H
/*------------------------------- Includes ----------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "string.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

/* ----------------- STMADC_InitTypeDef_Struct Default_Value define -------------------- */
#define STMADC_ENABLE_DEFAULT                               ENABLE
#define STMADC_DMA_ENABLE_DEFAULT                           DISABLE
#define STMADC_DATA_MAX                                     0xFFFF    /* 100% */
#define STMADC_DATA_MIN                                     0x0000    /*  0% */

/******************************************************************************
*                                                                             
* @brief  STMADC_DATA_Struct structure definition                    
*        
*         freshed = RESET:  
*         advalue:12bit AD data 
*
*
******************************************************************************/
typedef struct __STMADC_DATA_Struct
{
	FlagStatus          freshed;
  uint16_t            AD_data;            
} STMADC_DATA_Struct;


/******************************************************************************
*                                                                             *
* @brief  STMADCHARDWARE_TypeDef_Struct structure definition         					*
*                                                                             *
******************************************************************************/
typedef struct __STMADC_Hardware_TypeDef_Struct
{
  ADC_TypeDef     *STMADCx; 
  GPIO_TypeDef    *STMADC_GPIOx;
  uint16_t        STMADC_GPIO_Pin; 
  uint16_t        STMADC_Channel; 
	// DMA  Hardware 
		DMA_Channel_TypeDef *         STMADCx_DMA_Channelx;

} STMADC_Hardware_TypeDef_Struct;



typedef struct __STMADC_Memory_Struct
{
	FunctionalState                   ADCenable; 
	FunctionalState                		DMAenable;
} STMADC_Memory_Struct;

/******************************************************************************
*                                                                             
* @brief  STMADC_TypeDef_Struct structure definition                 
*                                                                             
******************************************************************************/
typedef struct __STMADC_TypeDef_Struct
{
  STMADC_Hardware_TypeDef_Struct    *STMADC_Hardware;
	STMADC_Memory_Struct               STMADC_Mem;        
} STMADC_TypeDef_Struct;

/******************************************************************************
*                                                                             *
* @brief  STMADC_Init_TypeDef_Struct structure definition           				  *
*                                                                             *
******************************************************************************/
typedef struct __STMADC_InitTypeDef_Struct
{
  FunctionalState               ADCenable;
	FunctionalState               DMAenable;
}STMADC_InitTypeDef_Struct;





#define STMADC_NUM_MAX         4   /* unit:tick   the max cycle */
#define STMADC_USED            4

#if(STMADC_NUM_MAX>4)
#error STMADC_NUM_MAX GREATER THAN 4
#endif

#if(STMADC_USED>STMADC_NUM_MAX)
#error STMADC_USED GREATER THAN STMADC_NUM_MAX
#endif

#if(STMADC_USED<1)
#error STMADC_USED LESS THAN 1
#endif


#ifndef STMADC_NULL	
#define STMADC_NULL          			((uint16_t)0x0000)/* No ADC selected */
#endif

#ifndef STMADC_GPIO_NULL
#define STMADC_GPIO_NULL         	((uint16_t)0x0000)/* No GPIO selected */
#endif

#ifndef STMADC_GPIO_Pin_NULL
#define STMADC_GPIO_Pin_NULL     	((uint16_t)0x0000)/* No Pin selected */
#endif

#ifndef STMADC_Channel_NULL
#define STMADC_Channel_NULL  			((uint16_t)0x0000)/* No ADC_Channel selected */
#endif

#ifndef STMADC_DMA_Channel_NULL 
#define STMADC_DMA_Channel_NULL 	 ((uint16_t)0x0000) /* No DMA_Channel selected */
#endif

/* -------- STMADC  hardware define  ------- */


#if(STMADC_NUM_MAX>=1)
extern STMADC_TypeDef_Struct STMADC1_BASE;
#define STMADC1                                  (&STMADC1_BASE)     /* STMADC 1 selected */
#define STMADC1_ADC   										  		  ADC1
#define STMADC1_GPIO      									   		GPIOA
#define STMADC1_GPIO_Pin                          GPIO_Pin_4 	       											  
#define STMADC1_Channel    										    ADC_Channel_4      
#define STMADC1_DMA_Channel          							DMA1_Channel1
#else
#define STMADC1                                   STMADC_NULL                     /* STMADC 1 noselected */
#define STMADC1_ADC   										  		  STMADC_NULL
#define STMADC1_GPIO      									   		STMADC_GPIO_NULL
#define STMADC1_GPIO_Pin                          STMADC_GPIO_Pin_NULL 												  
#define STMADC1_Channel    										    STMADC_Channel_NULL
#define STMADC1_DMA_Channel          							STMADC_DMA_Channe_NULL
#endif

#if(STMADC_NUM_MAX>=2)
extern STMADC_TypeDef_Struct STMADC2_BASE;
#define STMADC2                                  (&STMADC2_BASE)     /* STMADC 2 selected */
#define STMADC2_ADC   										  		  ADC1
#define STMADC2_GPIO      									   		GPIOC 
#define STMADC2_GPIO_Pin                          GPIO_Pin_1 												  
#define STMADC2_Channel    										    ADC_Channel_11
#define STMADC2_DMA_Channel          							DMA1_Channel2
#else
#define STMADC2                                   STMADC_NULL                    /* STMADC 2 noselected */
#define STMADC2_ADC   										  		  STMADC_NULL
#define STMADC2_GPIO      									   		STMADC_GPIO_NULL
#define STMADC2_GPIO_Pin                          STMADC_GPIO_Pin_NULL 												  
#define STMADC2_Channel    										    STMADC_Channel_NULL
#define STMADC2_DMA_Channel          							STMADC_DMA_Channe_NULL
#endif

#if(STMADC_NUM_MAX>=3)
extern STMADC_TypeDef_Struct STMADC3_BASE;
#define STMADC3                                  (&STMADC3_BASE)     /* STMADC 3 selected */
#define STMADC3_ADC   										  		  ADC1
#define STMADC3_GPIO      									   		GPIOC
#define STMADC3_GPIO_Pin                          GPIO_Pin_2 			 //2									  
#define STMADC3_Channel    										    ADC_Channel_12   // 2
#else
#define STMADC3                                   STMADC_NULL                     /* STMADC 3 noselected */
#define STMADC3_ADC   										  		  STMADC_NULL
#define STMADC3_GPIO      									   		STMADC_GPIO_NULL
#define STMADC3_GPIO_Pin                          STMADC_GPIO_Pin_NULL 												  
#define STMADC3_Channel    										    STMADC_Channel_NULL
#endif

#if(STMADC_NUM_MAX>=4)
extern STMADC_TypeDef_Struct STMADC4_BASE;
#define STMADC4                                  (&STMADC4_BASE)     /* STMADC 4 selected */
#define STMADC4_ADC   										  		  ADC1
#define STMADC4_GPIO      									   		GPIOC
#define STMADC4_GPIO_Pin                          GPIO_Pin_3 												  
#define STMADC4_Channel    										    ADC_Channel_13
#else
#define STMADC4                                   STMADC_NULL                    /* STMADC 4 noselected */
#define STMADC4_ADC   										  		  STMADC_NULL
#define STMADC4_GPIO      									   		STMADC_GPIO_NULL
#define STMADC4_GPIO_Pin                          STMADC_GPIO_Pin_NULL 												  
#define STMADC4_Channel    										    STMADC_Channel_NULL
#endif

/******************************************************************************
* @brief  Fills each STMADC_InitStruct member with its default value.     		*
* @param  STMADC_InitStruct:pointer to a STMADC_InitTypeDef           				*
*         structure which will be initialized.                                *
* @retval :None.                                                             	* 
******************************************************************************/
void STMADC_StructInit(STMADC_InitTypeDef_Struct* STMADCx_InitStruct);

/******************************************************************************
* @brief  Initializes the ADCx  according to the specified parameters in the  *
*         STMADC_InitStruct.                                           				*
* @param  STMADCx: where x can be 1 to STMADC_NUM_USED to   									*
*         select the STMADC.STMADC_NUM_USED is defined at      								*
*         STMADC.h,can be 1 to STMADC_NUM_MAX,and          										*
*         be 1 to 4.                                                     			*     
* @param  STMADC_InitStruct:pointer to a STMADC_InitTypeDef 									*
*         structure which will be initialized.                         				*       
* @retval :ERROR or SUCCESS.                                         					*         
******************************************************************************/
ErrorStatus STMADCx_Init(STMADC_TypeDef_Struct* STMADCx,STMADC_InitTypeDef_Struct* STMADCx_InitStruct);


/******************************************************************************
* @brief  Open STMADCx                         	      		              	    *
* @param  STMADCx: where x can be  1 to STMADC_NUM_MAX   				  					  *
*         to select the STMADC_USE_DEFAULT is defined at                      *
*         SENSOR.h,can be 1 to SENSOR_NUM_MAX,and                             *
*         SENSOR_NUM_MAX is also defined at SENSOR.h,                         *
*         can be 1 to 8.                                                      *
* @param                                                                      *
*                                                                             *
* @retval : ERROR or SUCCESS.                                                 *
******************************************************************************/ 
ErrorStatus STMADC_Open(STMADC_TypeDef_Struct* STMADCx);

/******************************************************************************
* @brief  Close STMADCx                         	      		              	  *
* @param  STMADCx: where x can be  1 to STMADC_NUM_MAX   				  					  *
*         to select the STMADC_USE_DEFAULT is defined at                      *
*         SENSOR.h,can be 1 to SENSOR_NUM_MAX,and                             *
*         SENSOR_NUM_MAX is also defined at SENSOR.h,                         *
*         can be 1 to 8.                                                      *
* @param                                                                      *
*                                                                             *
* @retval : ERROR or SUCCESS.                                                 *
******************************************************************************/ 
ErrorStatus STMADC_Close(STMADC_TypeDef_Struct* STMADCx);


/******************************************************************************
* @brief  Read the float value voltage of SENSORx. 
*
*                                                                  					  *
* @param  STMADCx: where x can be 1 to SENSOR_NUM_USED  to  									* 
*         select the SENSOR.SENSOR_NUM_USED is defined at 									  *
*         SENSOR.h,can be 1 to SENSOR_NUM_MAX,and          										*
*         SENSOR_NUM_MAX is also defined at SENSOR.h,can   										*
*         be 1 to 4.  
* @param  voltage: 100 times bigger than the regular            							*
*   			voltage = ad_data*3.3*11/4096)*100
            	 																															  *
 * @retval :	ERROR or SUCCESS.                            										*
******************************************************************************/

ErrorStatus  STMADC_Read_vol_data(STMADC_TypeDef_Struct* STMADCx,uint16_t* voltage);


/******************************************************************************
* @brief  Read the float value of SENSORx.                         					  *
*                                                                  					  *
* @param  STMADCx: where x can be 1 to SENSOR_NUM_USED  to  							* 
*         select the SENSOR.SENSOR_NUM_USED is defined at 									  *
*         SENSOR.h,can be 1 to SENSOR_NUM_MAX,and          										*
*         SENSOR_NUM_MAX is also defined at SENSOR.h,can   										*
*         be 1 to 4.                                                          *
* @param  STMADC_DATA_Struct  STMADC:change opening of the SENSORx.   *
*         STMADC.advalue:opening of the STMADCx,This parameter can be:*
*					1.0f to SENSOR_MAXOPENING,and 0.0f to SENSOR_MINOPENING             *
*                                                      											  *
*          STMADC.freshed: STMADCx data update or not                 *
*                       RESET:  There is no data update to STMADCx        *
*          STMADC.value:   float data of the STMADCx                  *
 * @retval :ERROR or SUCCESS.                                                 *
******************************************************************************/

ErrorStatus STMADC_Read_data(STMADC_TypeDef_Struct* STMADCx,STMADC_DATA_Struct* Adcdata);

/******************************************************************************
* @brief  ADC Convert Tick, users must call this function in fixed time       *
*         interval to start AD convert, the fixed time interval is no less    *
*         than 100us .              																			    *
*         The switching sequence:  STMADC1  to STMADC8                *
*                                 																						*
* @param  void                               								  				 			  *
*         																		                                *
* @retval : ERROR or SUCCESS.                                                 *
******************************************************************************/ 

void STMADC_Convert_Tick(void);




#endif /* __SSTMADC_h */