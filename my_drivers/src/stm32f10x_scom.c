/*------------------------------- Includes -----------------------------------*/
#include "stm32f10x_scom.h"
#include "stm32f10x_gpio.h"

#define BUFFER_MAX  128								
typedef struct __SCOM_RingBuffer_Struct
{
	u16 Head;
	u16 Tail;
	u16 Length;
	uint8_t RingBuf[BUFFER_MAX];
}SCOM_RingBuffer_Struct;


extern SCOMHardware_TypeDef_Struct SCOM_Hardware[SCOM_NUM_MAX];
/*------------------------------- SCOM_BASE -----------------------------------*/
SCOM_TypeDef_Struct 							SCOM1_BASE,SCOM2_BASE,SCOM3_BASE;

static uint32_t										__SCOM_CPU_PRIMASK_REG_SAVE_VALUE;

SCOM_READ_DATA_Struct							SCOM3_rxDataBuffer;
SCOM_WRITE_DATA_Struct						SCOM3_txDataBuffer;

SCOM_READ_DATA_Struct							SCOM2_rxDataBuffer;
SCOM_WRITE_DATA_Struct						SCOM2_txDataBuffer;

SCOM_READ_DATA_Struct							SCOM1_rxDataBuffer;
SCOM_WRITE_DATA_Struct						SCOM1_txDataBuffer;

CURRENT_OPENMVDATA_Struct       	OpenMV_DataBuffer;



static __INLINE void __SCOM_Disable_IRQ(void);
static __INLINE void __SCOM_Restore_IRQ(void);




SCOM_READ_DATA_Struct												read_data;
SCOM_FrameByte_Struct 											FrameByteNum;

SCOM_RingBuffer_Struct                			RingBuffer;
ErrorStatus Write_RingBuffer(uint8_t data)
{
	if(RingBuffer.Length>=BUFFER_MAX )return ERROR;
	
	RingBuffer.RingBuf[RingBuffer.Tail]=data;
	RingBuffer.Tail=(RingBuffer.Tail+1)%BUFFER_MAX;
	RingBuffer.Length++;
	return SUCCESS;
}
ErrorStatus Read_RingBuffer(uint8_t *pdata)
{
	if(RingBuffer.Length==0)return ERROR;
	*pdata=RingBuffer.RingBuf[RingBuffer.Head];
	RingBuffer.Head=(RingBuffer.Head+1)%BUFFER_MAX;
	RingBuffer.Length--;
}


void SCOM_StructInit(SCOM_InitTypeDef_Struct*SCOMx_InitStruct)
{
	SCOMx_InitStruct->enable_SCOM														=	ENABLE_SCOM_DEFAULT;
	SCOMx_InitStruct->enable_DMA														=	ENABLE_DMA_DEFAULT;
	
	SCOMx_InitStruct->usartInitParm.USART_BaudRate					=	SCOM_BAUDRATE_DEFAULT;
	SCOMx_InitStruct->usartInitParm.USART_WordLength				=	SCOM_WORDLENGTH_DEFAULT;
	SCOMx_InitStruct->usartInitParm.USART_StopBits					=	SCOM_STOPBIT_DEFAULT;
	SCOMx_InitStruct->usartInitParm.USART_Parity						=	SCOM_PARITY_DEFAULT	;
	SCOMx_InitStruct->usartInitParm.USART_Mode							=	SCOM_USARTMODE_DEFAULT;
	SCOMx_InitStruct->usartInitParm.USART_HardwareFlowControl	=	SCOM_HARDWARELOWCONTROL_DEFAULT;
	
	SCOMx_InitStruct->SCOM_USART_PreemptionPriority					=	SCOM_USART_PREEMPTIONPRIORITY_DEFAULT;
	SCOMx_InitStruct->SCOM_USART_SubPriority								=	SCOM_USART_SUBPRIORITY_DEFAULT;
	SCOMx_InitStruct->SCOM_DMA_PreemptionPriority						=	SCOM_DMA_PREEMPTIONPRIORITY_DEFAULT;
	SCOMx_InitStruct->SCOM_DMA_SubPriority									=	SCOM_DMA_SUBPRIORITY_DEFAULT;

	RingBuffer.Head=0;
	RingBuffer.Length=0;
	RingBuffer.Tail=0;
	
}

