/**
  ******************************************************************************
  * @file    stm32f10x_scom.h
  * @author  YUE JUNFENG
  * @version V1.1.2
  * @date    24/DEC/2018
  * @brief   This file provides all the SCOM channel init functions and data processing function 
	* @update   USART Interrupt support Openmv data receive
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __STM32F10X_SCOM_H
#define __STM32F10X_SCOM_H	

/* Includes ------------------------------------------------------------------*/	 

#include "stm32f10x.h" 	
#include "stm32f10x_dma.h"
#include <string.h>


	
	/******************************************************************************************
	 // Define a variable of SCOM_InitTypeDef_Struct type
	 SCOM_InitTypeDef_Struct  SCOM_InitStruct;
	 // Define a variable of SCOM_READ_DATA_Struct type
	 SCOM_READ_DATA_Struct *pdata;
	 // Define a variable of SCOM_WRITE_DATA_Struct type
	 SCOM_WRITE_DATA_Struct *pdata;
	 // Define a variable of CURRENT_OPENMVDATA_Struct type
	 CURRENT_OPENMVDATA_Struct *pdata;
	 
	 // Enable GPIOx which are related to SCOMx to APB2 or APB1 peripheral clock. 
	 // GPIOx   are defined at stm32f10x_scom.h and stated at stm32f10x_scom_hardwaretable.c,
   //	user must modify them for different Applications.
	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE);
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USARTx,ENABLE);
	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	


 		SCOM_StructInit(&SCOM_InitStruct);
		
		// Set the value of SCOM_InitStruct. 
		
		// According to the value of SCOM_InitStruct, initialize SCOMx.
		SCOM_Init(SCOMx,&SCOM_InitStruct);
		
		
		
	
*******************************************************************************************/


/* ----------------------------- SCOM_Parameter_define ------------------------------- */
#define SCOM_USART_PREEMPTIONPRIORITY_DEFAULT					2
#define	SCOM_USART_SUBPRIORITY_DEFAULT								2
#define SCOM_DMA_PREEMPTIONPRIORITY_DEFAULT						3
#define	SCOM_DMA_SUBPRIORITY_DEFAULT									3

/* ----------------- SCOM_InitTypeDef_Struct Default_Value define -------------------- */
#define	ENABLE_SCOM_DEFAULT							ENABLE
#define	ENABLE_DMA_DEFAULT							ENABLE

#define	SCOM_BAUDRATE_DEFAULT						115200
#define	SCOM_WORDLENGTH_DEFAULT					USART_WordLength_8b
#define	SCOM_STOPBIT_DEFAULT						USART_StopBits_1
#define	SCOM_PARITY_DEFAULT							USART_Parity_No /*USART_Parity_No*/
#define	SCOM_USARTMODE_DEFAULT					USART_Mode_Rx	|	USART_Mode_Tx
#define	SCOM_HARDWARELOWCONTROL_DEFAULT	USART_HardwareFlowControl_None


/* ----------------------------- SCOM_DataPacket_define ------------------------------- */
#define RX_BUFFER_MAX_SIZE                       10
#define TX_BUFFER_MAX_SIZE                       10

/* Define expected head title for DataPacket from OpenMv uart */
#define TITLE1 0xAA
#define TITLE2 0xAE


/** 
  * @brief    SCOM SCOM_READ_DATA structure definition
  */
	
typedef struct __SCOM_READ_DATA_Struct
{  
	 uint8_t      len;
   uint8_t      data[RX_BUFFER_MAX_SIZE];
}  SCOM_READ_DATA_Struct;


/** 
  * @brief    SCOM SCOM_WRITE_DATA structure definition
  */	

typedef struct __SCOM_WRITE_DATA_Struct
{  
	 uint8_t      len;
   uint8_t      data[TX_BUFFER_MAX_SIZE];
}  SCOM_WRITE_DATA_Struct;

/** 
  * @brief    SCOM SCOM_RX_Buffer structure definition
  */


typedef struct __SCOM_FrameByte_Struct
{  
	 FlagStatus      FrameByteFresh;
   uint8_t         FrameByteNum;
} SCOM_FrameByte_Struct;

/** 
  * @brief    SCOM OPENMVDATA structure definition
  */
typedef struct __CURRENT_OPENMVDATA_Struct
{  
	 int theta_err;
	 int	rho_err;
	
}CURRENT_OPENMVDATA_Struct;

/** 
  * @brief  SCOM Register structure definition   
  */

