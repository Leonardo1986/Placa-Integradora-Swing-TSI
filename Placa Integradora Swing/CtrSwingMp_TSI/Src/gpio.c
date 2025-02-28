/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
#include "../Drv/sw_config.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

void GPIO_Init_RS485(tGpio_Conf gpioConf )
{
	GPIO_InitTypeDef GPIO_Init = {0};

	if( gpioConf == _e_Gpio_Init ){
		_pin_set_l(						GPIO_COM_RS485_RTS_Port,
													GPIO_COM_RS485_RTS_Pin);
		GPIO_Init.Pin 			= GPIO_COM_RS485_RTS_Pin;
		GPIO_Init.Mode 			= GPIO_COM_RS485_RTS_Mode;
		GPIO_Init.Pull 			= GPIO_COM_RS485_RTS_Pull;
		GPIO_Init.Speed 		= GPIO_COM_RS485_RTS_Speed;
		HAL_GPIO_Init(				GPIO_COM_RS485_RTS_Port, &GPIO_Init);

		GPIO_Init.Pin 			= GPIO_COM_RS485_TX_Pin;
		GPIO_Init.Mode 			= GPIO_COM_RS485_TX_Mode;
		GPIO_Init.Pull 			= GPIO_COM_RS485_TX_Pull;
		GPIO_Init.Speed 		= GPIO_COM_RS485_TX_Speed;
		GPIO_Init.Alternate = GPIO_COM_RS485_TX_Alt;
		HAL_GPIO_Init(				GPIO_COM_RS485_TX_Port, &GPIO_Init);

		GPIO_Init.Pin 			= GPIO_COM_RS485_RX_Pin;
		GPIO_Init.Mode 			= GPIO_COM_RS485_RX_Mode;
		GPIO_Init.Pull 			= GPIO_COM_RS485_RX_Pull;
		GPIO_Init.Speed 		= GPIO_COM_RS485_RX_Speed;
		GPIO_Init.Alternate = GPIO_COM_RS485_RX_Alt;
		HAL_GPIO_Init(				GPIO_COM_RS485_RX_Port, &GPIO_Init);
	}
	else if( gpioConf == _e_Gpio_DeInit ){
		HAL_GPIO_DeInit(	GPIO_COM_RS485_RTS_Port, 	GPIO_COM_RS485_RTS_Pin 	);
		HAL_GPIO_DeInit(	GPIO_COM_RS485_TX_Port, 	GPIO_COM_RS485_TX_Pin		);
		HAL_GPIO_DeInit(	GPIO_COM_RS485_RX_Port, 	GPIO_COM_RS485_RX_Pin		);
	}
}

void GPIO_Init_Buzzer(tGpio_Conf gpioConf )
{
#if(SWCFG_BUZZER)
	GPIO_InitTypeDef GPIO_Init = {0};

	if( gpioConf == _e_Gpio_Init )
	{
		_pin_set_l(GPIO_HMI_Buzz_Port, GPIO_HMI_Buzz_Pin);
		GPIO_Init.Pin 	= GPIO_HMI_Buzz_Pin;
		GPIO_Init.Mode 	= GPIO_HMI_Buzz_Mode;
		GPIO_Init.Pull 	= GPIO_HMI_Buzz_Pull;
		GPIO_Init.Speed = GPIO_HMI_Buzz_Speed;
		//GPIO_Init.Alternate = GPIO_HMI_Buzz_Alt;
		HAL_GPIO_Init(GPIO_HMI_Buzz_Port, &GPIO_Init);
	}
	else if( gpioConf == _e_Gpio_DeInit )
	{
		HAL_GPIO_DeInit(GPIO_HMI_Buzz_Port, GPIO_HMI_Buzz_Pin);
	}
	#warning "Buzzer Ligado"
#else
	#warning "Buzzer Desligado"
#endif
}

void GPIO_Init_Disp_Led(tGpio_Conf gpioConf )
{
	GPIO_InitTypeDef GPIO_Init = {0};

	if( gpioConf == _e_Gpio_Init ){
		_pin_set_l(	GPIO_HMI_Disp_Led_Port, GPIO_HMI_Disp_Led_Pin );

		GPIO_Init.Pin 			= GPIO_HMI_Disp_Led_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_Disp_Led_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_Disp_Led_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_Disp_Led_Speed;
		//GPIO_Init.Alternate = GPIO_HMI_Disp_Led_Alt;
		HAL_GPIO_Init( GPIO_HMI_Disp_Led_Port, &GPIO_Init );
	}
	else if( gpioConf == _e_Gpio_DeInit ){
		HAL_GPIO_DeInit(	GPIO_HMI_Disp_Led_Port, 	GPIO_HMI_Disp_Led_Pin 	);
	}
}

void GPIO_Init_PictoInOut_Part1(tGpio_Conf gpioConf )
{
	GPIO_InitTypeDef GPIO_Init = {0};

	if( gpioConf == _e_Gpio_Init ){
		_pin_set_l(						GPIO_IOs_Pic_Out_G_Port,
													GPIO_IOs_Pic_Out_G_Pin);
		GPIO_Init.Pin 			= GPIO_IOs_Pic_Out_G_Pin;
		GPIO_Init.Mode 			= GPIO_IOs_Pic_Out_G_Mode;
		GPIO_Init.Pull 			= GPIO_IOs_Pic_Out_G_Pull;
		GPIO_Init.Speed 		= GPIO_IOs_Pic_Out_G_Speed;
		GPIO_Init.Alternate = GPIO_IOs_Pic_Out_G_Alt;
		HAL_GPIO_Init(				GPIO_IOs_Pic_Out_G_Port, &GPIO_Init);

		_pin_set_l(						GPIO_IOs_Pic_Out_B_Port,
													GPIO_IOs_Pic_Out_B_Pin);
		GPIO_Init.Pin 			= GPIO_IOs_Pic_Out_B_Pin;
		GPIO_Init.Mode 			= GPIO_IOs_Pic_Out_B_Mode;
		GPIO_Init.Pull 			= GPIO_IOs_Pic_Out_B_Pull;
		GPIO_Init.Speed 		= GPIO_IOs_Pic_Out_B_Speed;
		GPIO_Init.Alternate = GPIO_IOs_Pic_Out_B_Alt;
		HAL_GPIO_Init(				GPIO_IOs_Pic_Out_B_Port, &GPIO_Init);
	}
	else if( gpioConf == _e_Gpio_DeInit ){
		HAL_GPIO_DeInit(	GPIO_IOs_Pic_Out_G_Port, 	GPIO_IOs_Pic_Out_G_Pin 	);
		HAL_GPIO_DeInit(	GPIO_IOs_Pic_Out_B_Port, 	GPIO_IOs_Pic_Out_B_Pin 	);
	}
}

