/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for
  *                      the gpio
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Defines -----------------------------------------------------------*/

#define PLACA_MOTOR_PASSO  1

#define CONFIG_SWING_GPIO_HMI_BUS  1


#define GPIO_ENCODER_IT		1
#define GPIO_ENCODER_PULL	0
#define GPIO_ENCODER	    GPIO_ENCODER_PULL

/*** ** * Alternate Functions Pins * ** ***/

//RS485 - USART2

#define GPIO_COM_RS485_RTS_Pin 						GPIO_PIN_1
#define GPIO_COM_RS485_RTS_Port 					GPIOA
#define GPIO_COM_RS485_RTS_Mode						GPIO_MODE_OUTPUT_PP
#define GPIO_COM_RS485_RTS_Pull						GPIO_NOPULL
#define GPIO_COM_RS485_RTS_Speed					GPIO_SPEED_FREQ_LOW

#define GPIO_COM_RS485_TX_Pin 						GPIO_PIN_2
#define GPIO_COM_RS485_TX_Port 						GPIOA
#define GPIO_COM_RS485_TX_Mode						GPIO_MODE_AF_PP
#define GPIO_COM_RS485_TX_Pull						GPIO_PULLUP
#define GPIO_COM_RS485_TX_Speed						GPIO_SPEED_FREQ_HIGH
#define GPIO_COM_RS485_TX_Alt					  	GPIO_AF1_USART2

#define GPIO_COM_RS485_RX_Pin 						GPIO_PIN_3
#define GPIO_COM_RS485_RX_Port 						GPIOA
#define GPIO_COM_RS485_RX_Mode						GPIO_MODE_AF_PP
#define GPIO_COM_RS485_RX_Pull						GPIO_PULLUP
#define GPIO_COM_RS485_RX_Speed						GPIO_SPEED_FREQ_HIGH
#define GPIO_COM_RS485_RX_Alt					  	GPIO_AF1_USART2

//Buzzer - TIM17-CH1
#if PLACA_MOTOR_PASSO
  #define GPIO_HMI_Buzz_Pin 						GPIO_PIN_5
  #define GPIO_HMI_Buzz_Port 						GPIOF
  #define GPIO_HMI_Buzz_Mode						GPIO_MODE_OUTPUT_PP
  #define GPIO_HMI_Buzz_Pull						GPIO_NOPULL
  #define GPIO_HMI_Buzz_Speed						GPIO_SPEED_FREQ_HIGH
#else
  #define GPIO_HMI_Buzz_Pin 						GPIO_PIN_4
  #define GPIO_HMI_Buzz_Port 						GPIOC
  #define GPIO_HMI_Buzz_Mode						GPIO_MODE_OUTPUT_PP
  #define GPIO_HMI_Buzz_Pull						GPIO_NOPULL
  #define GPIO_HMI_Buzz_Speed						GPIO_SPEED_FREQ_HIGH
#endif

//Disp LED - TIM16-CH1
#if PLACA_MOTOR_PASSO
  #define GPIO_HMI_Disp_Led_Pin 					GPIO_PIN_4
  #define GPIO_HMI_Disp_Led_Port 					GPIOF
  #define GPIO_HMI_Disp_Led_Mode					GPIO_MODE_OUTPUT_PP
  #define GPIO_HMI_Disp_Led_Pull					GPIO_NOPULL
  #define GPIO_HMI_Disp_Led_Speed					GPIO_SPEED_FREQ_HIGH
#else
  #define GPIO_HMI_Disp_Led_Pin 					GPIO_PIN_2
  #define GPIO_HMI_Disp_Led_Port 					GPIOB
  #define GPIO_HMI_Disp_Led_Mode					GPIO_MODE_OUTPUT_PP
  #define GPIO_HMI_Disp_Led_Pull					GPIO_NOPULL
  #define GPIO_HMI_Disp_Led_Speed					GPIO_SPEED_FREQ_HIGH
#endif

//Picto Out - TIM15 && TIM3

	//outG = TIM15-CH1
#if PLACA_MOTOR_PASSO
  #define GPIO_IOs_Pic_Out_G_Pin 					GPIO_PIN_9
  #define GPIO_IOs_Pic_Out_G_Port 					GPIOC
  #define GPIO_IOs_Pic_Out_G_Mode					GPIO_MODE_AF_PP
  #define GPIO_IOs_Pic_Out_G_Pull					GPIO_NOPULL
  #define GPIO_IOs_Pic_Out_G_Speed					GPIO_SPEED_FREQ_HIGH
  #define GPIO_IOs_Pic_Out_G_Alt					GPIO_AF1_TIM3
#else
  #define GPIO_IOs_Pic_Out_G_Pin 					GPIO_PIN_14
  #define GPIO_IOs_Pic_Out_G_Port 					GPIOB
  #define GPIO_IOs_Pic_Out_G_Mode					GPIO_MODE_AF_PP
  #define GPIO_IOs_Pic_Out_G_Pull					GPIO_NOPULL
  #define GPIO_IOs_Pic_Out_G_Speed					GPIO_SPEED_FREQ_HIGH
  #define GPIO_IOs_Pic_Out_G_Alt					GPIO_AF1_TIM15
#endif
  //outB = TIM15-CH2
#if PLACA_MOTOR_PASSO
  #define GPIO_IOs_Pic_Out_B_Pin 					GPIO_PIN_8
  #define GPIO_IOs_Pic_Out_B_Port 					GPIOA
  #define GPIO_IOs_Pic_Out_B_Mode					GPIO_MODE_AF_PP
  #define GPIO_IOs_Pic_Out_B_Pull					GPIO_NOPULL
  #define GPIO_IOs_Pic_Out_B_Speed					GPIO_SPEED_FREQ_HIGH
  #define GPIO_IOs_Pic_Out_B_Alt					GPIO_AF2_TIM1