typedef struct __SCOMRegister_TypeDef_Struct
{
		FunctionalState                enable_SCOM;
		FunctionalState                enable_DMA;
		USART_InitTypeDef              usartParm;		
	
		SCOM_READ_DATA_Struct*         rx_Buffer;			
    FlagStatus                     USART_rxEndFlag;						
		SCOM_WRITE_DATA_Struct*        tx_Buffer;
		FlagStatus                     USART_txStartFlag;  	
    SCOM_FrameByte_Struct          Frame_ByteNum;	
	
} SCOMRegister_TypeDef_Struct;

/** 
  * @brief  SCOM Hardware structure definition   
  */


typedef struct __SCOMHardware_TypeDef_Struct
{
	
		// USART Hardware
		USART_TypeDef*                USARTx;
		uint8_t                       USARTx_IRQn;		

		GPIO_TypeDef *                tx_GPIOx;
		uint16_t                      tx_GPIO_Pin;
			
		GPIO_TypeDef *                rx_GPIOx;
		uint16_t                      rx_GPIO_Pin;
		// DMA  Hardware 
		DMA_Channel_TypeDef *         rx_DMAy_Channelx;
		uint8_t                       rx_DMAy_Channelx_IRQn;	
		DMA_Channel_TypeDef *         tx_DMAy_Channelx;
		uint8_t                       tx_DMAy_Channelx_IRQn;

	
} SCOMHardware_TypeDef_Struct;


/** 
  * @brief  SCOM Type structure definition 
  */

typedef struct __SCOM_TypeDef_Struct
{
	
	 SCOMHardware_TypeDef_Struct *SCOM_Hardware;	
	 SCOMRegister_TypeDef_Struct SCOM_Operating;

}SCOM_TypeDef_Struct;

/** 
  * @brief  SCOM Init structure definition 
  */

typedef struct __SCOM_InitTypeDef_Struct
{

		FunctionalState             enable_SCOM;

		FunctionalState             enable_DMA;
			
		USART_InitTypeDef           usartInitParm;
			
		uint8_t	                    SCOM_USART_PreemptionPriority;
		uint8_t	                    SCOM_USART_SubPriority;
			
		uint8_t	                    SCOM_DMA_PreemptionPriority;
		uint8_t	                    SCOM_DMA_SubPriority;
	
	
}SCOM_InitTypeDef_Struct;

/** @defgroup SCOM_Exported_Macros
  * @{
  */



/* ------------------------------- SCOM_nums_define --------------------------------- */
/* It is not permitted to modify SCOM_NUM_MAX greater than 3 */
#define SCOM_NUM_MAX                           3
/* It is not permitted to modify SCOM_NUM_USED greater than SCOM_NUM_MAX */
#define SCOM_NUM_USED                          3


#if(SCOM_NUM_MAX>3)
#error SCOM_NUM_MAX BIGGER THAN 3
#endif

#if(SCOM_NOW_NUM>SCOM_NUM_MAX)
#error SCOM_NUM_USED BIGGER THAN SCOM_NUM_USED
#endif

#if(SCOM_NUM_USED>=1)
extern SCOM_TypeDef_Struct SCOM1_BASE;
#define SCOM1                         (&SCOM1_BASE)      /* SCOM1 selected */
#define SCOM1_USART                   USART3 
#define SCOM1_USART_IRQn		          39  
#define SCOM1_tx_GPIO                 GPIOB
#define SCOM1_tx_GPIO_Pin	            GPIO_Pin_10
#define SCOM1_rx_GPIO                 GPIOB
#define SCOM1_rx_GPIO_Pin             GPIO_Pin_11
#define SCOM1_rx_DMA_Channel          DMA1_Channel3
#define SCOM1_rx_DMA_Channel_IRQn     13  	
#define SCOM1_tx_DMA_Channel          DMA1_Channel2
#define SCOM1_tx_DMA_Channel_IRQn     12          
#else
#define SCOM1                         NULL
#define SCOM1_USART                   NULL 
#define SCOM1_USART_IRQn		          NULL
#define SCOM1_tx_GPIO                 NULL
#define SCOM1_tx_GPIO_Pin	            NULL
#define SCOM1_rx_GPIO                 NULL
#define SCOM1_rx_GPIO_Pin             NULL
#define SCOM1_rx_DMA_Channel          NULL
#define SCOM1_rx_DMA_Channel_IRQn     NULL	
#define SCOM1_tx_DMA_Channel          NULL
#define SCOM1_tx_DMA_Channel_IRQn     NULL
#endif