void GPIO_Init_PictoInOut_Part2(tGpio_Conf gpioConf )
{
	GPIO_InitTypeDef GPIO_Init = {0};

	if( gpioConf == _e_Gpio_Init ){
		_pin_set_l(						GPIO_IOs_Pic_Out_R_Port,
													GPIO_IOs_Pic_Out_R_Pin);
		GPIO_Init.Pin 			= GPIO_IOs_Pic_Out_R_Pin;
		GPIO_Init.Mode 			= GPIO_IOs_Pic_Out_R_Mode;
		GPIO_Init.Pull 			= GPIO_IOs_Pic_Out_R_Pull;
		GPIO_Init.Speed 		= GPIO_IOs_Pic_Out_R_Speed;
		GPIO_Init.Alternate = GPIO_IOs_Pic_Out_R_Alt;
		HAL_GPIO_Init(				GPIO_IOs_Pic_Out_R_Port, &GPIO_Init);

		_pin_set_l(						GPIO_IOs_Pic_In_B_Port,
													GPIO_IOs_Pic_In_B_Pin);
		GPIO_Init.Pin 			= GPIO_IOs_Pic_In_B_Pin;
		GPIO_Init.Mode 			= GPIO_IOs_Pic_In_B_Mode;
		GPIO_Init.Pull 			= GPIO_IOs_Pic_In_B_Pull;
		GPIO_Init.Speed 		= GPIO_IOs_Pic_In_B_Speed;
		GPIO_Init.Alternate = GPIO_IOs_Pic_In_B_Alt;
		HAL_GPIO_Init(				GPIO_IOs_Pic_In_B_Port, &GPIO_Init);

		_pin_set_l(						GPIO_IOs_Pic_In_G_Port,
													GPIO_IOs_Pic_In_G_Pin);
		GPIO_Init.Pin 			= GPIO_IOs_Pic_In_G_Pin;
		GPIO_Init.Mode 			= GPIO_IOs_Pic_In_G_Mode;
		GPIO_Init.Pull 			= GPIO_IOs_Pic_In_G_Pull;
		GPIO_Init.Speed 		= GPIO_IOs_Pic_In_G_Speed;
		GPIO_Init.Alternate = GPIO_IOs_Pic_In_G_Alt;
		HAL_GPIO_Init(				GPIO_IOs_Pic_In_G_Port, &GPIO_Init);

		_pin_set_l(						GPIO_IOs_Pic_In_R_Port,
													GPIO_IOs_Pic_In_R_Pin);
		GPIO_Init.Pin 			= GPIO_IOs_Pic_In_R_Pin;
		GPIO_Init.Mode 			= GPIO_IOs_Pic_In_R_Mode;
		GPIO_Init.Pull 			= GPIO_IOs_Pic_In_R_Pull;
		GPIO_Init.Speed 		= GPIO_IOs_Pic_In_R_Speed;
		GPIO_Init.Alternate = GPIO_IOs_Pic_In_R_Alt;
		HAL_GPIO_Init(				GPIO_IOs_Pic_In_R_Port, &GPIO_Init);
	}
	else if( gpioConf == _e_Gpio_DeInit ){
		HAL_GPIO_DeInit(	GPIO_IOs_Pic_Out_R_Port, 	GPIO_IOs_Pic_Out_R_Pin 	);
		HAL_GPIO_DeInit(	GPIO_IOs_Pic_In_B_Port, 	GPIO_IOs_Pic_In_B_Pin 	);
		HAL_GPIO_DeInit(	GPIO_IOs_Pic_In_G_Port, 	GPIO_IOs_Pic_In_G_Pin 	);
		HAL_GPIO_DeInit(	GPIO_IOs_Pic_In_R_Port, 	GPIO_IOs_Pic_In_R_Pin 	);
	}
}

void GPIO_Init_Urn_Sol(tGpio_Conf gpioConf)
{
	GPIO_InitTypeDef GPIO_Init = {0};

	if( gpioConf == _e_Gpio_Init ){
		_pin_set_l(						GPIO_IOs_Urn_Sol_Port,
													GPIO_IOs_Urn_Sol_Pin);
		GPIO_Init.Pin 			= GPIO_IOs_Urn_Sol_Pin;
		GPIO_Init.Mode 			= GPIO_IOs_Urn_Sol_Mode;
		GPIO_Init.Pull 			= GPIO_IOs_Urn_Sol_Pull;
		GPIO_Init.Speed 		= GPIO_IOs_Urn_Sol_Speed;
		GPIO_Init.Alternate = GPIO_IOs_Urn_Sol_Alt;
		HAL_GPIO_Init(				GPIO_IOs_Urn_Sol_Port, &GPIO_Init);
	}
	else if( gpioConf == _e_Gpio_DeInit ){
		HAL_GPIO_DeInit(	GPIO_IOs_Urn_Sol_Port, 	GPIO_IOs_Urn_Sol_Pin 	);
	}
}