ErrorStatus SCOM_Init(SCOM_TypeDef_Struct* SCOMx, SCOM_InitTypeDef_Struct* SCOMx_InitStruct)
{
	
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	 
	if((SCOMx	==	NULL)||(SCOMx_InitStruct	==	NULL))
		return ERROR;
	if(SCOMx==SCOM3)
		SCOM3_BASE.SCOM_Hardware	=	&SCOM_Hardware[2];
	else if(SCOMx==SCOM2)
		SCOM2_BASE.SCOM_Hardware	=	&SCOM_Hardware[1];
	else
		return ERROR;
	
	SCOMx->SCOM_Operating.enable_SCOM	=	SCOMx_InitStruct->enable_SCOM;
	
	if((SCOMx->SCOM_Hardware->tx_DMAy_Channelx==NULL)||
		(SCOMx->SCOM_Hardware->rx_DMAy_Channelx==NULL))
	SCOMx->SCOM_Operating.enable_DMA=DISABLE;
	else 
		SCOMx->SCOM_Operating.enable_DMA=SCOMx_InitStruct->enable_DMA;
	
	if(SCOMx==SCOM3)
	{
		SCOMx->SCOM_Operating.rx_Buffer=&SCOM3_rxDataBuffer;
	}
	else if(SCOMx==SCOM2)
	{
		SCOMx->SCOM_Operating.rx_Buffer=&SCOM2_rxDataBuffer;
	}
	else 
		return ERROR;
	
	
	SCOMx->SCOM_Operating.rx_Buffer->len=0;
	
	if(SCOMx==SCOM3)
	SCOMx->SCOM_Operating.tx_Buffer=&SCOM3_txDataBuffer;
	else if(SCOMx==SCOM2)
	SCOMx->SCOM_Operating.tx_Buffer=&SCOM2_txDataBuffer;
	else 
		return ERROR;
	
	SCOMx->SCOM_Operating.USART_txStartFlag							=RESET;
	
	
	SCOMx->SCOM_Operating.Frame_ByteNum.FrameByteFresh	=RESET;
	SCOMx->SCOM_Operating.Frame_ByteNum.FrameByteNum		=0;
	SCOMx->SCOM_Operating.USART_rxEndFlag								=RESET;
	
	

	
	GPIO_StructInit(&GPIO_InitStructure);
	
	//USARTx_TX			初始化
	GPIO_InitStructure.GPIO_Pin = SCOMx->SCOM_Hardware->tx_GPIO_Pin; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_Init(SCOMx->SCOM_Hardware->tx_GPIOx, &GPIO_InitStructure);
	
	//USARTx_RX	  初始化
  GPIO_InitStructure.GPIO_Pin = SCOMx->SCOM_Hardware->rx_GPIO_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
  GPIO_Init(SCOMx->SCOM_Hardware->rx_GPIOx, &GPIO_InitStructure);
	
	


	//init usart parameter
	SCOMx->SCOM_Operating.usartParm.USART_BaudRate								=SCOMx_InitStruct->usartInitParm.USART_BaudRate;
	SCOMx->SCOM_Operating.usartParm.USART_HardwareFlowControl			=SCOMx_InitStruct->usartInitParm.USART_HardwareFlowControl;
	SCOMx->SCOM_Operating.usartParm.USART_Mode										=SCOMx_InitStruct->usartInitParm.USART_Mode;
	SCOMx->SCOM_Operating.usartParm.USART_Parity									=SCOMx_InitStruct->usartInitParm.USART_Parity;
	SCOMx->SCOM_Operating.usartParm.USART_StopBits								=SCOMx_InitStruct->usartInitParm.USART_StopBits;
	SCOMx->SCOM_Operating.usartParm.USART_WordLength							=SCOMx_InitStruct->usartInitParm.USART_WordLength;
		
	
	
	USART_Init(SCOMx->SCOM_Hardware->USARTx,&SCOMx_InitStruct->usartInitParm);
	


	
	if(SCOMx->SCOM_Operating.enable_DMA==ENABLE)
	{
		//DMA send configuration 
		NVIC_InitStructure.NVIC_IRQChannel										=SCOMx->SCOM_Hardware->tx_DMAy_Channelx_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=SCOMx_InitStruct->SCOM_DMA_PreemptionPriority;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority					=SCOMx_InitStruct->SCOM_DMA_SubPriority;
		NVIC_InitStructure.NVIC_IRQChannelCmd									=ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
	

	
	
	
	
	if((SCOMx->SCOM_Hardware->rx_DMAy_Channelx	!=NULL)&(SCOMx->SCOM_Hardware->tx_DMAy_Channelx!=NULL))
	{
		if(SCOMx->SCOM_Operating.enable_DMA==ENABLE)
		{
			DMA_DeInit(SCOMx->SCOM_Hardware->rx_DMAy_Channelx);
			DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)(&SCOMx->SCOM_Hardware->USARTx->DR);
			DMA_InitStructure.DMA_MemoryBaseAddr		=(uint32_t)(SCOMx->SCOM_Operating.rx_Buffer->data);
			DMA_InitStructure.DMA_DIR								=DMA_DIR_PeripheralSRC;
			DMA_InitStructure.DMA_BufferSize				=RX_BUFFER_MAX_SIZE;
			DMA_InitStructure.DMA_PeripheralInc			=DMA_PeripheralInc_Disable;			
			DMA_InitStructure.DMA_MemoryInc 				=DMA_MemoryInc_Enable;			
			DMA_InitStructure.DMA_PeripheralDataSize= DMA_PeripheralDataSize_Byte;
			DMA_InitStructure.DMA_MemoryDataSize		=DMA_MemoryDataSize_Byte;
			DMA_InitStructure.DMA_Mode							=DMA_Mode_Normal;
			DMA_InitStructure.DMA_Priority					=DMA_Priority_VeryHigh;
			DMA_InitStructure.DMA_M2M								=DMA_M2M_Disable;
			
			DMA_Init(SCOMx->SCOM_Hardware->rx_DMAy_Channelx,&DMA_InitStructure);		
			DMA_ITConfig(SCOMx->SCOM_Hardware->rx_DMAy_Channelx,DMA_IT_TC,ENABLE);
			DMA_SetCurrDataCounter(SCOMx->SCOM_Hardware->rx_DMAy_Channelx,RX_BUFFER_MAX_SIZE);
			
			DMA_DeInit(SCOMx->SCOM_Hardware->tx_DMAy_Channelx);
			
			DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)(&SCOMx->SCOM_Hardware->USARTx->DR);
			DMA_InitStructure.DMA_MemoryBaseAddr		=(uint32_t)(SCOMx->SCOM_Operating.tx_Buffer->data);
			DMA_InitStructure.DMA_DIR								=DMA_DIR_PeripheralDST;
			DMA_InitStructure.DMA_BufferSize				=TX_BUFFER_MAX_SIZE;
			DMA_Init(SCOMx->SCOM_Hardware->tx_DMAy_Channelx,&DMA_InitStructure);
			
			DMA_ITConfig(SCOMx->SCOM_Hardware->tx_DMAy_Channelx,DMA_IT_TC,ENABLE);
			DMA_SetCurrDataCounter(SCOMx->SCOM_Hardware->tx_DMAy_Channelx,0);
		}
	}

	
	if((SCOMx->SCOM_Hardware->rx_DMAy_Channelx!=NULL)&(SCOMx->SCOM_Hardware->tx_DMAy_Channelx!=NULL))
	{
		if(SCOMx->SCOM_Operating.enable_DMA==DISABLE)
		{
			DMA_Cmd(SCOMx->SCOM_Hardware->rx_DMAy_Channelx,DISABLE);
			DMA_Cmd(SCOMx->SCOM_Hardware->tx_DMAy_Channelx,DISABLE);
		}
		else 
		{
			DMA_Cmd(SCOMx->SCOM_Hardware->rx_DMAy_Channelx,ENABLE);
			DMA_Cmd(SCOMx->SCOM_Hardware->tx_DMAy_Channelx,ENABLE);
		}
	}
	
	//interrupt USART1
		NVIC_InitStructure.NVIC_IRQChannel										=SCOMx->SCOM_Hardware->USARTx_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=SCOMx_InitStruct->SCOM_USART_PreemptionPriority;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority					=SCOMx_InitStruct->SCOM_USART_SubPriority;
		NVIC_InitStructure.NVIC_IRQChannelCmd									=ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	
		if(SCOMx->SCOM_Operating.enable_DMA==DISABLE)
	{
		USART_ITConfig(SCOMx->SCOM_Hardware->USARTx,USART_IT_RXNE,ENABLE);
		USART_ITConfig(SCOMx->SCOM_Hardware->USARTx,USART_IT_IDLE,DISABLE);
		USART_ITConfig(SCOMx->SCOM_Hardware->USARTx,USART_IT_TC,DISABLE);		
	}
	else 
	{
		USART_ITConfig(SCOMx->SCOM_Hardware->USARTx,USART_IT_RXNE,DISABLE);
		USART_ITConfig(SCOMx->SCOM_Hardware->USARTx,USART_IT_IDLE,ENABLE);
		USART_ITConfig(SCOMx->SCOM_Hardware->USARTx,USART_IT_TC,DISABLE);		
		if((SCOMx->SCOM_Hardware->rx_DMAy_Channelx	!=NULL)&(SCOMx->SCOM_Hardware->tx_DMAy_Channelx!=NULL))
		{
			USART_DMACmd(SCOMx->SCOM_Hardware->USARTx,USART_DMAReq_Rx,ENABLE);
			USART_DMACmd(SCOMx->SCOM_Hardware->USARTx,USART_DMAReq_Tx,ENABLE);
		}
		
	}	

	
	return SUCCESS;


}
			
			
		

	