#if(SCOM_NUM_USED>=2)
extern SCOM_TypeDef_Struct SCOM2_BASE;
#define SCOM2                        (&SCOM2_BASE)      /* SCOM2 selected */
#define SCOM2_USART                   USART2 
#define SCOM2_USART_IRQn		          38   
#define SCOM2_tx_GPIO                 GPIOA
#define SCOM2_tx_GPIO_Pin	            GPIO_Pin_2
#define SCOM2_rx_GPIO                 GPIOA
#define SCOM2_rx_GPIO_Pin             GPIO_Pin_3
#define SCOM2_rx_DMA_Channel          DMA1_Channel6
#define SCOM2_rx_DMA_Channel_IRQn     16  	
#define SCOM2_tx_DMA_Channel          DMA1_Channel7
#define SCOM2_tx_DMA_Channel_IRQn     17  
#else
#define SCOM2                         NULL
#define SCOM2_USART                   NULL 
#define SCOM2_USART_IRQn		          NULL
#define SCOM2_tx_GPIO                 NULL
#define SCOM2_tx_GPIO_Pin	            NULL
#define SCOM2_rx_GPIO                 NULL
#define SCOM2_rx_GPIO_Pin             NULL
#define SCOM2_rx_DMA_Channel          NULL
#define SCOM2_rx_DMA_Channel_IRQn     NULL	
#define SCOM2_tx_DMA_Channel          NULL
#define SCOM2_tx_DMA_Channel_IRQn     NULL
#endif

#if(SCOM_NUM_USED>=3)
extern SCOM_TypeDef_Struct SCOM3_BASE;
#define SCOM3                        (&SCOM3_BASE)      /* SCOM3 selected */
#define SCOM3_USART                   USART1 
#define SCOM3_USART_IRQn		          37  
#define SCOM3_tx_GPIO                 GPIOA
#define SCOM3_tx_GPIO_Pin	            GPIO_Pin_9
#define SCOM3_rx_GPIO                 GPIOA
#define SCOM3_rx_GPIO_Pin             GPIO_Pin_10
#define SCOM3_rx_DMA_Channel          DMA1_Channel5
#define SCOM3_rx_DMA_Channel_IRQn     15  	
#define SCOM3_tx_DMA_Channel          DMA1_Channel4
#define SCOM3_tx_DMA_Channel_IRQn     14  
#else
#define SCOM3                         NULL
#define SCOM3_USART                   NULL 
#define SCOM3_USART_IRQn		          NULL
#define SCOM3_tx_GPIO                 NULL
#define SCOM3_tx_GPIO_Pin	            NULL
#define SCOM3_rx_GPIO                 NULL
#define SCOM3_rx_GPIO_Pin             NULL
#define SCOM3_rx_DMA_Channel          NULL
#define SCOM3_rx_DMA_Channel_IRQn     NULL	
#define SCOM3_tx_DMA_Channel          NULL
#define SCOM3_tx_DMA_Channel_IRQn     NULL
#endif



    
/**
  * @}
  */
	
/** @defgroup SCOM_Exported_Functions
  * @{
  */
	
/**
  * @brief  USART1 Interrupt handler.
  * @param  None.
  * @retval None.
  */

void SCOM_USART1_IRQHandler(void);

/**
  * @brief  USART2 Interrupt handler.
  * @param  None.
  * @retval None.
  */
void SCOM_USART2_IRQHandler(void);

/**
  * @brief  USART3 Interrupt handler.
  * @param  None.
  * @retval None.
  */
void SCOM_USART3_IRQHandler(void);



/**
  * @brief  DMA1_Channel2 interrupt SendData service routine
  * @param  None.
  * @retval None.
  */
void SCOM_DMA1_Channel2_IRQHandler(void);

/**
  * @brief  DMA1_Channel3 interrupt SendData service routine
  * @param  None.
  * @retval None.
  */
void SCOM_DMA1_Channel3_IRQHandler(void);

/**
  * @brief  DMA1_Channel4 interrupt SendData service routine
  * @param  None.
  * @retval None.
  */
void SCOM_DMA1_Channel4_IRQHandler(void);

/**
  * @brief  DMA1_Channel5 interrupt SendData service routine
  * @param  None.
  * @retval None.
  */
void SCOM_DMA1_Channel5_IRQHandler(void);