void GPIO_Init_Motors(tGpio_Conf gpioConf )
{
	GPIO_InitTypeDef GPIO_Init = {0};

	if( gpioConf == _e_Gpio_Init ){
		_pin_set_l( GPIO_MT1_DIR_Port, GPIO_MT1_DIR_Pin);

		GPIO_Init.Pin 			= GPIO_MT1_DIR_Pin;
		GPIO_Init.Mode 			= GPIO_MT1_DIR_Mode;
		GPIO_Init.Pull 			= GPIO_MT1_DIR_Pull;
		GPIO_Init.Speed 		= GPIO_MT1_DIR_Speed;
		HAL_GPIO_Init( GPIO_MT1_DIR_Port, &GPIO_Init);

		_pin_set_l( GPIO_MT1_STEP_Port, GPIO_MT1_STEP_Pin);

		GPIO_Init.Pin 			= GPIO_MT1_STEP_Pin;
		GPIO_Init.Mode 			= GPIO_MT1_STEP_Mode;
		GPIO_Init.Pull 			= GPIO_MT1_STEP_Pull;
		GPIO_Init.Speed 		= GPIO_MT1_STEP_Speed;
		GPIO_Init.Alternate 	= GPIO_MT1_STEP_Alt;
		HAL_GPIO_Init( GPIO_MT1_STEP_Port, &GPIO_Init);

		_pin_set_l( GPIO_MT2_DIR_Port, GPIO_MT2_DIR_Pin);

		GPIO_Init.Pin 			= GPIO_MT2_DIR_Pin;
		GPIO_Init.Mode 			= GPIO_MT2_DIR_Mode;
		GPIO_Init.Pull 			= GPIO_MT2_DIR_Pull;
		GPIO_Init.Speed 		= GPIO_MT2_DIR_Speed;
		HAL_GPIO_Init( GPIO_MT2_DIR_Port, &GPIO_Init);

		_pin_set_l( GPIO_MT2_STEP_Port, GPIO_MT2_STEP_Pin);

		GPIO_Init.Pin 			= GPIO_MT2_STEP_Pin;
		GPIO_Init.Mode 			= GPIO_MT2_STEP_Mode;
		GPIO_Init.Pull 			= GPIO_MT2_STEP_Pull;
		GPIO_Init.Speed 		= GPIO_MT2_STEP_Speed;
		GPIO_Init.Alternate 	= GPIO_MT2_STEP_Alt;
		HAL_GPIO_Init( GPIO_MT2_STEP_Port, &GPIO_Init);

		_pin_set_l( GPIO_MT_ENN_Port, GPIO_MT_ENN_Pin);

		GPIO_Init.Pin 			= GPIO_MT_ENN_Pin;
		GPIO_Init.Mode 			= GPIO_MT_ENN_Mode;
		GPIO_Init.Pull 			= GPIO_MT_ENN_Pull;
		GPIO_Init.Speed 		= GPIO_MT_ENN_Speed;
		HAL_GPIO_Init( GPIO_MT_ENN_Port, &GPIO_Init);

	}
	else if( gpioConf == _e_Gpio_DeInit ){
		HAL_GPIO_DeInit(	GPIO_MT1_DIR_Port, 	GPIO_MT1_DIR_Pin 	);
		HAL_GPIO_DeInit(	GPIO_MT1_STEP_Port, 	GPIO_MT1_STEP_Pin 	);
		HAL_GPIO_DeInit(	GPIO_MT2_DIR_Port, 	GPIO_MT2_DIR_Pin 	);
		HAL_GPIO_DeInit(	GPIO_MT2_STEP_Port, 	GPIO_MT2_STEP_Pin 	);
	}
}

void GPIO_Init_Urn(tGpio_Conf gpioConf )
{
	GPIO_InitTypeDef GPIO_Init = {0};

	if( gpioConf == _e_Gpio_Init ){
		_pin_set_l(						GPIO_IOs_Urn_Pic_Port,
													GPIO_IOs_Urn_Pic_Pin);
		GPIO_Init.Pin 			= GPIO_IOs_Urn_Pic_Pin;
		GPIO_Init.Mode 			= GPIO_IOs_Urn_Pic_Mode;
		GPIO_Init.Pull 			= GPIO_IOs_Urn_Pic_Pull;
		GPIO_Init.Speed 		= GPIO_IOs_Urn_Pic_Speed;
		HAL_GPIO_Init(				GPIO_IOs_Urn_Pic_Port, &GPIO_Init);

		_pin_set_l(						GPIO_IOs_Urn_Det_Port,
													GPIO_IOs_Urn_Det_Pin);
		GPIO_Init.Pin 			= GPIO_IOs_Urn_Det_Pin;
		GPIO_Init.Mode 			= GPIO_IOs_Urn_Det_Mode;
		GPIO_Init.Pull 			= GPIO_IOs_Urn_Det_Pull;
		HAL_GPIO_Init(				GPIO_IOs_Urn_Det_Port, &GPIO_Init);

		HAL_NVIC_SetPriority(EXTI4_15_IRQn, 2, 1);
		HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
	}
	else if( gpioConf == _e_Gpio_DeInit ){
		HAL_GPIO_DeInit(	GPIO_IOs_Urn_Pic_Port, 	GPIO_IOs_Urn_Pic_Pin 	);
		HAL_GPIO_DeInit(	GPIO_IOs_Urn_Det_Port, 	GPIO_IOs_Urn_Det_Pin 	);
		HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
	}
}