ErrorStatus SCOM_On(SCOM_TypeDef_Struct* SCOMx)
{
	if(SCOMx	==	NULL)	return ERROR;
	USART_Cmd(SCOMx->SCOM_Hardware->USARTx,ENABLE);
	
	if((SCOMx->SCOM_Hardware->rx_DMAy_Channelx	!=NULL)&(SCOMx->SCOM_Hardware->tx_DMAy_Channelx!=NULL))
	{
		if(SCOMx->SCOM_Operating.enable_DMA==ENABLE)
		{
			DMA_Cmd(SCOMx->SCOM_Hardware->rx_DMAy_Channelx,ENABLE);
		}
	}
	return SUCCESS;
}

ErrorStatus SCOM_Off(SCOM_TypeDef_Struct* SCOMx)
{
	if(SCOMx	==	NULL)	return ERROR;
	USART_Cmd(SCOMx->SCOM_Hardware->USARTx,DISABLE);
	
	if((SCOMx->SCOM_Hardware->rx_DMAy_Channelx	!=NULL)&(SCOMx->SCOM_Hardware->tx_DMAy_Channelx!=NULL))
	{
		if(SCOMx->SCOM_Operating.enable_DMA==ENABLE)
		{
			DMA_Cmd(SCOMx->SCOM_Hardware->rx_DMAy_Channelx,DISABLE);
		}
	}
	return SUCCESS;
}