#else
  #define GPIO_IOs_Pic_Out_B_Pin 					GPIO_PIN_15
  #define GPIO_IOs_Pic_Out_B_Port 					GPIOB
  #define GPIO_IOs_Pic_Out_B_Mode					GPIO_MODE_AF_PP
  #define GPIO_IOs_Pic_Out_B_Pull					GPIO_NOPULL
  #define GPIO_IOs_Pic_Out_B_Speed					GPIO_SPEED_FREQ_HIGH
  #define GPIO_IOs_Pic_Out_B_Alt					GPIO_AF1_TIM15
#endif
	//outR = TIM3-CH4
#if PLACA_MOTOR_PASSO
  #define GPIO_IOs_Pic_Out_R_Pin 					GPIO_PIN_8
  #define GPIO_IOs_Pic_Out_R_Port 					GPIOC
  #define GPIO_IOs_Pic_Out_R_Mode					GPIO_MODE_AF_PP
  #define GPIO_IOs_Pic_Out_R_Pull					GPIO_NOPULL
  #define GPIO_IOs_Pic_Out_R_Speed					GPIO_SPEED_FREQ_HIGH
  #define GPIO_IOs_Pic_Out_R_Alt					GPIO_AF1_TIM3
#else
  #define GPIO_IOs_Pic_Out_R_Pin 					GPIO_PIN_1
  #define GPIO_IOs_Pic_Out_R_Port 					GPIOB
  #define GPIO_IOs_Pic_Out_R_Mode					GPIO_MODE_AF_PP
  #define GPIO_IOs_Pic_Out_R_Pull					GPIO_NOPULL
  #define GPIO_IOs_Pic_Out_R_Speed					GPIO_SPEED_FREQ_HIGH
  #define GPIO_IOs_Pic_Out_R_Alt					GPIO_AF1_TIM3
#endif
	//inB = TIM3-CH3
#if PLACA_MOTOR_PASSO
  #define GPIO_IOs_Pic_In_B_Pin 					GPIO_PIN_11
  #define GPIO_IOs_Pic_In_B_Port 					GPIOA
  #define GPIO_IOs_Pic_In_B_Mode					GPIO_MODE_AF_PP
  #define GPIO_IOs_Pic_In_B_Pull					GPIO_NOPULL
  #define GPIO_IOs_Pic_In_B_Speed					GPIO_SPEED_FREQ_HIGH
  #define GPIO_IOs_Pic_In_B_Alt					  	GPIO_AF2_TIM1
#else
  #define GPIO_IOs_Pic_In_B_Pin 					GPIO_PIN_0
  #define GPIO_IOs_Pic_In_B_Port 					GPIOB
  #define GPIO_IOs_Pic_In_B_Mode					GPIO_MODE_AF_PP
  #define GPIO_IOs_Pic_In_B_Pull					GPIO_NOPULL
  #define GPIO_IOs_Pic_In_B_Speed					GPIO_SPEED_FREQ_HIGH
  #define GPIO_IOs_Pic_In_B_Alt						GPIO_AF1_TIM3
#endif
	//inB = TIM3-CH2
#if PLACA_MOTOR_PASSO
  #define GPIO_IOs_Pic_In_G_Pin 					GPIO_PIN_10
  #define GPIO_IOs_Pic_In_G_Port 					GPIOA
  #define GPIO_IOs_Pic_In_G_Mode					GPIO_MODE_AF_PP
  #define GPIO_IOs_Pic_In_G_Pull					GPIO_NOPULL
  #define GPIO_IOs_Pic_In_G_Speed					GPIO_SPEED_FREQ_HIGH
  #define GPIO_IOs_Pic_In_G_Alt					  	GPIO_AF2_TIM1
#else
  #define GPIO_IOs_Pic_In_G_Pin 					GPIO_PIN_7
  #define GPIO_IOs_Pic_In_G_Port 					GPIOA
  #define GPIO_IOs_Pic_In_G_Mode					GPIO_MODE_AF_PP
  #define GPIO_IOs_Pic_In_G_Pull					GPIO_NOPULL
  #define GPIO_IOs_Pic_In_G_Speed					GPIO_SPEED_FREQ_HIGH
  #define GPIO_IOs_Pic_In_G_Alt						GPIO_AF1_TIM3
#endif
	//inR = TIM3-CH1
#if PLACA_MOTOR_PASSO
  #define GPIO_IOs_Pic_In_R_Pin 					GPIO_PIN_9
  #define GPIO_IOs_Pic_In_R_Port 					GPIOA
  #define GPIO_IOs_Pic_In_R_Mode					GPIO_MODE_AF_PP
  #define GPIO_IOs_Pic_In_R_Pull					GPIO_NOPULL
  #define GPIO_IOs_Pic_In_R_Speed					GPIO_SPEED_FREQ_HIGH
  #define GPIO_IOs_Pic_In_R_Alt					 	GPIO_AF2_TIM1
#else
  #define GPIO_IOs_Pic_In_R_Pin 					GPIO_PIN_6
  #define GPIO_IOs_Pic_In_R_Port 					GPIOA
  #define GPIO_IOs_Pic_In_R_Mode					GPIO_MODE_AF_PP
  #define GPIO_IOs_Pic_In_R_Pull					GPIO_NOPULL
  #define GPIO_IOs_Pic_In_R_Speed					GPIO_SPEED_FREQ_HIGH
  #define GPIO_IOs_Pic_In_R_Alt						GPIO_AF1_TIM3
#endif

//Urn Sol - TIM14-CH1

#if PLACA_MOTOR_PASSO
  #define GPIO_IOs_Urn_Sol_Pin 					  	GPIO_PIN_1
  #define GPIO_IOs_Urn_Sol_Port 					GPIOB
  #define GPIO_IOs_Urn_Sol_Mode					  	GPIO_MODE_AF_PP
  #define GPIO_IOs_Urn_Sol_Pull					  	GPIO_NOPULL
  #define GPIO_IOs_Urn_Sol_Speed					GPIO_SPEED_FREQ_LOW
  #define GPIO_IOs_Urn_Sol_Alt					  	GPIO_AF4_TIM14