void GPIO_Init_Sensor(tGpio_Conf gpioConf )
{
	GPIO_InitTypeDef GPIO_Init = {0};

	if( gpioConf == _e_Gpio_Init ){
		_pin_set_l(	GPIO_IOs_Sen_In_Port, GPIO_IOs_Sen_In_Pin );
		GPIO_Init.Pin  = GPIO_IOs_Sen_In_Pin;
		GPIO_Init.Mode = GPIO_IOs_Sen_In_Mode;
		GPIO_Init.Pull = GPIO_IOs_Sen_In_Pull;
		HAL_GPIO_Init( GPIO_IOs_Sen_In_Port, &GPIO_Init );

		_pin_set_l( GPIO_IOs_Sen_Out_Port, GPIO_IOs_Sen_Out_Pin );
		GPIO_Init.Pin  = GPIO_IOs_Sen_Out_Pin;
		GPIO_Init.Mode = GPIO_IOs_Sen_Out_Mode;
		GPIO_Init.Pull = GPIO_IOs_Sen_Out_Pull;
		HAL_GPIO_Init( GPIO_IOs_Sen_Out_Port, &GPIO_Init );

		_pin_set_l( GPIO_IOs_Sen_MidUp_Port, GPIO_IOs_Sen_MidUp_Pin );
		GPIO_Init.Pin  = GPIO_IOs_Sen_MidUp_Pin;
		GPIO_Init.Mode = GPIO_IOs_Sen_MidUp_Mode;
		GPIO_Init.Pull = GPIO_IOs_Sen_MidUp_Pull;
		HAL_GPIO_Init( GPIO_IOs_Sen_MidUp_Port, &GPIO_Init );

		_pin_set_l( GPIO_IOs_Sen_MidDown_Port, GPIO_IOs_Sen_MidDown_Pin );
		GPIO_Init.Pin  = GPIO_IOs_Sen_MidDown_Pin;
		GPIO_Init.Mode = GPIO_IOs_Sen_MidDown_Mode;
		GPIO_Init.Pull = GPIO_IOs_Sen_MidDown_Pull;
		HAL_GPIO_Init( GPIO_IOs_Sen_MidDown_Port, &GPIO_Init );
	}
	else if( gpioConf == _e_Gpio_DeInit ){
		HAL_GPIO_DeInit( GPIO_IOs_Sen_In_Port,      GPIO_IOs_Sen_In_Pin      );
		HAL_GPIO_DeInit( GPIO_IOs_Sen_Out_Port,     GPIO_IOs_Sen_Out_Pin     );
		HAL_GPIO_DeInit( GPIO_IOs_Sen_MidUp_Port,   GPIO_IOs_Sen_MidUp_Pin   );
		HAL_GPIO_DeInit( GPIO_IOs_Sen_MidDown_Port, GPIO_IOs_Sen_MidDown_Pin );
	}
}

void GPIO_Init_PicA(tGpio_Conf gpioConf )
{
	GPIO_InitTypeDef GPIO_Init = {0};

	if( gpioConf == _e_Gpio_Init ){
		_pin_set_l(						GPIO_IOs_PicA_X_Port,
													GPIO_IOs_PicA_X_Pin);
		GPIO_Init.Pin 			= GPIO_IOs_PicA_X_Pin;
		GPIO_Init.Mode 			= GPIO_IOs_PicA_X_Mode;
		GPIO_Init.Pull 			= GPIO_IOs_PicA_X_Pull;
		GPIO_Init.Speed 		= GPIO_IOs_PicA_X_Speed;
		HAL_GPIO_Init(				GPIO_IOs_PicA_X_Port, &GPIO_Init);

		_pin_set_l(						GPIO_IOs_PicA_D_Port,
													GPIO_IOs_PicA_D_Pin);
		GPIO_Init.Pin 			= GPIO_IOs_PicA_D_Pin;
		GPIO_Init.Mode 			= GPIO_IOs_PicA_D_Mode;
		GPIO_Init.Pull 			= GPIO_IOs_PicA_D_Pull;
		GPIO_Init.Speed 		= GPIO_IOs_PicA_D_Speed;
		HAL_GPIO_Init(				GPIO_IOs_PicA_D_Port, &GPIO_Init);

		_pin_set_l(						GPIO_IOs_PicA_E_Port,
													GPIO_IOs_PicA_E_Pin);
		GPIO_Init.Pin 			= GPIO_IOs_PicA_E_Pin;
		GPIO_Init.Mode 			= GPIO_IOs_PicA_E_Mode;
		GPIO_Init.Pull 			= GPIO_IOs_PicA_E_Pull;
		GPIO_Init.Speed 		= GPIO_IOs_PicA_E_Speed;
		HAL_GPIO_Init(				GPIO_IOs_PicA_E_Port, &GPIO_Init);
	}
	else if( gpioConf == _e_Gpio_DeInit ){
		HAL_GPIO_DeInit(	GPIO_IOs_PicA_X_Port, 			GPIO_IOs_PicA_X_Pin 			);
		HAL_GPIO_DeInit(	GPIO_IOs_PicA_D_Port, 			GPIO_IOs_PicA_D_Pin 			);
		HAL_GPIO_DeInit(	GPIO_IOs_PicA_E_Port, 			GPIO_IOs_PicA_E_Pin 			);
	}
}