ErrorStatus SCOM_ReadData(SCOM_TypeDef_Struct*SCOMx, SCOM_READ_DATA_Struct* pdata)
{
	if((SCOMx==NULL)||(pdata==NULL)) return ERROR;
	
	if(SCOMx->SCOM_Operating.USART_rxEndFlag==SET)
	{
		
		if(SCOMx==SCOM1)
		{
			__SCOM_Disable_IRQ();
			pdata->len=SCOM1_rxDataBuffer.len;
			memcpy(pdata->data,SCOM1_rxDataBuffer.data,pdata->len);
			SCOMx->SCOM_Operating.USART_rxEndFlag=RESET;
			__SCOM_Restore_IRQ();
			
		}
		
		else if(SCOMx==SCOM2)
		{
			__SCOM_Disable_IRQ();
			pdata->len=SCOM2_rxDataBuffer.len;
			memcpy(pdata->data,SCOM2_rxDataBuffer.data,pdata->len);
			SCOMx->SCOM_Operating.USART_rxEndFlag=RESET;
			__SCOM_Restore_IRQ();
			
		}
		
		if(SCOMx==SCOM3)
		{
			
			__SCOM_Disable_IRQ();
			pdata->len=SCOM3_rxDataBuffer.len;
			memcpy(pdata->data,SCOMx->SCOM_Operating.rx_Buffer->data,SCOMx->SCOM_Operating.rx_Buffer->len);
			SCOMx->SCOM_Operating.USART_rxEndFlag=RESET;
			__SCOM_Restore_IRQ();
		}

		else 
			return ERROR;
	}
		return SUCCESS;
	}

	


ErrorStatus SCOM_Read_FrameByteNum(SCOM_TypeDef_Struct*SCOMx, SCOM_FrameByte_Struct* FrameByteNum)
{
	if(SCOMx==NULL)	return ERROR;
	
	if(SCOMx->SCOM_Operating.Frame_ByteNum.FrameByteFresh==SET)
	{
		__SCOM_Disable_IRQ();
		FrameByteNum->FrameByteNum		=SCOMx->SCOM_Operating.Frame_ByteNum.FrameByteNum;
		SCOMx->SCOM_Operating.Frame_ByteNum.FrameByteNum=0;
		FrameByteNum->FrameByteFresh	=	SET;
		__SCOM_Restore_IRQ();
	}
	else
	{
		__SCOM_Disable_IRQ();
		FrameByteNum->FrameByteFresh=RESET;
		FrameByteNum->FrameByteNum=0;
		__SCOM_Restore_IRQ();
	}
	return SUCCESS;
}



