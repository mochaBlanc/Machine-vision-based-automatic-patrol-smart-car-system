/**
  ******************************************************************************
  * @file    stm32f10x_scom_it.c
  * @author  J.F. YUE
  * @version V1.0.0
  * @date    15/April/2018
  * @brief   This file provides all the SCOM channel init functions.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, HANGZHOU NORMAL UNIVERSITY YUEJUNFENG SHALL NOT BE HELD LIABLE
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
	
/** @defgroup SCOM_Import_Functions
  * @{
  */	

/**
  * @}
  */

/** @defgroup SCOM_FunctionPrototypes
  * @{
  */
	
/**
  * @brief  USART1 interrupt service routine
  * @param  None.
  * @retval None.
  */
	
void USART1_IRQHandler(void) 
{
	 SCOM_USART1_IRQHandler();
}
void SCOM_USART1_IRQHandler(void) 
{
#if(SCOM1_USART_IRQn == 37)
	 SCOM_USART_IRQHandler(SCOM1);
#endif	
	
#if(SCOM2_USART_IRQn == 37)
	 SCOM_USART_IRQHandler(SCOM2);
#endif
	
#if(SCOM3_USART_IRQn == 37)
	SCOM_USART_IRQHandler(SCOM3);
#endif	
	
#if(SCOM4_USART_IRQn == 37)
	SCOM_USART_IRQHandler(SCOM4);
#endif
	
#if(SCOM5_USART_IRQn == 37)
	SCOM_USART_IRQHandler(SCOM5);
#endif		
}


/**
  * @brief  USART2 interrupt service routine
  * @param  None.
  * @retval None.
  */
void USART2_IRQHandler(void) 
{
	SCOM_USART2_IRQHandler();
}
void SCOM_USART2_IRQHandler(void) 
{
#if(SCOM1_USART_IRQn == 38)	
	 SCOM_USART_IRQHandler(SCOM1);
#endif	
	
#if(SCOM2_USART_IRQn == 38)	
	 SCOM_USART_IRQHandler(SCOM2);
#endif
	
#if(SCOM3_USART_IRQn == 38)	
	SCOM_USART_IRQHandler(SCOM3);
#endif

#if(SCOM4_USART_IRQn == 38)	
	SCOM_USART_IRQHandler(SCOM4);
#endif

#if(SCOM5_USART_IRQn == 38)	
	SCOM_USART_IRQHandler(SCOM5);
#endif	
}

/**
  * @brief  USART3 interrupt service routine
  * @param  None.
  * @retval None.
  */
void USART3_IRQHandler(void) 
{
	SCOM_USART3_IRQHandler();
}
void SCOM_USART3_IRQHandler(void) 
{
#if(SCOM1_USART_IRQn == 39)	
	 SCOM_USART_IRQHandler(SCOM1);
#endif	
	
#if(SCOM2_USART_IRQn == 39)	
	 SCOM_USART_IRQHandler(SCOM2);
#endif
	
#if(SCOM3_USART_IRQn == 39)	
	SCOM_USART_IRQHandler(SCOM3);
#endif	
	
#if(SCOM4_USART_IRQn == 39)	
	SCOM_USART_IRQHandler(SCOM4);
#endif		
	
#if(SCOM5_USART_IRQn == 39)	
	SCOM_USART_IRQHandler(SCOM5);
#endif		
	


	
}


/**
  * @brief  DMA1_Channel2 interrupt SendData service routine
  * @param  None.
  * @retval None.
  */
void DMA1_Channel2_IRQHandler(void)
{
	 SCOM_DMA1_Channel2_IRQHandler();
}
void SCOM_DMA1_Channel2_IRQHandler(void)
{

#if(SCOM1_tx_DMA_Channel_IRQn == 12)	
    SCOM_TX_DMA_IRQHandler(SCOM1);
#endif	
	
#if(SCOM2_tx_DMA_Channel_IRQn == 12)	
    SCOM_TX_DMA_IRQHandler(SCOM2);	
#endif
	
#if(SCOM3_tx_DMA_Channel_IRQn == 12)	
    SCOM_TX_DMA_IRQHandler(SCOM3);	
#endif
	
#if(SCOM4_tx_DMA_Channel_IRQn == 12)	
    SCOM_TX_DMA_IRQHandler(SCOM4);		
#endif	
}



/**
  * @brief  DMA1_Channel4 interrupt SendData  service routine
  * @param  None.
  * @retval None.
  */
void DMA1_Channel4_IRQHandler(void)
{
	 SCOM_DMA1_Channel4_IRQHandler();
}
void SCOM_DMA1_Channel4_IRQHandler(void)
{
#if(SCOM1_tx_DMA_Channel_IRQn == 14)	
    SCOM_TX_DMA_IRQHandler(SCOM1);
#endif	
	
#if(SCOM2_tx_DMA_Channel_IRQn == 14)	
    SCOM_TX_DMA_IRQHandler(SCOM2);	
#endif
	
#if(SCOM3_tx_DMA_Channel_IRQn == 14)	
    SCOM_TX_DMA_IRQHandler(SCOM3);	
#endif		

#if(SCOM4_tx_DMA_Channel_IRQn == 14)	
    SCOM_TX_DMA_IRQHandler(SCOM4);	
#endif		
}

/**
  * @brief  DMA1_Channel7 interrupt SendData service routine
  * @param  None.
  * @retval None.
  */
void DMA1_Channel7_IRQHandler(void)
{
	 SCOM_DMA1_Channel7_IRQHandler();
}
void SCOM_DMA1_Channel7_IRQHandler(void)
{
#if(SCOM1_tx_DMA_Channel_IRQn == 17)	
    SCOM_TX_DMA_IRQHandler(SCOM1);
#endif	
	
#if(SCOM2_tx_DMA_Channel_IRQn == 17)	
    SCOM_TX_DMA_IRQHandler(SCOM2);	
#endif
	
#if(SCOM3_tx_DMA_Channel_IRQn == 17)	
    SCOM_TX_DMA_IRQHandler(SCOM3);	
#endif

#if(SCOM4_tx_DMA_Channel_IRQn == 17)	
    SCOM_TX_DMA_IRQHandler(SCOM4);	
#endif	

}

/**
  * @}
  */

/********* (C) COPYRIGHT 2014 HANGZHOU NORMAL UNIVERSITY   *****END OF FILE****/