void GPIO_Init_ComIo(tGpio_Conf gpioConf )
{
	GPIO_InitTypeDef GPIO_Init = {0};

	if( gpioConf == _e_Gpio_Init ){
		_pin_set_l(						GPIO_COM_OUT1_Port,
													GPIO_COM_OUT1_Pin);
		GPIO_Init.Pin 			= GPIO_COM_OUT1_Pin;
		GPIO_Init.Mode 			= GPIO_COM_OUT1_Mode;
		GPIO_Init.Pull 			= GPIO_COM_OUT1_Pull;
		GPIO_Init.Speed 		= GPIO_COM_OUT1_Speed;
		HAL_GPIO_Init(				GPIO_COM_OUT1_Port, &GPIO_Init);

		_pin_set_l(						GPIO_COM_OUT2_Port,
													GPIO_COM_OUT2_Pin);
		GPIO_Init.Pin 			= GPIO_COM_OUT2_Pin;
		GPIO_Init.Mode 			= GPIO_COM_OUT2_Mode;
		GPIO_Init.Pull 			= GPIO_COM_OUT2_Pull;
		GPIO_Init.Speed 		= GPIO_COM_OUT2_Speed;
		HAL_GPIO_Init(				GPIO_COM_OUT2_Port, &GPIO_Init);

		_pin_set_l(						GPIO_COM_IN1_Port,
													GPIO_COM_IN1_Pin);
		GPIO_Init.Pin 			= GPIO_COM_IN1_Pin;
		GPIO_Init.Mode 			= GPIO_COM_IN1_Mode;
		GPIO_Init.Pull 			= GPIO_COM_IN1_Pull;
		HAL_GPIO_Init(				GPIO_COM_IN1_Port, &GPIO_Init);

		_pin_set_l(						GPIO_COM_IN2_Port,
													GPIO_COM_IN2_Pin);
		GPIO_Init.Pin 			= GPIO_COM_IN2_Pin;
		GPIO_Init.Mode 			= GPIO_COM_IN2_Mode;
		GPIO_Init.Pull 			= GPIO_COM_IN2_Pull;
		HAL_GPIO_Init(				GPIO_COM_IN2_Port, &GPIO_Init);
	}
	else if( gpioConf == _e_Gpio_DeInit ){
		HAL_GPIO_DeInit(	GPIO_COM_OUT1_Port, GPIO_COM_OUT1_Pin );
		HAL_GPIO_DeInit(	GPIO_COM_OUT2_Port, GPIO_COM_OUT2_Pin );
		HAL_GPIO_DeInit(	GPIO_COM_IN1_Port, 	GPIO_COM_IN1_Pin 	);
		HAL_GPIO_DeInit(	GPIO_COM_IN2_Port, 	GPIO_COM_IN2_Pin 	);
	}
}

#if CONFIG_SWING_GPIO_HMI_BUS

// Configurado os IOs PD2, PB3, PB4, PB5 como Input para Configurar a Swing

void GPIO_Init_Hmi(tGpio_Conf gpioConf )
{
	GPIO_InitTypeDef GPIO_Init = {0};

	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();
	  __HAL_RCC_GPIOC_CLK_ENABLE();
	  __HAL_RCC_GPIOD_CLK_ENABLE();
	  __HAL_RCC_GPIOF_CLK_ENABLE();

	if( gpioConf == _e_Gpio_Init ){
		_pin_set_h(	GPIO_HMI_BUS_D0_Port, GPIO_HMI_BUS_D0_Pin);

		GPIO_Init.Pin 			= GPIO_HMI_BUS_D0_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_BUS_D0_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_BUS_D0_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_BUS_D0_Speed;
		HAL_GPIO_Init( GPIO_HMI_BUS_D0_Port, &GPIO_Init);


		_pin_set_h(	GPIO_HMI_BUS_D1_Port, GPIO_HMI_BUS_D1_Pin);

		GPIO_Init.Pin 			= GPIO_HMI_BUS_D1_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_BUS_D1_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_BUS_D1_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_BUS_D1_Speed;
		HAL_GPIO_Init( GPIO_HMI_BUS_D1_Port, &GPIO_Init);


		_pin_set_h(	GPIO_HMI_BUS_D2_Port, GPIO_HMI_BUS_D2_Pin);

		GPIO_Init.Pin 			= GPIO_HMI_BUS_D2_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_BUS_D2_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_BUS_D2_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_BUS_D2_Speed;
		HAL_GPIO_Init( GPIO_HMI_BUS_D2_Port, &GPIO_Init);


		_pin_set_h( GPIO_HMI_BUS_D3_Port, GPIO_HMI_BUS_D3_Pin);

		GPIO_Init.Pin 			= GPIO_HMI_BUS_D3_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_BUS_D3_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_BUS_D3_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_BUS_D3_Speed;
		HAL_GPIO_Init( GPIO_HMI_BUS_D3_Port, &GPIO_Init);


		_pin_set_h( GPIO_HMI_BUS_D4_Port, GPIO_HMI_BUS_D4_Pin);

		GPIO_Init.Pin 			= GPIO_HMI_BUS_D4_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_BUS_D4_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_BUS_D4_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_BUS_D4_Speed;
		HAL_GPIO_Init( GPIO_HMI_BUS_D4_Port, &GPIO_Init);


		_pin_set_h(	GPIO_HMI_BUS_D5_Port, GPIO_HMI_BUS_D5_Pin);

		GPIO_Init.Pin 			= GPIO_HMI_BUS_D5_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_BUS_D5_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_BUS_D5_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_BUS_D5_Speed;
		HAL_GPIO_Init( GPIO_HMI_BUS_D5_Port, &GPIO_Init);


		_pin_set_h(	GPIO_HMI_BUS_D6_Port, GPIO_HMI_BUS_D6_Pin);

		GPIO_Init.Pin 			= GPIO_HMI_BUS_D6_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_BUS_D6_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_BUS_D6_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_BUS_D6_Speed;
		HAL_GPIO_Init( GPIO_HMI_BUS_D6_Port, &GPIO_Init);


		_pin_set_h( GPIO_HMI_BUS_D7_Port, GPIO_HMI_BUS_D7_Pin);

		GPIO_Init.Pin 			= GPIO_HMI_BUS_D7_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_BUS_D7_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_BUS_D7_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_BUS_D7_Speed;
		HAL_GPIO_Init(	GPIO_HMI_BUS_D7_Port, &GPIO_Init);


		_pin_set_l(	GPIO_HMI_Disp_Rst_Port, GPIO_HMI_Disp_Rst_Pin);

		GPIO_Init.Pin 			= GPIO_HMI_Disp_Rst_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_Disp_Rst_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_Disp_Rst_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_Disp_Rst_Speed;
		HAL_GPIO_Init( GPIO_HMI_Disp_Rst_Port, &GPIO_Init);


		_pin_set_l(	GPIO_HMI_DISP_EN_Port, GPIO_HMI_DISP_EN_Pin);

		GPIO_Init.Pin 			= GPIO_HMI_DISP_EN_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_DISP_EN_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_DISP_EN_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_DISP_EN_Speed;
		HAL_GPIO_Init( GPIO_HMI_DISP_EN_Port, &GPIO_Init);


		_pin_set_l(	GPIO_HMI_Sel_Disp_Key_Port, GPIO_HMI_Sel_Disp_Key_Pin);

		GPIO_Init.Pin 			= GPIO_HMI_Sel_Disp_Key_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_Sel_Disp_Key_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_Sel_Disp_Key_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_Sel_Disp_Key_Speed;
		HAL_GPIO_Init( GPIO_HMI_Sel_Disp_Key_Port, &GPIO_Init);


	}
	else if( gpioConf == _e_Gpio_DeInit ){
		HAL_GPIO_DeInit(	GPIO_HMI_BUS_D0_Port, GPIO_HMI_BUS_D0_Pin );
		HAL_GPIO_DeInit(	GPIO_HMI_BUS_D1_Port, GPIO_HMI_BUS_D1_Pin );
		HAL_GPIO_DeInit(	GPIO_HMI_BUS_D2_Port, GPIO_HMI_BUS_D2_Pin );
		HAL_GPIO_DeInit(	GPIO_HMI_BUS_D3_Port, GPIO_HMI_BUS_D3_Pin );
		HAL_GPIO_DeInit(	GPIO_HMI_BUS_D4_Port, GPIO_HMI_BUS_D4_Pin );
		HAL_GPIO_DeInit(	GPIO_HMI_BUS_D5_Port, GPIO_HMI_BUS_D5_Pin );
		HAL_GPIO_DeInit(	GPIO_HMI_BUS_D6_Port, GPIO_HMI_BUS_D6_Pin );
		HAL_GPIO_DeInit(	GPIO_HMI_BUS_D7_Port, GPIO_HMI_BUS_D7_Pin );

		HAL_GPIO_DeInit(	GPIO_HMI_Disp_Rst_Port, GPIO_HMI_Disp_Rst_Pin );
		HAL_GPIO_DeInit(	GPIO_HMI_DISP_EN_Port, GPIO_HMI_DISP_EN_Pin );
		HAL_GPIO_DeInit(	GPIO_HMI_Sel_Disp_Key_Port, GPIO_HMI_Sel_Disp_Key_Pin );
	}
}