ErrorStatus	SCOM_USART_SendData(SCOM_TypeDef_Struct	*SCOMx)
{
	
	
	uint8_t		data;
	
	static uint8_t sendNum1=0;
	static uint8_t sendNum2=0;
	static uint8_t sendNum3=0;

	
	if(SCOMx==SCOM1)
	{
		data=SCOMx->SCOM_Operating.tx_Buffer->data[sendNum1];
		if(sendNum1<SCOMx->SCOM_Operating.tx_Buffer->len)
		{
			USART_SendData(SCOMx->SCOM_Hardware->USARTx,(uint16_t)data);
			sendNum1++;
			return SUCCESS;
		}
		else
		{
			SCOMx->SCOM_Operating.USART_txStartFlag	=	RESET;
			sendNum1	=	0;
			return SUCCESS;
		}
	}	
	else if(SCOMx	==SCOM2)
	{
		data=SCOMx->SCOM_Operating.tx_Buffer->data[sendNum2];
		if(sendNum2<SCOMx->SCOM_Operating.tx_Buffer->len)
		{
			USART_SendData(SCOMx->SCOM_Hardware->USARTx,(uint16_t)data);			
			sendNum2++;
			return SUCCESS;
		}
		else
		{
			SCOMx->SCOM_Operating.USART_txStartFlag	=	RESET;
			sendNum2	=	0;
			return SUCCESS;
		}
	}
	else if(SCOMx	==SCOM3)
	{
		data=SCOMx->SCOM_Operating.tx_Buffer->data[sendNum3];
		if(sendNum3<SCOMx->SCOM_Operating.tx_Buffer->len)
		{
			USART_SendData(SCOMx->SCOM_Hardware->USARTx,(uint16_t)data);
			SCOMx->SCOM_Operating.USART_txStartFlag	=	SET;
			sendNum3++;
			return SUCCESS;
		}
		else
		{
			SCOMx->SCOM_Operating.USART_txStartFlag	=	RESET;
			sendNum3	=	0;
			return SUCCESS;
		}
	}
	else return ERROR;
	
}


ErrorStatus	SCOM_DMA_USART_SendData(SCOM_TypeDef_Struct	*SCOMx)
{
	uint8_t cnt1,cnt2;
	
	cnt1=SCOMx->SCOM_Operating.tx_Buffer->len;
	if(cnt1>TX_BUFFER_MAX_SIZE)			cnt1=TX_BUFFER_MAX_SIZE;
	
	
	DMA_Cmd(SCOMx->SCOM_Hardware->tx_DMAy_Channelx,DISABLE);
	cnt2=DMA_GetCurrDataCounter(SCOMx->SCOM_Hardware->tx_DMAy_Channelx);
	DMA_Cmd(SCOMx->SCOM_Hardware->tx_DMAy_Channelx,ENABLE);
	if(cnt2<=0x00)
	{
		
		DMA_Cmd(SCOMx->SCOM_Hardware->tx_DMAy_Channelx,DISABLE);
		DMA_SetCurrDataCounter(SCOMx->SCOM_Hardware->tx_DMAy_Channelx,(uint16_t)cnt1);
		DMA_Cmd(SCOMx->SCOM_Hardware->tx_DMAy_Channelx,ENABLE);
		return SUCCESS;
	}
	 
	return ERROR;
	
	
}
ErrorStatus SCOM_WriteData(SCOM_TypeDef_Struct* SCOMx, SCOM_WRITE_DATA_Struct* pdata)
{
	if((SCOMx==NULL)||(pdata==NULL)) return ERROR;
	if(SCOMx->SCOM_Operating.USART_txStartFlag==SET)
	{
		
		if(SCOMx==SCOM1)
		{
			SCOM1_txDataBuffer.len	=	pdata->len;
			memcpy(SCOM1_txDataBuffer.data,pdata->data,pdata->len);
		}
		else if(SCOMx==SCOM2)
		{
			SCOM2_txDataBuffer.len	=	pdata->len;
			memcpy(SCOM2_txDataBuffer.data,pdata->data,pdata->len);
		}
		else if(SCOMx==SCOM3)
		{
			SCOM3_txDataBuffer.len	=	pdata->len;
			memcpy(SCOM3_txDataBuffer.data,pdata->data,pdata->len);
		}	
		else 
			return ERROR;
	}

	if(SCOMx->SCOM_Operating.enable_DMA==ENABLE)
	{
		if(SCOMx->SCOM_Operating.USART_txStartFlag==SET)
		{
			SCOMx->SCOM_Operating.USART_txStartFlag=RESET;
			SCOM_DMA_USART_SendData(SCOMx);
		}
	}
	else 
	{
		if(SCOMx->SCOM_Operating.USART_txStartFlag==SET)
		{
			SCOMx->SCOM_Operating.USART_txStartFlag=RESET;
			SCOM_USART_SendData(SCOMx);
		}
	}
	return SUCCESS;
}
			