#else
  #define GPIO_IOs_Urn_Sol_Pin 						GPIO_PIN_4
  #define GPIO_IOs_Urn_Sol_Port 					GPIOA
  #define GPIO_IOs_Urn_Sol_Mode						GPIO_MODE_AF_PP
  #define GPIO_IOs_Urn_Sol_Pull						GPIO_NOPULL
  #define GPIO_IOs_Urn_Sol_Speed					GPIO_SPEED_FREQ_LOW
  #define GPIO_IOs_Urn_Sol_Alt						GPIO_AF4_TIM14
#endif

//Motors - TIM1-CHall
#if PLACA_MOTOR_PASSO
  #define GPIO_MT1_DIR_Pin 					  	  	GPIO_PIN_15
  #define GPIO_MT1_DIR_Port 						GPIOB
  #define GPIO_MT1_DIR_Mode					  	  	GPIO_MODE_OUTPUT_PP	//	GPIO_MODE_AF_PP
  #define GPIO_MT1_DIR_Pull						    GPIO_NOPULL			//	GPIO_PULLDOWN
  #define GPIO_MT1_DIR_Speed						GPIO_SPEED_FREQ_HIGH

#else
  #define GPIO_MT1_RL1_Pin 								GPIO_PIN_8
  #define GPIO_MT1_RL1_Port 							GPIOA
  #define GPIO_MT1_RL1_Mode								GPIO_MODE_AF_PP
  #define GPIO_MT1_RL1_Pull								GPIO_PULLDOWN
  #define GPIO_MT1_RL1_Speed							GPIO_SPEED_FREQ_LOW
  #define GPIO_MT1_RL1_Alt								GPIO_AF2_TIM1
#endif

#if PLACA_MOTOR_PASSO
  #define GPIO_MT1_STEP_Pin 						  GPIO_PIN_14
  #define GPIO_MT1_STEP_Port 						  GPIOB
  #define GPIO_MT1_STEP_Mode						  GPIO_MODE_AF_PP  	//GPIO_MODE_AF_PP
  #define GPIO_MT1_STEP_Pull						  GPIO_NOPULL				//GPIO_PULLDOWN
  #define GPIO_MT1_STEP_Speed						  GPIO_SPEED_FREQ_HIGH
  #define GPIO_MT1_STEP_Alt						      GPIO_AF1_TIM15

#else
  #define GPIO_MT1_RL2_Pin 								GPIO_PIN_9
  #define GPIO_MT1_RL2_Port 							GPIOA
  #define GPIO_MT1_RL2_Mode								GPIO_MODE_AF_PP
  #define GPIO_MT1_RL2_Pull								GPIO_PULLDOWN
  #define GPIO_MT1_RL2_Speed							GPIO_SPEED_FREQ_LOW
  #define GPIO_MT1_RL2_Alt								GPIO_AF2_TIM1
#endif

#if PLACA_MOTOR_PASSO
  #define GPIO_MT2_DIR_Pin 						    GPIO_PIN_9
  #define GPIO_MT2_DIR_Port 					    GPIOB
  #define GPIO_MT2_DIR_Mode						    GPIO_MODE_OUTPUT_PP		//GPIO_MODE_AF_PP
  #define GPIO_MT2_DIR_Pull						    GPIO_NOPULL				//GPIO_PULLDOWN
  #define GPIO_MT2_DIR_Speed					    GPIO_SPEED_FREQ_HIGH

#else
  #define GPIO_MT2_RL1_Pin 								GPIO_PIN_10
  #define GPIO_MT2_RL1_Port 							GPIOA
  #define GPIO_MT2_RL1_Mode								GPIO_MODE_AF_PP
  #define GPIO_MT2_RL1_Pull								GPIO_PULLDOWN
  #define GPIO_MT2_RL1_Speed							GPIO_SPEED_FREQ_LOW
  #define GPIO_MT2_RL1_Alt								GPIO_AF2_TIM1
#endif

#if PLACA_MOTOR_PASSO
  #define GPIO_MT2_STEP_Pin 						  GPIO_PIN_8
  #define GPIO_MT2_STEP_Port 						  GPIOB
  #define GPIO_MT2_STEP_Mode						  GPIO_MODE_AF_PP		//GPIO_MODE_AF_PP
  #define GPIO_MT2_STEP_Pull						  GPIO_NOPULL				//GPIO_PULLDOWN
  #define GPIO_MT2_STEP_Speed						  GPIO_SPEED_FREQ_HIGH
  #define GPIO_MT2_STEP_Alt						      GPIO_AF2_TIM16

#else
  #define GPIO_MT2_RL2_Pin 								GPIO_PIN_11
  #define GPIO_MT2_RL2_Port 							GPIOA
  #define GPIO_MT2_RL2_Mode								GPIO_MODE_AF_PP
  #define GPIO_MT2_RL2_Pull								GPIO_PULLDOWN
  #define GPIO_MT2_RL2_Speed							GPIO_SPEED_FREQ_LOW
  #define GPIO_MT2_RL2_Alt								GPIO_AF2_TIM1
#endif

/*** ** * Basic Functions Pins * ** ***/

//Urn Pic - Out PP

#define GPIO_IOs_Urn_Pic_Pin 						GPIO_PIN_13
#define GPIO_IOs_Urn_Pic_Port 					GPIOC
#define GPIO_IOs_Urn_Pic_Mode						GPIO_MODE_OUTPUT_PP
#define GPIO_IOs_Urn_Pic_Pull						GPIO_NOPULL
#define GPIO_IOs_Urn_Pic_Speed					GPIO_SPEED_FREQ_LOW

//Urn Det - EXTI9