#else  // CONFIG_SWING_GPIO_HMI_BUS

void GPIO_Init_Hmi(tGpio_Conf gpioConf )
{
	GPIO_InitTypeDef GPIO_Init = {0};

	if( gpioConf == _e_Gpio_Init ){
		_pin_set_l(						GPIO_HMI_BUS_D0_Port,
													GPIO_HMI_BUS_D0_Pin);
		GPIO_Init.Pin 			= GPIO_HMI_BUS_D0_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_BUS_D0_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_BUS_D0_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_BUS_D0_Speed;
		HAL_GPIO_Init(				GPIO_HMI_BUS_D0_Port, &GPIO_Init);

		_pin_set_l(						GPIO_HMI_BUS_D1_Port,
													GPIO_HMI_BUS_D1_Pin);
		GPIO_Init.Pin 			= GPIO_HMI_BUS_D1_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_BUS_D1_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_BUS_D1_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_BUS_D1_Speed;
		HAL_GPIO_Init(				GPIO_HMI_BUS_D1_Port, &GPIO_Init);

		_pin_set_l(						GPIO_HMI_BUS_D2_Port,
													GPIO_HMI_BUS_D2_Pin);
		GPIO_Init.Pin 			= GPIO_HMI_BUS_D2_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_BUS_D2_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_BUS_D2_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_BUS_D2_Speed;
		HAL_GPIO_Init(				GPIO_HMI_BUS_D2_Port, &GPIO_Init);

		_pin_set_l(						GPIO_HMI_BUS_D3_Port,
													GPIO_HMI_BUS_D3_Pin);
		GPIO_Init.Pin 			= GPIO_HMI_BUS_D3_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_BUS_D3_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_BUS_D3_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_BUS_D3_Speed;
		HAL_GPIO_Init(				GPIO_HMI_BUS_D3_Port, &GPIO_Init);

		_pin_set_l(						GPIO_HMI_BUS_D4_Port,
													GPIO_HMI_BUS_D4_Pin);
		GPIO_Init.Pin 			= GPIO_HMI_BUS_D4_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_BUS_D4_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_BUS_D4_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_BUS_D4_Speed;
		HAL_GPIO_Init(				GPIO_HMI_BUS_D4_Port, &GPIO_Init);

		_pin_set_l(						GPIO_HMI_BUS_D5_Port,
													GPIO_HMI_BUS_D5_Pin);
		GPIO_Init.Pin 			= GPIO_HMI_BUS_D5_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_BUS_D5_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_BUS_D5_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_BUS_D5_Speed;
		HAL_GPIO_Init(				GPIO_HMI_BUS_D5_Port, &GPIO_Init);

		_pin_set_l(						GPIO_HMI_BUS_D6_Port,
													GPIO_HMI_BUS_D6_Pin);
		GPIO_Init.Pin 			= GPIO_HMI_BUS_D6_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_BUS_D6_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_BUS_D6_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_BUS_D6_Speed;
		HAL_GPIO_Init(				GPIO_HMI_BUS_D6_Port, &GPIO_Init);

		_pin_set_l(						GPIO_HMI_BUS_D7_Port,
													GPIO_HMI_BUS_D7_Pin);
		GPIO_Init.Pin 			= GPIO_HMI_BUS_D7_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_BUS_D7_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_BUS_D7_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_BUS_D7_Speed;
		HAL_GPIO_Init(				GPIO_HMI_BUS_D7_Port, &GPIO_Init);

		_pin_set_l(						GPIO_HMI_Disp_Rst_Port,
													GPIO_HMI_Disp_Rst_Pin);
		GPIO_Init.Pin 			= GPIO_HMI_Disp_Rst_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_Disp_Rst_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_Disp_Rst_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_Disp_Rst_Speed;
		HAL_GPIO_Init(				GPIO_HMI_Disp_Rst_Port, &GPIO_Init);

		_pin_set_l(						GPIO_HMI_DISP_EN_Port,
													GPIO_HMI_DISP_EN_Pin);
		GPIO_Init.Pin 			= GPIO_HMI_DISP_EN_Pin;
		GPIO_Init.Mode 			= GPIO_HMI_DISP_EN_Mode;
		GPIO_Init.Pull 			= GPIO_HMI_DISP_EN_Pull;
		GPIO_Init.Speed 		= GPIO_HMI_DISP_EN_Speed;
		HAL_GPIO_Init(				GPIO_HMI_DISP_EN_Port, &GPIO_Init);


	}
	else if( gpioConf == _e_Gpio_DeInit ){
		HAL_GPIO_DeInit(	GPIO_HMI_BUS_D0_Port, GPIO_HMI_BUS_D0_Pin );
		HAL_GPIO_DeInit(	GPIO_HMI_BUS_D1_Port, GPIO_HMI_BUS_D1_Pin );
		HAL_GPIO_DeInit(	GPIO_HMI_BUS_D2_Port, GPIO_HMI_BUS_D2_Pin );
		HAL_GPIO_DeInit(	GPIO_HMI_BUS_D3_Port, GPIO_HMI_BUS_D3_Pin );
		HAL_GPIO_DeInit(	GPIO_HMI_BUS_D4_Port, GPIO_HMI_BUS_D4_Pin );
		HAL_GPIO_DeInit(	GPIO_HMI_BUS_D5_Port, GPIO_HMI_BUS_D5_Pin );
		HAL_GPIO_DeInit(	GPIO_HMI_BUS_D6_Port, GPIO_HMI_BUS_D6_Pin );
		HAL_GPIO_DeInit(	GPIO_HMI_BUS_D7_Port, GPIO_HMI_BUS_D7_Pin );
	}
}