ErrorStatus SCOM_USART_IRQHandler(SCOM_TypeDef_Struct* SCOMx)
{
	static			uint8_t SCOM1_ByteNum	=0;
	static			uint8_t SCOM2_ByteNum	=0;
	static			uint8_t SCOM3_ByteNum	=0;

	uint8_t Clear=Clear;
	
	uint8_t temp=0;
	

	if(SCOMx==NULL) return ERROR;
	
	if(SCOMx==SCOM1)
	{
			
		if(USART_GetITStatus(SCOMx->SCOM_Hardware->USARTx, USART_IT_RXNE) != RESET) //字节流中断
		{
			USART_ClearITPendingBit(SCOMx->SCOM_Hardware->USARTx,USART_IT_RXNE);
			
			SCOMx->SCOM_Operating.rx_Buffer->data[SCOM1_ByteNum++]=USART_ReceiveData(SCOMx->SCOM_Hardware->USARTx);
			SCOMx->SCOM_Operating.rx_Buffer->len++;
			
			SCOMx->SCOM_Operating.Frame_ByteNum.FrameByteNum++;
			
			if(SCOM1_ByteNum>=RX_BUFFER_MAX_SIZE)
			{			
				SCOMx->SCOM_Operating.Frame_ByteNum.FrameByteFresh=SET;
				SCOMx->SCOM_Operating.USART_rxEndFlag=SET;
				SCOM_Read_FrameByteNum(SCOMx, &FrameByteNum);
				SCOM1_ByteNum=0;
			}		
			temp=SCOMx->SCOM_Hardware->USARTx->DR;			
			Optical_Flow_Receive_Prepare(SCOMx,temp,TITLE1,TITLE2);					
		}
					
		if(USART_GetITStatus(SCOMx->SCOM_Hardware->USARTx, USART_IT_IDLE) != RESET) //帧中断 supported by dma 
		{
			
			Clear=SCOMx->SCOM_Hardware->USARTx->SR;
			Clear=SCOMx->SCOM_Hardware->USARTx->DR;
			
    	DMA_Cmd(SCOMx->SCOM_Hardware->rx_DMAy_Channelx,DISABLE);
			temp=RX_BUFFER_MAX_SIZE-DMA_GetCurrDataCounter(SCOMx->SCOM_Hardware->rx_DMAy_Channelx); //算出本帧长度
			SCOMx->SCOM_Operating.rx_Buffer->len=temp;
			
			SCOMx->SCOM_Operating.Frame_ByteNum.FrameByteFresh=SET;
			SCOMx->SCOM_Operating.Frame_ByteNum.FrameByteNum=temp;
			SCOM_Read_FrameByteNum(SCOMx, &FrameByteNum);
			
			SCOMx->SCOM_Operating.USART_rxEndFlag=SET;	

		}	
	}
	
	
	
		if(SCOMx==SCOM2)
	{
			
		if(USART_GetITStatus(SCOMx->SCOM_Hardware->USARTx, USART_IT_RXNE) != RESET) 
		{
			USART_ClearITPendingBit(SCOMx->SCOM_Hardware->USARTx,USART_IT_RXNE);
			
			SCOMx->SCOM_Operating.rx_Buffer->data[SCOM2_ByteNum++]=USART_ReceiveData(SCOMx->SCOM_Hardware->USARTx);
			SCOMx->SCOM_Operating.rx_Buffer->len++;
			
			SCOMx->SCOM_Operating.Frame_ByteNum.FrameByteNum++;
			
			if(SCOM2_ByteNum>=RX_BUFFER_MAX_SIZE)
			{			
				SCOMx->SCOM_Operating.Frame_ByteNum.FrameByteFresh=SET;
				SCOMx->SCOM_Operating.USART_rxEndFlag=SET;
				SCOM_Read_FrameByteNum(SCOMx, &FrameByteNum);
				SCOM2_ByteNum=0;
			}	
			
			temp=SCOMx->SCOM_Hardware->USARTx->DR;			
			Optical_Flow_Receive_Prepare(SCOMx,temp,TITLE1,TITLE2);		
			
		}
			
			
		if(USART_GetITStatus(SCOMx->SCOM_Hardware->USARTx, USART_IT_IDLE) != RESET) //support dma 
		{
			
			Clear=SCOMx->SCOM_Hardware->USARTx->SR;
			Clear=SCOMx->SCOM_Hardware->USARTx->DR;
			
    	DMA_Cmd(SCOMx->SCOM_Hardware->rx_DMAy_Channelx,DISABLE);
			temp=RX_BUFFER_MAX_SIZE-DMA_GetCurrDataCounter(SCOMx->SCOM_Hardware->rx_DMAy_Channelx); //算出本帧长度
			SCOMx->SCOM_Operating.rx_Buffer->len=temp;
			
			SCOMx->SCOM_Operating.Frame_ByteNum.FrameByteFresh=SET;
			SCOMx->SCOM_Operating.Frame_ByteNum.FrameByteNum=temp;
			SCOM_Read_FrameByteNum(SCOMx, &FrameByteNum);
			
			SCOMx->SCOM_Operating.USART_rxEndFlag=SET;	

		}	
	}
	
	
	if(SCOMx==SCOM3)
	{
		if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET) 
		{ 
			Clear = USART_ReceiveData(USART1);
			USART_ClearFlag(USART1, USART_FLAG_ORE); 
		}
		
		
		if(USART_GetITStatus(SCOMx->SCOM_Hardware->USARTx, USART_IT_RXNE) != RESET) //add function of receiving openmv data 
		{
			USART_ClearITPendingBit(SCOMx->SCOM_Hardware->USARTx,USART_IT_RXNE);
					
			SCOMx->SCOM_Operating.rx_Buffer->data[SCOM3_ByteNum++]=USART_ReceiveData(SCOMx->SCOM_Hardware->USARTx);
			SCOMx->SCOM_Operating.rx_Buffer->len++;
			
			SCOMx->SCOM_Operating.Frame_ByteNum.FrameByteNum++;
			
			if(SCOM3_ByteNum>=RX_BUFFER_MAX_SIZE)
			{			
				SCOMx->SCOM_Operating.Frame_ByteNum.FrameByteFresh=SET;
				SCOMx->SCOM_Operating.USART_rxEndFlag=SET;
				SCOM_Read_FrameByteNum(SCOMx, &FrameByteNum);
				SCOM3_ByteNum=0;
			}	
			
					
		}
			
			
		if(USART_GetITStatus(SCOMx->SCOM_Hardware->USARTx, USART_IT_IDLE) != RESET) //support dma 
		{
			
			Clear=SCOMx->SCOM_Hardware->USARTx->SR;
			Clear=SCOMx->SCOM_Hardware->USARTx->DR;
			
    	DMA_Cmd(SCOMx->SCOM_Hardware->rx_DMAy_Channelx,DISABLE);
			temp=RX_BUFFER_MAX_SIZE-DMA_GetCurrDataCounter(SCOMx->SCOM_Hardware->rx_DMAy_Channelx); //算出本帧长度
			SCOMx->SCOM_Operating.rx_Buffer->len=temp;
			
			SCOMx->SCOM_Operating.Frame_ByteNum.FrameByteFresh=SET;
			SCOMx->SCOM_Operating.Frame_ByteNum.FrameByteNum=temp;
			SCOM_Read_FrameByteNum(SCOMx, &FrameByteNum);
			
			if(SCOMx->SCOM_Operating.rx_Buffer->data[0]==TITLE1)
			{
				if(SCOMx->SCOM_Operating.rx_Buffer->data[1]==TITLE2)
				{
					OpenMV_DataBuffer.theta_err=(int)(*(SCOMx->SCOM_Operating.rx_Buffer->data+2)<<0)|(int)(*(SCOMx->SCOM_Operating.rx_Buffer->data+3)<<8)|
																			(int)(*(SCOMx->SCOM_Operating.rx_Buffer->data+4)<<16)|(int)(*(SCOMx->SCOM_Operating.rx_Buffer->data+5)<<24);
					OpenMV_DataBuffer.rho_err=(int)(*(SCOMx->SCOM_Operating.rx_Buffer->data+6)<<0)|(int)(*(SCOMx->SCOM_Operating.rx_Buffer->data+7)<<8)|
																		(int)(*(SCOMx->SCOM_Operating.rx_Buffer->data+8)<<16)|(int)(*(SCOMx->SCOM_Operating.rx_Buffer->data+9)<<24);				
					memset(SCOMx->SCOM_Operating.rx_Buffer->data,0,sizeof(SCOMx->SCOM_Operating.rx_Buffer->data));
				}			
			}
					
					DMA_SetCurrDataCounter(SCOMx->SCOM_Hardware->rx_DMAy_Channelx,RX_BUFFER_MAX_SIZE);
					DMA_Cmd(SCOMx->SCOM_Hardware->rx_DMAy_Channelx, ENABLE); 
					SCOMx->SCOM_Operating.USART_rxEndFlag=SET;	

		}	
	}
		
}