#if PLACA_MOTOR_PASSO
  #define GPIO_IOs_Urn_Det_Pin 					GPIO_PIN_0
  #define GPIO_IOs_Urn_Det_Port 				GPIOB
  #define GPIO_IOs_Urn_Det_Mode					GPIO_MODE_IT_RISING//GPIO_MODE_INPUT//GPIO_MODE_IT_RISING_FALLING//GPIO_MODE_IT_RISING
  #define GPIO_IOs_Urn_Det_Pull					GPIO_PULLUP
#else
  #define GPIO_IOs_Urn_Det_Pin 						GPIO_PIN_9
  #define GPIO_IOs_Urn_Det_Port 					GPIOC
  #define GPIO_IOs_Urn_Det_Mode						GPIO_MODE_IT_RISING//GPIO_MODE_INPUT//GPIO_MODE_IT_RISING_FALLING//GPIO_MODE_IT_RISING
  #define GPIO_IOs_Urn_Det_Pull						GPIO_PULLUP
#endif

//Sensor - IN PU

#define GPIO_IOs_Sen_In_Pin 					GPIO_PIN_14
#define GPIO_IOs_Sen_In_Port 					GPIOC
#define GPIO_IOs_Sen_In_Mode					GPIO_MODE_INPUT
#define GPIO_IOs_Sen_In_Pull					GPIO_NOPULL

#define GPIO_IOs_Sen_Out_Pin 					GPIO_PIN_15
#define GPIO_IOs_Sen_Out_Port 					GPIOC
#define GPIO_IOs_Sen_Out_Mode					GPIO_MODE_INPUT
#define GPIO_IOs_Sen_Out_Pull					GPIO_NOPULL

#define GPIO_IOs_Sen_MidUp_Pin 					GPIO_PIN_0
#define GPIO_IOs_Sen_MidUp_Port 				GPIOF
#define GPIO_IOs_Sen_MidUp_Mode					GPIO_MODE_INPUT
#define GPIO_IOs_Sen_MidUp_Pull					GPIO_NOPULL

#define GPIO_IOs_Sen_MidDown_Pin 				GPIO_PIN_1
#define GPIO_IOs_Sen_MidDown_Port 			    GPIOF
#define GPIO_IOs_Sen_MidDown_Mode				GPIO_MODE_INPUT
#define GPIO_IOs_Sen_MidDown_Pull				GPIO_NOPULL

//Picto Animado

#if PLACA_MOTOR_PASSO
  #define GPIO_IOs_PicA_X_Pin 					  GPIO_PIN_12
  #define GPIO_IOs_PicA_X_Port 					  GPIOB
  #define GPIO_IOs_PicA_X_Mode					  GPIO_MODE_OUTPUT_PP
  #define GPIO_IOs_PicA_X_Pull					  GPIO_NOPULL
  #define GPIO_IOs_PicA_X_Speed					  GPIO_SPEED_FREQ_HIGH
#else
  #define GPIO_IOs_PicA_X_Pin 						GPIO_PIN_8
  #define GPIO_IOs_PicA_X_Port 						GPIOC
  #define GPIO_IOs_PicA_X_Mode						GPIO_MODE_OUTPUT_PP
  #define GPIO_IOs_PicA_X_Pull						GPIO_NOPULL
  #define GPIO_IOs_PicA_X_Speed						GPIO_SPEED_FREQ_HIGH
#endif

#define GPIO_IOs_PicA_D_Pin 						GPIO_PIN_12
#define GPIO_IOs_PicA_D_Port 						GPIOA
#define GPIO_IOs_PicA_D_Mode						GPIO_MODE_OUTPUT_PP
#define GPIO_IOs_PicA_D_Pull						GPIO_NOPULL
#define GPIO_IOs_PicA_D_Speed						GPIO_SPEED_FREQ_LOW

#define GPIO_IOs_PicA_E_Pin 						GPIO_PIN_6
#define GPIO_IOs_PicA_E_Port 						GPIOF
#define GPIO_IOs_PicA_E_Mode						GPIO_MODE_OUTPUT_PP
#define GPIO_IOs_PicA_E_Pull						GPIO_NOPULL
#define GPIO_IOs_PicA_E_Speed						GPIO_SPEED_FREQ_LOW

//Comunicacao IO

#if PLACA_MOTOR_PASSO
  #define GPIO_COM_OUT1_Pin 						  GPIO_PIN_2
  #define GPIO_COM_OUT1_Port 						  GPIOC
  #define GPIO_COM_OUT1_Mode						  GPIO_MODE_OUTPUT_PP
  #define GPIO_COM_OUT1_Pull						  GPIO_NOPULL
  #define GPIO_COM_OUT1_Speed						  GPIO_SPEED_FREQ_LOW
#else
  #define GPIO_COM_OUT1_Pin 							GPIO_PIN_1
  #define GPIO_COM_OUT1_Port 							GPIOC
  #define GPIO_COM_OUT1_Mode							GPIO_MODE_OUTPUT_PP
  #define GPIO_COM_OUT1_Pull							GPIO_NOPULL
  #define GPIO_COM_OUT1_Speed							GPIO_SPEED_FREQ_LOW
#endif

#if PLACA_MOTOR_PASSO
  #define GPIO_COM_OUT2_Pin 						  GPIO_PIN_3
  #define GPIO_COM_OUT2_Port 						  GPIOC
  #define GPIO_COM_OUT2_Mode						  GPIO_MODE_OUTPUT_PP
  #define GPIO_COM_OUT2_Pull						  GPIO_NOPULL
  #define GPIO_COM_OUT2_Speed						  GPIO_SPEED_FREQ_LOW
#else
  #define GPIO_COM_OUT2_Pin 							GPIO_PIN_2
  #define GPIO_COM_OUT2_Port 							GPIOC
  #define GPIO_COM_OUT2_Mode							GPIO_MODE_OUTPUT_PP
  #define GPIO_COM_OUT2_Pull							GPIO_NOPULL
  #define GPIO_COM_OUT2_Speed							GPIO_SPEED_FREQ_LOW