#endif  // CONFIG_SWING_GPIO_HMI_BUS

void GPIO_Init_Motor_Locks(tGpio_Conf gpioConf )
{
	GPIO_InitTypeDef GPIO_Init = {0};

	if( gpioConf == _e_Gpio_Init ){
		GPIO_Init.Pin 			= GPIO_MT1_Enc2_Pin;
		GPIO_Init.Mode 			= GPIO_MT1_Enc2_Mode;
		GPIO_Init.Pull 			= GPIO_MT1_Enc2_Pull;
		HAL_GPIO_Init(				GPIO_MT1_Enc2_Port, &GPIO_Init);

		GPIO_Init.Pin 			= GPIO_MT2_Enc2_Pin;
		GPIO_Init.Mode 			= GPIO_MT2_Enc2_Mode;
		GPIO_Init.Pull 			= GPIO_MT2_Enc2_Pull;
		HAL_GPIO_Init(				GPIO_MT2_Enc2_Port, &GPIO_Init);

		GPIO_Init.Pin 			= GPIO_MT1_Enc1_Pin;
		GPIO_Init.Mode 			= GPIO_MT1_Enc1_Mode;
		GPIO_Init.Pull 			= GPIO_MT1_Enc1_Pull;
		HAL_GPIO_Init(				GPIO_MT1_Enc1_Port, &GPIO_Init);

		GPIO_Init.Pin 			= GPIO_MT2_Enc1_Pin;
		GPIO_Init.Mode 			= GPIO_MT2_Enc1_Mode;
		GPIO_Init.Pull 			= GPIO_MT2_Enc1_Pull;
		HAL_GPIO_Init(				GPIO_MT2_Enc1_Port, &GPIO_Init);

		GPIO_Init.Pin 			= GPIO_MT1_Disc1_Pin;
		GPIO_Init.Mode 			= GPIO_MT1_Disc1_Mode;
		GPIO_Init.Pull 			= GPIO_MT1_Disc1_Pull;
		HAL_GPIO_Init(				GPIO_MT1_Disc1_Port, &GPIO_Init);

		GPIO_Init.Pin 			= GPIO_MT2_Disc1_Pin;
		GPIO_Init.Mode 			= GPIO_MT2_Disc1_Mode;
		GPIO_Init.Pull 			= GPIO_MT2_Disc1_Pull;
		HAL_GPIO_Init(				GPIO_MT2_Disc1_Port, &GPIO_Init);

		GPIO_Init.Pin 			= GPIO_MT1_Disc2_Pin;
		GPIO_Init.Mode 			= GPIO_MT1_Disc2_Mode;
		GPIO_Init.Pull 			= GPIO_MT1_Disc2_Pull;
		HAL_GPIO_Init(				GPIO_MT1_Disc2_Port, &GPIO_Init);

		GPIO_Init.Pin 			= GPIO_MT2_Disc2_Pin;
		GPIO_Init.Mode 			= GPIO_MT2_Disc2_Mode;
		GPIO_Init.Pull 			= GPIO_MT2_Disc2_Pull;
		HAL_GPIO_Init(				GPIO_MT2_Disc2_Port, &GPIO_Init);

		_pin_set_l(						GPIO_MT1_STEP_Port,
													GPIO_MT1_STEP_Pin);
		GPIO_Init.Pin 			= GPIO_MT1_STEP_Pin;
		GPIO_Init.Mode 			= GPIO_MT1_STEP_Mode;
		GPIO_Init.Pull 			= GPIO_MT1_STEP_Pull;
		GPIO_Init.Speed 		= GPIO_MT1_STEP_Speed;
		GPIO_Init.Alternate 	= GPIO_MT1_STEP_Alt;
		HAL_GPIO_Init(				GPIO_MT1_STEP_Port, &GPIO_Init);

		_pin_set_l(						GPIO_MT1_Lock1_Port,
													GPIO_MT1_Lock1_Pin);
		GPIO_Init.Pin 			= GPIO_MT1_Lock1_Pin;
		GPIO_Init.Mode 			= GPIO_MT1_Lock1_Mode;
		GPIO_Init.Pull 			= GPIO_MT1_Lock1_Pull;
		GPIO_Init.Speed 		= GPIO_MT1_Lock1_Speed;
		GPIO_Init.Alternate 	= GPIO_MT1_Lock1_Atl;
		HAL_GPIO_Init(				GPIO_MT1_Lock1_Port, &GPIO_Init);

		_pin_set_l(						GPIO_MT2_Lock1_Port,
													GPIO_MT2_Lock1_Pin);
		GPIO_Init.Pin 			= GPIO_MT2_Lock1_Pin;
		GPIO_Init.Mode 			= GPIO_MT2_Lock1_Mode;
		GPIO_Init.Pull 			= GPIO_MT2_Lock1_Pull;
		GPIO_Init.Speed 		= GPIO_MT2_Lock1_Speed;
		GPIO_Init.Alternate 	= GPIO_MT2_Lock1_Alt;
		HAL_GPIO_Init(				GPIO_MT2_Lock1_Port, &GPIO_Init);

#if PLACA_MOTOR_PASSO == 0

		_pin_set_l(						GPIO_MT1_Lock2_Port,
													GPIO_MT1_Lock2_Pin);
		GPIO_Init.Pin 			= GPIO_MT1_Lock2_Pin;
		GPIO_Init.Mode 			= GPIO_MT1_Lock2_Mode;
		GPIO_Init.Pull 			= GPIO_MT1_Lock2_Pull;
		GPIO_Init.Speed 		= GPIO_MT1_Lock2_Speed;
		HAL_GPIO_Init(				GPIO_MT1_Lock2_Port, &GPIO_Init);

		_pin_set_l(						GPIO_MT2_Lock2_Port,
													GPIO_MT2_Lock2_Pin);
		GPIO_Init.Pin 			= GPIO_MT2_Lock2_Pin;
		GPIO_Init.Mode 			= GPIO_MT2_Lock2_Mode;
		GPIO_Init.Pull 			= GPIO_MT2_Lock2_Pull;
		GPIO_Init.Speed 		= GPIO_MT2_Lock2_Speed;
		HAL_GPIO_Init(				GPIO_MT2_Lock2_Port, &GPIO_Init);
#endif

	}
	else if( gpioConf == _e_Gpio_DeInit ){
		HAL_GPIO_DeInit(	GPIO_MT1_DIR_Port, 	GPIO_MT1_DIR_Pin 	);
		HAL_GPIO_DeInit(	GPIO_MT1_STEP_Port, 	GPIO_MT1_STEP_Pin 	);
		HAL_GPIO_DeInit(	GPIO_MT2_DIR_Port, 	GPIO_MT2_DIR_Pin 	);
		HAL_GPIO_DeInit(	GPIO_MT2_STEP_Port, 	GPIO_MT2_STEP_Pin 	);
	}
}

