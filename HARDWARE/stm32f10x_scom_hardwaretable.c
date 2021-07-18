/**
  ******************************************************************************
  * @file    stm32f10x_scom_hardwaretable.c
  * @author  J.F.YUE
  * @version V1.0.0
  * @date    24/DEC/2018
  * @brief   This file provides all the SCOM channel init functions.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, HANGZHOU NORMAL UNIVERSITY YUEJUNFENG  SHALL NOT BE HELD LIABLE
  * FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS 
  * ARISING FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF
  * THE CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2014 HANGZHOU NORMAL UNIVERSITY </center></h2>
  */
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_scom.h"

/** @addtogroup My_Driver
  * @{
  */
	
/**
  * @}
  */
	
/** @defgroup SCOM_Private_Variables
  * @{
  */
	
/*-----------------------------Hardware_Table_Definition--------------------------------
 * SCOMx_USART;  //where x= 1,2,3
 * SCOMx_USART_IRQn;		
 * SCOMx_tx_GPIO;
 * SCOMx_tx_GPIO_Pin;	
 * SCOMx_rx_GPIO;
 * SCOMx_rx_GPIO_Pin;
 * SCOMx_rx_DMA_Channel;
 * SCOMx_rx_DMA_Channel_IRQn;	
 * SCOMx_tx_DMA_Channel;
 * SCOMx_tx_DMA_Channel_IRQn;
 *------------------------------------------------------------------------------------*/	
/* --------------------------------- SCOM Hardware ----------------------------------- */
SCOMHardware_TypeDef_Struct SCOM_Hardware[] = 
{

    { SCOM1_USART,SCOM1_USART_IRQn ,SCOM1_tx_GPIO, SCOM1_tx_GPIO_Pin,SCOM1_rx_GPIO,SCOM1_rx_GPIO_Pin,  
    SCOM1_rx_DMA_Channel, SCOM1_rx_DMA_Channel_IRQn,SCOM1_tx_DMA_Channel,SCOM1_tx_DMA_Channel_IRQn  },  //SCOM1
		
		{ SCOM2_USART,SCOM2_USART_IRQn ,SCOM2_tx_GPIO, SCOM2_tx_GPIO_Pin,SCOM2_rx_GPIO,SCOM2_rx_GPIO_Pin,  
    SCOM2_rx_DMA_Channel, SCOM2_rx_DMA_Channel_IRQn,SCOM2_tx_DMA_Channel,SCOM2_tx_DMA_Channel_IRQn  },  //SCOM2

    { SCOM3_USART,SCOM3_USART_IRQn ,SCOM3_tx_GPIO, SCOM3_tx_GPIO_Pin,SCOM3_rx_GPIO,SCOM3_rx_GPIO_Pin,  
    SCOM3_rx_DMA_Channel, SCOM3_rx_DMA_Channel_IRQn,SCOM3_tx_DMA_Channel,SCOM3_tx_DMA_Channel_IRQn  },  //SCOM3

};	


	
/**
  * @}
  */


/********* (C) COPYRIGHT 2014 HANGZHOU NORMAL UNIVERSITY  *****END OF FILE****/	