ErrorStatus	SCOM_TX_DMA_IRQHandler(SCOM_TypeDef_Struct *SCOMx)
{
	uint32_t	DMAy_IT;
	uint32_t	DMAy_FLAG;
	if(SCOMx==NULL)	return ERROR;
	
	if(SCOMx->SCOM_Hardware->tx_DMAy_Channelx==DMA1_Channel2)
	{
		DMAy_IT=DMA1_IT_TC2;
		DMAy_FLAG=DMA1_FLAG_TC2;
	}
	else if(SCOMx->SCOM_Hardware->tx_DMAy_Channelx==DMA1_Channel4)
	{
		if(DMA_GetFlagStatus(DMA1_FLAG_TC4)==SET)
    {
        
      DMA_ClearFlag(DMA1_FLAG_TC4);
			DMA_Cmd (DMA1_Channel4,DISABLE);
//		  DMA_SetCurrDataCounter(DMA1_Channel4, SCOMx->SCOM_Operating.tx_Buffer->len); 连续发
//      DMA_Cmd(DMA1_Channel4,ENABLE);
		}
	}
	else if(SCOMx->SCOM_Hardware->tx_DMAy_Channelx==DMA1_Channel7)
	{
		DMAy_IT=DMA1_IT_TC7;
		DMAy_FLAG=DMA1_FLAG_TC7;
	}
	else if(SCOMx->SCOM_Hardware->rx_DMAy_Channelx==DMA1_Channel5)
	{
		DMAy_IT=DMA1_IT_TC5;
		DMAy_FLAG=DMA1_FLAG_TC5;
	}
	return SUCCESS;
}