/**
  * @brief  DMA1_Channel6 interrupt SendData service routine
  * @param  None.
  * @retval None.
  */
void SCOM_DMA1_Channel6_IRQHandler(void);

/**
  * @brief  DMA1_Channel7 interrupt SendData service routine
  * @param  None.
  * @retval None.
  */
void SCOM_DMA1_Channel7_IRQHandler(void);	


/**
  * @brief  Fills each SCOM_InitStruct member with its default value.
  * @param SCOM_InitStruct : pointer to a SCOM_InitTypeDef
  *   structure which will be initialized.
  * @retval : None
  */
void SCOM_StructInit(SCOM_InitTypeDef_Struct*SCOMx_InitStruct);

/**
  * @brief  Initializes the SCOMx  according to the specified
  *   parameters in the SCOM_InitStruct.
  * @param SCOMx: where x can be  1 to SCOM_NUM_USED  to select the SCOM.
  *        SCOM_NUM_USED is defined at stm32f10x_scom.h, can be 1 to SCOM_NUM_MAX, 
	*        and SCOM_NUM_MAX is also defined at stm32f10x_scom.h, can be 1 to 3.
  * @param SCOM_InitStruct : pointer to a SCOM_InitTypeDef_Struct
  *   structure which will be initialized.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus SCOM_Init(SCOM_TypeDef_Struct* SCOMx, SCOM_InitTypeDef_Struct* SCOMx_InitStruct);

/**
  * @brief  Enable the specified SCOM peripheral.
  * @param SCOMx: where x can be  1 to SCOM_NUM_USED  to select the SCOM.
  *        SCOM_NUM_USED is defined at stm32f10x_scom.h, can be 1 to SCOM_NUM_MAX, 
	*        and SCOM_NUM_MAX is also defined at stm32f10x_scom.h, can be 1 to 3.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus SCOM_On(SCOM_TypeDef_Struct* SCOMx);

/**
  * @brief  Disable the specified SCOM peripheral.
  * @param SCOMx: where x can be  1 to SCOM_NUM_USED  to select the SCOM.
  *        SCOM_NUM_USED is defined at stm32f10x_scom.h, can be 1 to SCOM_NUM_MAX, 
	*        and SCOM_NUM_MAX is also defined at stm32f10x_scom.h, can be 1 to 3.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus SCOM_Off(SCOM_TypeDef_Struct* SCOMx);

/**
  * @brief Read data from SCOMx to pdata of pointers..
  * @param SCOMx: where x can be  1 to SCOM_NUM_USED  to select the SCOM.
  *        SCOM_NUM_USED is defined at stm32f10x_scom.h, can be 1 to SCOM_NUM_MAX, 
	*        and SCOM_NUM_MAX is also defined at stm32f10x_scom.h, can be 1 to 3.
  * @param pdata: pointer to a SCOM_READ_DATA_Struct structure.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus SCOM_ReadData(SCOM_TypeDef_Struct*SCOMx, SCOM_READ_DATA_Struct* pdata);

/**
  * @brief Write data from the array of pointers  to SCOMx .
  * @param SCOMx: where x can be  1 to SCOM_NUM_USED  to select the SCOM.
  *        SCOM_NUM_USED is defined at stm32f10x_scom.h, can be 1 to SCOM_NUM_MAX, 
	*        and SCOM_NUM_MAX is also defined at stm32f10x_scom.h, can be 1 to 3.
  * @param pdata: pointer to a SCOM_WRITE_DATA_Struct structure.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus SCOM_WriteData(SCOM_TypeDef_Struct* SCOMx, SCOM_WRITE_DATA_Struct* pdata);

/**
  * @brief Send data from the array of buffer to SCOMx .
  * @param SCOMx: where x can be  1 to SCOM_NUM_USED  to select the SCOM.
  *        SCOM_NUM_USED is defined at stm32f10x_scom.h, can be 1 to SCOM_NUM_MAX, 
	*        and SCOM_NUM_MAX is also defined at stm32f10x_scom.h, can be 1 to 3.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus	SCOM_USART_SendData(SCOM_TypeDef_Struct	*SCOMx);

/**
  * @brief Receive status of complete frame.
  * @param SCOMx: where x can be  1 to SCOM_NUM_USED  to select the SCOM.
  *        SCOM_NUM_USED is defined at stm32f10x_scom.h, can be 1 to SCOM_NUM_MAX, 
	*        and SCOM_NUM_MAX is also defined at stm32f10x_scom.h, can be 1 to 3.
  * @param FrameByteNum: pointer to a SCOM_FrameByte_Struct structure.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus SCOM_Read_FrameByteNum(SCOM_TypeDef_Struct*SCOMx, SCOM_FrameByte_Struct* FrameByteNum);
/**
  * @}
  */
