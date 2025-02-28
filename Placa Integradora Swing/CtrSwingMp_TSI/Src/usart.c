/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "gpio.h"
#include "../Drv/task_manager.h"
#include "../Drv/protocolo_swing.h"
#include "../Drv/api_swing.h"
#include <string.h>
/* USER CODE BEGIN 0 */

#define USART_485_RTS_TX_ON()	_pin_set_h(GPIO_COM_RS485_RTS_Port,GPIO_COM_RS485_RTS_Pin);
#define USART_485_RTS_TX_OFF()	_pin_set_l(GPIO_COM_RS485_RTS_Port,GPIO_COM_RS485_RTS_Pin);
/* USER CODE END 0 */

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;

uint8_t rxBuffer[PACKET_SIZE];
uint8_t txBuffer[PACKET_SIZE];

_UsartCallBack_t	callBackRx;

HAL_StatusTypeDef halRet;

/*** ** * mecanismo rxTx * ** ***/

void Usart_Set_Rx_CallBack( _UsartCallBack_t callbackToSet )
{
	callBackRx = callbackToSet;
}

HAL_StatusTypeDef Usart_Set_Tx_Buff( uint8_t *uartBuffTx, uint8_t p_u8Size )
{
	USART_485_RTS_TX_ON();
	memcpy(txBuffer,uartBuffTx,PACKET_SIZE);
	halRet = HAL_UART_Transmit_DMA(&huart2, txBuffer, PACKET_SIZE);
	__NOP();
	return halRet;
}

HAL_StatusTypeDef Usart_Set_Rx_Buff( uint8_t *uartBuffTx, uint8_t p_u8Size)
{
	memset(rxBuffer,0,PACKET_SIZE);
	USART_485_RTS_TX_OFF();
	halRet = HAL_UART_Receive_DMA(&huart2, rxBuffer, PACKET_SIZE);
	__NOP();
	return halRet;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART2)
	{
		if( callBackRx != NULL ){
			callBackRx(rxBuffer);
			halRet = HAL_UART_Receive_DMA(&huart2, rxBuffer, PACKET_SIZE);
			__NOP();
		}
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART2)
	{
		USART_485_RTS_TX_OFF();
		halRet = HAL_UART_Receive_DMA(&huart2, txBuffer, PACKET_SIZE);
		__NOP();
	}
}

/*
void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART2)
	{
		__NOP();
	}
}
*/

/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;//UART_HWCONTROL_RTS;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

  USART_485_RTS_TX_OFF();
  if( HAL_UART_Receive_DMA(&huart2, rxBuffer, PACKET_SIZE ) != HAL_OK )
  {
	  Error_Handler();
  }

  USART_485_RTS_TX_OFF();

  Usart_Set_Rx_CallBack(Api_Swing_Rx_CallBack);
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA1     ------> USART2_RTS
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_Init_RS485(_e_Gpio_Init);



    /* USART2 DMA Init */
    /* USART2_RX Init */
    hdma_usart2_rx.Instance = DMA1_Channel5;
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_rx.Init.Mode = DMA_NORMAL;
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart2_rx);

    /* USART2_TX Init */
    hdma_usart2_tx.Instance = DMA1_Channel4;
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart2_tx);


  /* USER CODE BEGIN USART2_MspInit 1 */
    HAL_NVIC_SetPriority(USART2_IRQn, 3, 3);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA1     ------> USART2_RTS
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    //HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    GPIO_Init_RS485(_e_Gpio_DeInit);
    /* USART2 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