#endif

#if PLACA_MOTOR_PASSO
  #define GPIO_COM_IN1_Pin 						    GPIO_PIN_1
  #define GPIO_COM_IN1_Port 						  GPIOC
  #define GPIO_COM_IN1_Mode						    GPIO_MODE_INPUT
  #define GPIO_COM_IN1_Pull						    GPIO_NOPULL
#else
  #define GPIO_COM_IN1_Pin 								GPIO_PIN_12
  #define GPIO_COM_IN1_Port 							GPIOB
  #define GPIO_COM_IN1_Mode								GPIO_MODE_INPUT
  #define GPIO_COM_IN1_Pull								GPIO_NOPULL
#endif

#if PLACA_MOTOR_PASSO
  #define GPIO_COM_IN2_Pin 						    GPIO_PIN_0
  #define GPIO_COM_IN2_Port 						  GPIOC
  #define GPIO_COM_IN2_Mode						    GPIO_MODE_INPUT
  #define GPIO_COM_IN2_Pull						    GPIO_NOPULL
#else
  #define GPIO_COM_IN2_Pin 								GPIO_PIN_13
  #define GPIO_COM_IN2_Port 							GPIOB
  #define GPIO_COM_IN2_Mode								GPIO_MODE_INPUT
  #define GPIO_COM_IN2_Pull								GPIO_NOPULL
#endif


//Motor 1 - Inverti a serigrafia entao os pinos de disco e encoder estao trocados
#if PLACA_MOTOR_PASSO
  #define GPIO_MT1_Disc1_Pin 						  GPIO_PIN_10
  #define GPIO_MT1_Disc1_Port 					  GPIOB
  #define GPIO_MT1_Disc1_Mode						  GPIO_MODE_INPUT
  #define GPIO_MT1_Disc1_Pull						  GPIO_PULLUP
#else
  #define GPIO_MT1_Disc1_Pin 							GPIO_PIN_0
  #define GPIO_MT1_Disc1_Port 						GPIOC
  #define GPIO_MT1_Disc1_Mode							GPIO_MODE_INPUT
  #define GPIO_MT1_Disc1_Pull							GPIO_PULLUP
#endif

#if PLACA_MOTOR_PASSO
  #define GPIO_MT1_Disc2_Pin 						  GPIO_PIN_11
  #define GPIO_MT1_Disc2_Port 					  GPIOB
  #define GPIO_MT1_Disc2_Mode						  GPIO_MODE_INPUT
  #define GPIO_MT1_Disc2_Pull						  GPIO_PULLUP
#else
  #define GPIO_MT1_Disc2_Pin 							GPIO_PIN_4
  #define GPIO_MT1_Disc2_Port 						GPIOF
  #define GPIO_MT1_Disc2_Mode							GPIO_MODE_INPUT
  #define GPIO_MT1_Disc2_Pull							GPIO_PULLUP
#endif

// JAC - Avaliar qual usar
#if PLACA_MOTOR_PASSO
  #define GPIO_MT1_Enc2_Pin 						  GPIO_PIN_5
  #define GPIO_MT1_Enc2_Port 						  GPIOC
#if(GPIO_ENCODER == GPIO_ENCODER_IT)
  #define GPIO_MT1_Enc2_Mode						  GPIO_MODE_IT_RISING_FALLING
#else
  #define GPIO_MT1_Enc2_Mode						  GPIO_MODE_INPUT
#endif
 #define GPIO_MT1_Enc2_Pull						  GPIO_PULLUP

#else
  #define GPIO_MT1_Sen_Pin 								GPIO_PIN_5
  #define GPIO_MT1_Sen_Port 							GPIOA
  #define GPIO_MT1_Sen_Mode								GPIO_MODE_INPUT
  #define GPIO_MT1_Sen_Pull								GPIO_PULLUP
#endif

#if PLACA_MOTOR_PASSO
  #define GPIO_MT1_Enc1_Pin 						  GPIO_PIN_2
  #define GPIO_MT1_Enc1_Port 						  GPIOB
#if(GPIO_ENCODER == GPIO_ENCODER_IT)
  #define GPIO_MT1_Enc1_Mode						  GPIO_MODE_IT_RISING_FALLING
#else
  #define GPIO_MT1_Enc1_Mode						  GPIO_MODE_INPUT//GPIO_MODE_IT_RISING_FALLING//GPIO_MODE_INPUT
#endif
 #define GPIO_MT1_Enc1_Pull						  GPIO_PULLUP
#else
  #define GPIO_MT1_Enc1_Pin 							GPIO_PIN_11
  #define GPIO_MT1_Enc1_Port 							GPIOB
  #define GPIO_MT1_Enc1_Mode							GPIO_MODE_INPUT
  #define GPIO_MT1_Enc1_Pull							GPIO_PULLUP
#endif

#if PLACA_MOTOR_PASSO
  #define GPIO_MT1_Lock1_Pin 						  GPIO_PIN_7
  #define GPIO_MT1_Lock1_Port 					  GPIOC
  #define GPIO_MT1_Lock1_Mode						  GPIO_MODE_AF_PP
  #define GPIO_MT1_Lock1_Pull						  GPIO_NOPULL
  #define GPIO_MT1_Lock1_Speed					  GPIO_SPEED_FREQ_HIGH
  #define GPIO_MT1_Lock1_Atl						  GPIO_AF1_TIM3
#else
  #define GPIO_MT1_Lock1_Pin 							GPIO_PIN_9
  #define GPIO_MT1_Lock1_Port 						GPIOB
  #define GPIO_MT1_Lock1_Mode							GPIO_MODE_AF_PP
  #define GPIO_MT1_Lock1_Pull							GPIO_NOPULL
  #define GPIO_MT1_Lock1_Speed						GPIO_SPEED_FREQ_HIGH
  #define GPIO_MT1_Lock1_Atl							GPIO_AF2_TIM17
#endif

