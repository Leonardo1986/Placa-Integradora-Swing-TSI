/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : stm32f0xx_hal_msp.c
  * Description        : This file provides code for the MSP Initialization
  *                      and de-Initialization codes.
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
#include "main.h"
#include "gpio.h"
//#include "motor.h"

/* USER CODE BEGIN Includes */
#define MOTOR_TASK_IT_BY_OUTPUT_CAPTURE 1
/* USER CODE END Includes */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}

/**
* @brief TIM_PWM MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_pwm: TIM_PWM handle pointer
* @retval None
*/
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm)
{
  if(htim_pwm->Instance==TIM1)
  {
    __HAL_RCC_TIM1_CLK_ENABLE();
  }
  else if(htim_pwm->Instance==TIM3)
  {
    __HAL_RCC_TIM3_CLK_ENABLE();
  }
  else if(htim_pwm->Instance==TIM15)
  {
    __HAL_RCC_TIM15_CLK_ENABLE();

#if MOTOR_TASK_IT_BY_OUTPUT_CAPTURE
    HAL_NVIC_SetPriority(TIM15_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM15_IRQn);
#endif
  }
  else if(htim_pwm->Instance==TIM16)
  {
    __HAL_RCC_TIM16_CLK_ENABLE();

#if MOTOR_TASK_IT_BY_OUTPUT_CAPTURE
    HAL_NVIC_SetPriority(TIM16_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(TIM16_IRQn);
#endif
  }
}

/**
* @brief TIM_Base MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM6)
  {
    __HAL_RCC_TIM6_CLK_ENABLE();
  }
	else if(htim_base->Instance==TIM14)
  {
    __HAL_RCC_TIM14_CLK_ENABLE();
  }
  else if(htim_base->Instance==TIM16)
  {
    __HAL_RCC_TIM16_CLK_ENABLE();
  }
  else if(htim_base->Instance==TIM17)
  {
    __HAL_RCC_TIM17_CLK_ENABLE();
  }
}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(htim->Instance==TIM1)
  {
    __HAL_RCC_GPIOA_CLK_ENABLE();
//    GPIO_Init_Motors(_e_Gpio_Init);
  }
  else if(htim->Instance==TIM3)
  {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_Init_PictoInOut_Part2(_e_Gpio_Init);
  }
  else if(htim->Instance==TIM14)
  {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_Init_Urn_Sol(_e_Gpio_Init);
  }
  else if(htim->Instance==TIM15)
  {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_Init_Motors(_e_Gpio_Init);
//    GPIO_Init_PictoInOut_Part1(_e_Gpio_Init);
  }
  else if(htim->Instance==TIM16)
  {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_Init_Motors(_e_Gpio_Init);
    //GPIO_Init_Disp_Led(_e_Gpio_Init);
//    GPIO_Init_Motor2_Lock1( _e_Gpio_Init );
  }
  else if(htim->Instance==TIM17)
  {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    //GPIO_Init_Buzzer(_e_Gpio_Init);
    GPIO_Init_Motor1_Lock1( _e_Gpio_Init );
  }
}
/**
* @brief TIM_PWM MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param htim_pwm: TIM_PWM handle pointer
* @retval None
*/
void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* htim_pwm)
{
  if(htim_pwm->Instance==TIM1)
  {
    __HAL_RCC_TIM1_CLK_DISABLE();
  }
  else if(htim_pwm->Instance==TIM3)
  {
    __HAL_RCC_TIM3_CLK_DISABLE();
  }
  else if(htim_pwm->Instance==TIM15)
  {
    __HAL_RCC_TIM15_CLK_DISABLE();
  }
  else if(htim_pwm->Instance==TIM16)
  {
    __HAL_RCC_TIM16_CLK_DISABLE();
  }
}

/**
* @brief TIM_Base MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM6)
  {
    __HAL_RCC_TIM6_CLK_DISABLE();
  }
	else if(htim_base->Instance==TIM14)
  {
    __HAL_RCC_TIM14_CLK_DISABLE();
  }
  else if(htim_base->Instance==TIM16)
  {
    __HAL_RCC_TIM16_CLK_DISABLE();
  }
  else if(htim_base->Instance==TIM17)
  {
    __HAL_RCC_TIM17_CLK_DISABLE();
  }
}

/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
//void HAL_UART_MspInit(UART_HandleTypeDef* huart)
//{
////  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  if(huart->Instance==USART2)
//  {
//    __HAL_RCC_USART2_CLK_ENABLE();
//    __HAL_RCC_GPIOA_CLK_ENABLE();
//    GPIO_Init_RS485(_e_Gpio_Init);
//
//    HAL_NVIC_SetPriority(USART2_IRQn, 1, 0);
//	HAL_NVIC_EnableIRQ(USART2_IRQn);
//  }
//}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
//void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
//{
//  if(huart->Instance==USART2)
//  {
//    __HAL_RCC_USART2_CLK_DISABLE();
//    GPIO_Init_RS485(_e_Gpio_DeInit);
//  }
//}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