void GPIO_Init_Motor1_Lock1(tGpio_Conf gpioConf )
{
	GPIO_InitTypeDef GPIO_Init = {0};

	if( gpioConf == _e_Gpio_Init ){

		_pin_set_l(				GPIO_MT1_Lock1_Port,
								GPIO_MT1_Lock1_Pin);
		GPIO_Init.Pin 			= GPIO_MT1_Lock1_Pin;
		GPIO_Init.Mode 			= GPIO_MT1_Lock1_Mode;
		GPIO_Init.Pull 			= GPIO_MT1_Lock1_Pull;
		GPIO_Init.Speed 		= GPIO_MT1_Lock1_Speed;
		GPIO_Init.Alternate 	= GPIO_MT1_Lock1_Atl;
		HAL_GPIO_Init(			GPIO_MT1_Lock1_Port, &GPIO_Init);

	}
	else if( gpioConf == _e_Gpio_DeInit ){
		HAL_GPIO_DeInit(	GPIO_MT1_Lock1_Port, 	GPIO_MT1_Lock1_Pin 	);
	}
}

void GPIO_Init_Motor2_Lock1(tGpio_Conf gpioConf )
{
	GPIO_InitTypeDef GPIO_Init = {0};

	if( gpioConf == _e_Gpio_Init ){

		_pin_set_l(				GPIO_MT2_Lock1_Port,
								GPIO_MT2_Lock1_Pin);
		GPIO_Init.Pin 			= GPIO_MT2_Lock1_Pin;
		GPIO_Init.Mode 			= GPIO_MT2_Lock1_Mode;
		GPIO_Init.Pull 			= GPIO_MT2_Lock1_Pull;
		GPIO_Init.Speed 		= GPIO_MT2_Lock1_Speed;
		GPIO_Init.Alternate 	= GPIO_MT2_Lock1_Alt;
		HAL_GPIO_Init(			GPIO_MT2_Lock1_Port, &GPIO_Init);

	}
	else if( gpioConf == _e_Gpio_DeInit ){
		HAL_GPIO_DeInit(	GPIO_MT2_Lock1_Port, 	GPIO_MT2_Lock1_Pin 	);
	}
}

void GPIO_Init_All(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
	//COM
	GPIO_Init_RS485(_e_Gpio_Init);
	GPIO_Init_ComIo(_e_Gpio_Init);
	//IOS
	GPIO_Init_PictoInOut_Part1(_e_Gpio_Init);
	GPIO_Init_PictoInOut_Part2(_e_Gpio_Init);
	GPIO_Init_Urn_Sol(_e_Gpio_Init);
	GPIO_Init_Urn(_e_Gpio_Init);
	GPIO_Init_Sensor(_e_Gpio_Init);
	GPIO_Init_PicA(_e_Gpio_Init);

	//HMI
	GPIO_Init_Hmi(_e_Gpio_Init);

	GPIO_Init_Buzzer(_e_Gpio_Init);
	GPIO_Init_Disp_Led(_e_Gpio_Init);
	//Motors
	GPIO_Init_Motors(_e_Gpio_Init);
	GPIO_Init_Motor_Locks( _e_Gpio_Init );
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