// JAC - Placa nova nao utiliza
#if PLACA_MOTOR_PASSO == 0
  #define GPIO_MT1_Lock2_Pin 							GPIO_PIN_5
  #define GPIO_MT1_Lock2_Port 						GPIOC
  #define GPIO_MT1_Lock2_Mode							GPIO_MODE_OUTPUT_PP
  #define GPIO_MT1_Lock2_Pull							GPIO_NOPULL
  #define GPIO_MT1_Lock2_Speed						GPIO_SPEED_FREQ_HIGH
#endif



#if PLACA_MOTOR_PASSO
  #define GPIO_MT2_Disc1_Pin 						  GPIO_PIN_5
  #define GPIO_MT2_Disc1_Port 					  GPIOA
  #define GPIO_MT2_Disc1_Mode						  GPIO_MODE_INPUT
  #define GPIO_MT2_Disc1_Pull						  GPIO_PULLUP
#else
  #define GPIO_MT2_Disc1_Pin 							GPIO_PIN_3
  #define GPIO_MT2_Disc1_Port 						GPIOC
  #define GPIO_MT2_Disc1_Mode							GPIO_MODE_INPUT
  #define GPIO_MT2_Disc1_Pull							GPIO_PULLUP
#endif

#if PLACA_MOTOR_PASSO
  #define GPIO_MT2_Disc2_Pin 						  GPIO_PIN_6
  #define GPIO_MT2_Disc2_Port 					  GPIOA
  #define GPIO_MT2_Disc2_Mode						  GPIO_MODE_INPUT
  #define GPIO_MT2_Disc2_Pull						  GPIO_PULLUP
#else
  #define GPIO_MT2_Disc2_Pin 							GPIO_PIN_5
  #define GPIO_MT2_Disc2_Port 						GPIOF
  #define GPIO_MT2_Disc2_Mode							GPIO_MODE_INPUT
  #define GPIO_MT2_Disc2_Pull							GPIO_PULLUP
#endif

// JAC - Avaliar qual usar
#if PLACA_MOTOR_PASSO
  #define GPIO_MT2_Enc2_Pin 						GPIO_PIN_4
  #define GPIO_MT2_Enc2_Port 						GPIOC
#if(GPIO_ENCODER == GPIO_ENCODER_IT)
  #define GPIO_MT2_Enc2_Mode						GPIO_MODE_IT_RISING_FALLING
#else
  #define GPIO_MT2_Enc2_Mode						GPIO_MODE_INPUT
#endif
 #define GPIO_MT2_Enc2_Pull						GPIO_PULLUP
#else
  #define GPIO_MT2_Sen_Pin 								GPIO_PIN_6
  #define GPIO_MT2_Sen_Port 							GPIOC
  #define GPIO_MT2_Sen_Mode								GPIO_MODE_INPUT
  #define GPIO_MT2_Sen_Pull								GPIO_PULLUP
#endif

#if PLACA_MOTOR_PASSO
  #define GPIO_MT2_Enc1_Pin 						  GPIO_PIN_7
  #define GPIO_MT2_Enc1_Port 						  GPIOA
#if(GPIO_ENCODER == GPIO_ENCODER_IT)
  #define GPIO_MT2_Enc1_Mode						  GPIO_MODE_IT_RISING_FALLING
#else
  #define GPIO_MT2_Enc1_Mode						  GPIO_MODE_INPUT
#endif
 #define GPIO_MT2_Enc1_Pull						  GPIO_PULLUP
#else
  #define GPIO_MT2_Enc1_Pin 							GPIO_PIN_7
  #define GPIO_MT2_Enc1_Port 							GPIOC
  #define GPIO_MT2_Enc1_Mode							GPIO_MODE_INPUT
  #define GPIO_MT2_Enc1_Pull							GPIO_PULLUP
#endif

#if PLACA_MOTOR_PASSO
  #define GPIO_MT2_Lock1_Pin 						  GPIO_PIN_6
  #define GPIO_MT2_Lock1_Port 					  GPIOC
  #define GPIO_MT2_Lock1_Mode						  GPIO_MODE_AF_PP
  #define GPIO_MT2_Lock1_Pull						  GPIO_NOPULL
  #define GPIO_MT2_Lock1_Speed					  GPIO_SPEED_FREQ_LOW
  #define GPIO_MT2_Lock1_Alt						  GPIO_AF1_TIM3
#else
  #define GPIO_MT2_Lock1_Pin 							GPIO_PIN_8
  #define GPIO_MT2_Lock1_Port 						GPIOB
  #define GPIO_MT2_Lock1_Mode							GPIO_MODE_AF_PP
  #define GPIO_MT2_Lock1_Pull							GPIO_NOPULL
  #define GPIO_MT2_Lock1_Speed						GPIO_SPEED_FREQ_LOW
  #define GPIO_MT2_Lock1_Alt							GPIO_AF2_TIM16
#endif

// JAC - Placa nova nao utiliza
#if PLACA_MOTOR_PASSO == 0
  #define GPIO_MT2_Lock2_Pin 							GPIO_PIN_10
  #define GPIO_MT2_Lock2_Port 						GPIOB
  #define GPIO_MT2_Lock2_Mode							GPIO_MODE_OUTPUT_PP
  #define GPIO_MT2_Lock2_Pull							GPIO_NOPULL
  #define GPIO_MT2_Lock2_Speed						GPIO_SPEED_FREQ_HIGH
#endif

#if PLACA_MOTOR_PASSO
  #define GPIO_MT_ENN_Pin 						    GPIO_PIN_13
  #define GPIO_MT_ENN_Port 						    GPIOB
  #define GPIO_MT_ENN_Mode						    GPIO_MODE_OUTPUT_PP
  #define GPIO_MT_ENN_Pull						    GPIO_NOPULL
  #define GPIO_MT_ENN_Speed							GPIO_SPEED_FREQ_HIGH
#endif

//HMI


