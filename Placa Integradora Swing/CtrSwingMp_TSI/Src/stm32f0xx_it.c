/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "main.h"
#include "stm32f0xx_it.h"

#include "gpio.h"
#include "timing.h"
//#include "motor.h"
#include "inputs.h"
#include "outputs.h"
#include "stepmotor.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

extern TIM_HandleTypeDef	htim6;
extern TIM_HandleTypeDef	htim15;
extern TIM_HandleTypeDef	htim16;

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */
/******************************************************************************/

/**
  * @brief This function handles TIM6 global interrupt.
  */
/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
#if USART_USE_DMA
  USART_TypeDef *pUSART = USART2;
  uint32_t u32StatusRegister = 0, errorflags;

  u32StatusRegister = pUSART->ISR;

  /* If no error occurs */
  errorflags = (u32StatusRegister & (uint32_t)(USART_ISR_PE | USART_ISR_FE | USART_ISR_ORE | USART_ISR_NE | USART_ISR_RTOF));
  if(errorflags)
  {
	  // Clear error flag
	  pUSART->ICR |= (USART_ICR_PECF | USART_ICR_FECF | USART_ICR_NCF | USART_ICR_ORECF);

	  // Error handler
	  USART_CallbackError();

	  return;

  }
  else
  {
	  /* UART in mode Receiver ---------------------------------------------------*/
	  if (u32StatusRegister & USART_ISR_IDLE)
	  {
		  // Clear interrupt flag
		  pUSART->ICR |= (USART_ICR_IDLECF);

		  // Received packet
		  USART_CallbackReceivePacket();
		  return;
	  }
  }


#else
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
#endif
}

/**
  * @brief This function handles timer 6 interrupt.
  */
void TIM6_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim6);

	Timing_IT();
	//Inputs_IT();
	Outputs_Task_IT();

#if MOTOR_TASK_IT_BY_OUTPUT_CAPTURE == 0
//	Motor_Task_IT();
#endif
}

/**
  * @brief This function handles timer 15 interrupt.
  */
void TIM15_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim15);
//#if MOTOR_TASK_IT_BY_OUTPUT_CAPTURE
//	Motor_Task_IT(0);
//#endif

	StepMotor_IT(_e_StpMt_Pri);
}

/**
  * @brief This function handles timer 16 interrupt.
  */
void TIM16_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim16);

//#if MOTOR_TASK_IT_BY_OUTPUT_CAPTURE
//	Motor_Task_IT(1);
//#endif

	StepMotor_IT(_e_StpMt_Sec);
}

void EXTI0_1_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_IOs_Urn_Det_Pin);
}

void EXTI2_3_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_MT1_Enc1_Pin);
}

void EXTI4_15_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_MT1_Enc2_Pin);
	HAL_GPIO_EXTI_IRQHandler(GPIO_MT2_Enc1_Pin);
	HAL_GPIO_EXTI_IRQHandler(GPIO_MT2_Enc2_Pin);
}


/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void){
  /* USER CODE BEGIN HardFault_IRQn 0 */
	HAL_NVIC_SystemReset();
  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
		HAL_NVIC_SystemReset();
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

// IRQ handler for DMA chanel 4 and 5 global interrupt

void DMA1_Channel4_5_IRQHandler(void)
{
	//USART_CallbackError();
	HAL_DMA_IRQHandler(&hdma_usart2_tx);
	HAL_DMA_IRQHandler(&hdma_usart2_rx);

	//HAL_UART_IRQHandler(&huart2);
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