/** @defgroup SCOM_Internaled_Functions
  * @{
  */

/**
  * @brief Scom interrupt service routine(Receive interrupt and send interrupt), 
	         should be called by some USART interrupt service function.
  * @param SCOMx: where x can be  1 to SCOM_NUM_USED  to select the SCOM.
  *        SCOM_NUM_USED is defined at stm32f10x_scom.h, can be 1 to SCOM_NUM_MAX, 
	*        and SCOM_NUM_MAX is also defined at stm32f10x_scom.h, can be 1 to 3.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus SCOM_USART_IRQHandler(SCOM_TypeDef_Struct* SCOMx);


/**
  * @brief Detect typical flowing data and get ready to receive ONLY one byte(8 bit) for each interrupt if the head of frame is as default;
	* @param SCOMx: where x can be  1 to SCOM_NUM_USED  to select the SCOM.
  *        SCOM_NUM_USED is defined at stm32f10x_scom.h, can be 1 to SCOM_NUM_MAX, 
	*        and SCOM_NUM_MAX is also defined at stm32f10x_scom.h, can be 1 to 3.
  * @param Data 1 byte =8 bit received.
	* @param Check two head titleof frame set by user.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus Optical_Flow_Receive_Prepare(SCOM_TypeDef_Struct *SCOMx,uint8_t byte,uint8_t TITLE,uint8_t HEAD);

/**
  * @brief Handle with INT type divided as 4 byte then add up into int composition
	* @param SCOMx: where x can be  1 to SCOM_NUM_USED  to select the SCOM.
  *        SCOM_NUM_USED is defined at stm32f10x_scom.h, can be 1 to SCOM_NUM_MAX, 
	*        and SCOM_NUM_MAX is also defined at stm32f10x_scom.h, can be 1 to 3.
  * @param pdata: pointer to a CURRENT_OPENMVDATA_Struct structure.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus Data_Processing(SCOM_TypeDef_Struct *SCOMx,CURRENT_OPENMVDATA_Struct *data);

/**
  * @brief Read error adjusting data from openmv data according to Current openmv data BUFFER;
	* @param SCOMx: where x can be  1 to SCOM_NUM_USED  to select the SCOM.
  *        SCOM_NUM_USED is defined at stm32f10x_scom.h, can be 1 to SCOM_NUM_MAX, 
	*        and SCOM_NUM_MAX is also defined at stm32f10x_scom.h, can be 1 to 3.
  * @param pdata: pointer to a CURRENT_OPENMVDATA_Struct structure.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus Read_OpenmvProcessingData(SCOM_TypeDef_Struct *SCOMx,CURRENT_OPENMVDATA_Struct *pdata);
/**
  * @brief DMA interrupt Receive datas Program, should be called by some DMA interrupt service function.
  * @param SCOMx: where x can be  1 to SCOM_NUM_USED  to select the SCOM.
  *        SCOM_NUM_USED is defined at stm32f10x_scom.h, can be 1 to SCOM_NUM_MAX, 
	*        and SCOM_NUM_MAX is also defined at stm32f10x_scom.h, can be 1 to 3.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus SCOM_RX_DMA_IRQHandler(SCOM_TypeDef_Struct* SCOMx);

/**
  * @brief DMA interrupt send datas Program, should be called by some DMA interrupt service function.
  * @param SCOMx: where x can be  1 to SCOM_NUM_USED  to select the SCOM.
  *        SCOM_NUM_USED is defined at stm32f10x_scom.h, can be 1 to SCOM_NUM_MAX, 
	*        and SCOM_NUM_MAX is also defined at stm32f10x_scom.h, can be 1 to 3.
  * @retval : ERROR or SUCCESS.
  */
ErrorStatus	SCOM_TX_DMA_IRQHandler(SCOM_TypeDef_Struct *SCOMx);	

ErrorStatus Read_RingBuffer(uint8_t *pdata);
ErrorStatus Write_RingBuffer(uint8_t data);
/**
  * @}
  */

#endif /*__STM32F10X_SCOM_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/********* (C) COPYRIGHT 2014 HANGZHOU NORMAL UNIVERSITY SHAOGUOJIAN *****END OF FILE****/