ErrorStatus Optical_Flow_Receive_Prepare(SCOM_TypeDef_Struct *SCOMx,uint8_t byte,uint8_t TITLE,uint8_t HEAD)
{
	static uint8_t _data_cnt=0;
	static uint8_t state;
	if(SCOMx==NULL)return ERROR;
	
	if(state==0&&byte==TITLE)
	{
		state=1;
	}
	else if(state==1&&byte==HEAD)
	{
		state=2;
		_data_cnt=0;
	}
	else if(state==2)
	{
		SCOMx->SCOM_Operating.rx_Buffer->data[++_data_cnt]=byte;
		
		if(_data_cnt>=8)
		{
			state=0;
			Data_Processing(SCOMx,&OpenMV_DataBuffer);
		}
	}
	else state=0;
	return SUCCESS;
	
	
}



ErrorStatus Data_Processing(SCOM_TypeDef_Struct *SCOMx,CURRENT_OPENMVDATA_Struct *data)
{
	int theta_org,rho_org;
	
	if(SCOMx==NULL||data==NULL)return ERROR;
	
	theta_org=(int)(*(SCOMx->SCOM_Operating.rx_Buffer->data+1)<<0)|(int)(*(SCOMx->SCOM_Operating.rx_Buffer->data+2)<<8)|(int)(*(SCOMx->SCOM_Operating.rx_Buffer->data+3)<<16)|(int)(*(SCOMx->SCOM_Operating.rx_Buffer->data+4)<<24);
	rho_org=(int)(*(SCOMx->SCOM_Operating.rx_Buffer->data+5)<<0)|(int)(*(SCOMx->SCOM_Operating.rx_Buffer->data+6)<<8)|(int)(*(SCOMx->SCOM_Operating.rx_Buffer->data+7)<<16)|(int)(*(SCOMx->SCOM_Operating.rx_Buffer->data+8)<<24);
		
	data->theta_err=theta_org;
	data->rho_err=rho_org;
	
	return SUCCESS;
	
}

ErrorStatus Read_OpenmvProcessingData(SCOM_TypeDef_Struct *SCOMx,CURRENT_OPENMVDATA_Struct *pdata)
{
	static int Last_theta_err,Last_rho_err;

	if(SCOMx==NULL||pdata==NULL)return ERROR;
	
	if( OpenMV_DataBuffer.rho_err<90&&OpenMV_DataBuffer.rho_err>-90)
	{
		Last_rho_err=OpenMV_DataBuffer.rho_err;
		pdata->rho_err = OpenMV_DataBuffer.rho_err;
	}
	else pdata->rho_err=Last_rho_err;
	
  if( OpenMV_DataBuffer.theta_err<90&&OpenMV_DataBuffer.theta_err>-90)
	{
		Last_theta_err=OpenMV_DataBuffer.theta_err;
		pdata->theta_err = OpenMV_DataBuffer.theta_err;
	}
	else pdata->theta_err=Last_theta_err;
	
	return SUCCESS;
}

static __INLINE void	__SCOM_Disable_IRQ(void)
{
	__SCOM_CPU_PRIMASK_REG_SAVE_VALUE	=	__get_PRIMASK();
	
	__disable_irq();
}

static __INLINE void	__SCOM_Restore_IRQ(void)
{
	__set_PRIMASK(__SCOM_CPU_PRIMASK_REG_SAVE_VALUE);
}

		
		
	

	
	
	