#if CONFIG_SWING_GPIO_HMI_BUS

#define GPIO_HMI_BUS_D0_Pin 						GPIO_PIN_15
#define GPIO_HMI_BUS_D0_Port 						GPIOA
#define GPIO_HMI_BUS_D0_Mode						GPIO_MODE_OUTPUT_PP
#define GPIO_HMI_BUS_D0_Pull						GPIO_PULLDOWN  //GPIO_NOPULL
#define GPIO_HMI_BUS_D0_Speed						GPIO_SPEED_FREQ_HIGH

#define GPIO_HMI_BUS_D1_Pin 						GPIO_PIN_10
#define GPIO_HMI_BUS_D1_Port 						GPIOC
#define GPIO_HMI_BUS_D1_Mode						GPIO_MODE_OUTPUT_PP
#define GPIO_HMI_BUS_D1_Pull						GPIO_PULLDOWN  //GPIO_NOPULL
#define GPIO_HMI_BUS_D1_Speed						GPIO_SPEED_FREQ_HIGH

#define GPIO_HMI_BUS_D2_Pin 						GPIO_PIN_11
#define GPIO_HMI_BUS_D2_Port 						GPIOC
#define GPIO_HMI_BUS_D2_Mode						GPIO_MODE_OUTPUT_PP
#define GPIO_HMI_BUS_D2_Pull						GPIO_PULLDOWN //GPIO_NOPULL
#define GPIO_HMI_BUS_D2_Speed						GPIO_SPEED_FREQ_HIGH

#define GPIO_HMI_BUS_D3_Pin 						GPIO_PIN_12
#define GPIO_HMI_BUS_D3_Port 						GPIOC
#define GPIO_HMI_BUS_D3_Mode						GPIO_MODE_OUTPUT_PP
#define GPIO_HMI_BUS_D3_Pull						GPIO_PULLDOWN  //GPIO_NOPULL
#define GPIO_HMI_BUS_D3_Speed						GPIO_SPEED_FREQ_HIGH


#define GPIO_HMI_BUS_D4_Pin 						GPIO_PIN_2
#define GPIO_HMI_BUS_D4_Port 						GPIOD
#define GPIO_HMI_BUS_D4_Mode						GPIO_MODE_INPUT
#define GPIO_HMI_BUS_D4_Pull						GPIO_PULLUP  // GPIO_NOPULL
#define GPIO_HMI_BUS_D4_Speed						GPIO_SPEED_FREQ_HIGH

#define GPIO_HMI_BUS_D5_Pin 						GPIO_PIN_3
#define GPIO_HMI_BUS_D5_Port 						GPIOB
#define GPIO_HMI_BUS_D5_Mode						GPIO_MODE_INPUT
#define GPIO_HMI_BUS_D5_Pull						GPIO_PULLUP  // GPIO_NOPULL
#define GPIO_HMI_BUS_D5_Speed						GPIO_SPEED_FREQ_HIGH

#define GPIO_HMI_BUS_D6_Pin 						GPIO_PIN_4
#define GPIO_HMI_BUS_D6_Port 						GPIOB
#define GPIO_HMI_BUS_D6_Mode						GPIO_MODE_INPUT
#define GPIO_HMI_BUS_D6_Pull						GPIO_PULLUP  // GPIO_NOPULL
#define GPIO_HMI_BUS_D6_Speed						GPIO_SPEED_FREQ_HIGH

#define GPIO_HMI_BUS_D7_Pin 						GPIO_PIN_5
#define GPIO_HMI_BUS_D7_Port 						GPIOB
#define GPIO_HMI_BUS_D7_Mode						GPIO_MODE_INPUT
#define GPIO_HMI_BUS_D7_Pull						GPIO_PULLUP  // GPIO_NOPULL
#define GPIO_HMI_BUS_D7_Speed						GPIO_SPEED_FREQ_HIGH


#else

#define GPIO_HMI_BUS_D0_Pin 						GPIO_PIN_15
#define GPIO_HMI_BUS_D0_Port 						GPIOA
#define GPIO_HMI_BUS_D0_Mode						GPIO_MODE_OUTPUT_PP
#define GPIO_HMI_BUS_D0_Pull						GPIO_NOPULL
#define GPIO_HMI_BUS_D0_Speed						GPIO_SPEED_FREQ_HIGH

#define GPIO_HMI_BUS_D1_Pin 						GPIO_PIN_10
#define GPIO_HMI_BUS_D1_Port 						GPIOC
#define GPIO_HMI_BUS_D1_Mode						GPIO_MODE_OUTPUT_PP
#define GPIO_HMI_BUS_D1_Pull						GPIO_NOPULL
#define GPIO_HMI_BUS_D1_Speed						GPIO_SPEED_FREQ_HIGH

#define GPIO_HMI_BUS_D2_Pin 						GPIO_PIN_11
#define GPIO_HMI_BUS_D2_Port 						GPIOC
#define GPIO_HMI_BUS_D2_Mode						GPIO_MODE_OUTPUT_PP
#define GPIO_HMI_BUS_D2_Pull						GPIO_NOPULL
#define GPIO_HMI_BUS_D2_Speed						GPIO_SPEED_FREQ_HIGH

#define GPIO_HMI_BUS_D3_Pin 						GPIO_PIN_12
#define GPIO_HMI_BUS_D3_Port 						GPIOC
#define GPIO_HMI_BUS_D3_Mode						GPIO_MODE_OUTPUT_PP
#define GPIO_HMI_BUS_D3_Pull						GPIO_NOPULL
#define GPIO_HMI_BUS_D3_Speed						GPIO_SPEED_FREQ_HIGH

#define GPIO_HMI_BUS_D4_Pin 						GPIO_PIN_2
#define GPIO_HMI_BUS_D4_Port 						GPIOD
#define GPIO_HMI_BUS_D4_Mode						GPIO_MODE_OUTPUT_PP
#define GPIO_HMI_BUS_D4_Pull						GPIO_NOPULL
#define GPIO_HMI_BUS_D4_Speed						GPIO_SPEED_FREQ_HIGH

#define GPIO_HMI_BUS_D5_Pin 						GPIO_PIN_3
#define GPIO_HMI_BUS_D5_Port 						GPIOB
#define GPIO_HMI_BUS_D5_Mode						GPIO_MODE_OUTPUT_PP
#define GPIO_HMI_BUS_D5_Pull						GPIO_NOPULL
#define GPIO_HMI_BUS_D5_Speed						GPIO_SPEED_FREQ_HIGH

#define GPIO_HMI_BUS_D6_Pin 						GPIO_PIN_4
#define GPIO_HMI_BUS_D6_Port 						GPIOB
#define GPIO_HMI_BUS_D6_Mode						GPIO_MODE_OUTPUT_PP
#define GPIO_HMI_BUS_D6_Pull						GPIO_NOPULL
#define GPIO_HMI_BUS_D6_Speed						GPIO_SPEED_FREQ_HIGH

#define GPIO_HMI_BUS_D7_Pin 						GPIO_PIN_5
#define GPIO_HMI_BUS_D7_Port 						GPIOB
#define GPIO_HMI_BUS_D7_Mode						GPIO_MODE_OUTPUT_PP
#define GPIO_HMI_BUS_D7_Pull						GPIO_NOPULL
#define GPIO_HMI_BUS_D7_Speed						GPIO_SPEED_FREQ_HIGH

#endif

#define GPIO_HMI_Disp_Rst_Pin 					    GPIO_PIN_0
#define GPIO_HMI_Disp_Rst_Port 					    GPIOA
#define GPIO_HMI_Disp_Rst_Mode					    GPIO_MODE_OUTPUT_PP
#define GPIO_HMI_Disp_Rst_Pull					    GPIO_NOPULL
#define GPIO_HMI_Disp_Rst_Speed					    GPIO_SPEED_FREQ_LOW

#define GPIO_HMI_DISP_EN_Pin 						GPIO_PIN_6
#define GPIO_HMI_DISP_EN_Port 					    GPIOB
#define GPIO_HMI_DISP_EN_Mode						GPIO_MODE_OUTPUT_PP
#define GPIO_HMI_DISP_EN_Pull						GPIO_NOPULL
#define GPIO_HMI_DISP_EN_Speed					    GPIO_SPEED_FREQ_HIGH

#define GPIO_HMI_DISP_RS_Pin 						GPIO_PIN_7
#define GPIO_HMI_DISP_RS_Port 					    GPIOB
#define GPIO_HMI_DISP_RS_Mode						GPIO_MODE_OUTPUT_PP
#define GPIO_HMI_DISP_RS_Pull						GPIO_NOPULL
#define GPIO_HMI_DISP_RS_Speed					    GPIO_SPEED_FREQ_HIGH

#define GPIO_HMI_Sel_Disp_Key_Pin 			        GPIO_PIN_7
#define GPIO_HMI_Sel_Disp_Key_Port 			        GPIOF
#define GPIO_HMI_Sel_Disp_Key_Mode			        GPIO_MODE_OUTPUT_PP
#define GPIO_HMI_Sel_Disp_Key_Pull			        GPIO_NOPULL
#define GPIO_HMI_Sel_Disp_Key_Speed			        GPIO_SPEED_FREQ_HIGH

//Macro
#define _pin_tg(port,pin)		  HAL_GPIO_TogglePin(port, pin)

#define _pin_set_l(port,pin)		( port->BSRR 	= (uint32_t)pin << 16U	)//HAL_GPIO_WritePin(port, pin, RESET);//( port->BSRR 	= (uint32_t)pin << 16U	)
#define _pin_set_h(port,pin)		( port->BSRR 	= (uint32_t)pin					)//HAL_GPIO_WritePin(port, pin, SET);//( port->BSRR 	= (uint32_t)pin					)

#define _pin_get(port,pin)			( port->IDR & pin )


#define GPIO_MT1_DIR_LEFT		_pin_set_l(GPIO_MT1_DIR_Port, GPIO_MT1_DIR_Pin)
#define GPIO_MT1_DIR_RIGHT		_pin_set_h(GPIO_MT1_DIR_Port, GPIO_MT1_DIR_Pin)

#define GPIO_MT2_DIR_LEFT		_pin_set_l(GPIO_MT2_DIR_Port, GPIO_MT2_DIR_Pin)
#define GPIO_MT2_DIR_RIGHT		_pin_set_h(GPIO_MT2_DIR_Port, GPIO_MT2_DIR_Pin)


typedef enum{
	_e_Gpio_Init		= 0,
	_e_Gpio_DeInit	= 1,
}tGpio_Conf;


void GPIO_Init_All(void);
//COM
void GPIO_Init_RS485(tGpio_Conf gpioConf );
void GPIO_Init_ComIo(tGpio_Conf gpioConf );
//IOS
void GPIO_Init_PictoInOut_Part1(tGpio_Conf gpioConf );
void GPIO_Init_PictoInOut_Part2(tGpio_Conf gpioConf );
void GPIO_Init_Urn_Sol(tGpio_Conf gpioConf );
void GPIO_Init_Urn(tGpio_Conf gpioConf );
void GPIO_Init_Sensor(tGpio_Conf gpioConf );
void GPIO_Init_PicA(tGpio_Conf gpioConf );
//HMI
void GPIO_Init_Buzzer(tGpio_Conf gpioConf );
void GPIO_Init_Disp_Led(tGpio_Conf gpioConf );
//Motors
void GPIO_Init_Motors(tGpio_Conf gpioConf );
void GPIO_Init_Motor_Locks(tGpio_Conf gpioConf );
void GPIO_Init_Motor1_Lock1(tGpio_Conf gpioConf );
void GPIO_Init_Motor2_Lock1(tGpio_Conf gpioConf );

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
