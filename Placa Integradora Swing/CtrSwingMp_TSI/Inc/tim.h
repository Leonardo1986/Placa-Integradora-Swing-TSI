/**
  ******************************************************************************
  * File Name          : tim.h
  * Description        : Timers header
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __tim_H
#define __tim_H
#ifdef __cplusplus
 extern "C" {
#endif

/*** ** * Includes * ** ***/

#include "main.h"
#include "gpio.h"

/*** ** * Defines * ** ***/

#define _tim_SetPeriod_1(x) 		(TIM1->ARR = x)
#define _tim_SetPeriod_3(x) 		(TIM3->ARR = x)
#define _tim_SetPeriod_6(x) 		(TIM6->ARR = x)
#define _tim_SetPeriod_14(x)		(TIM14->ARR = x)
#define _tim_SetPeriod_15(x)		(TIM15->ARR = x)
#define _tim_SetPeriod_16(x)		(TIM16->ARR = x)
#define _tim_SetPeriod_17(x)		(TIM17->ARR = x)

#if PLACA_MOTOR_PASSO

#define OUTPUTS_TIMER_MOTOR_1	TIM15	//Pwm Motors // JAC - Motor 2 Timer 16
#define OUTPUTS_TIMER_MOTOR_2	TIM16
#define OUTPUTS_TIMER_PICTO1	TIM1	//Pwm Picto Out RGB
#define OUTPUTS_TIMER_PICTO2	TIM3	//Pwm Picto In RGB
#define OUTPUTS_TIMER_URN		TIM14	//Pwm Urna Solenoide
#define OUTPUTS_TIMER_MT2_LOCK	TIM3 //3
#define OUTPUTS_TIMER_MT1_LOCK	TIM3 //3
//#define OUTPUTS_TIMER_DISPLED	TIM16	//Pwm Led Display
//#define OUTPUTS_TIMER_BUZZER	TIM17	//Pwm Buzzer

#else

#define OUTPUTS_TIMER_MOTORS	TIM1	//Pwm Motors
#define OUTPUTS_TIMER_PICTO1	TIM3	//Pwm Picto RGB
#define OUTPUTS_TIMER_PICTO2	TIM15	//Pwm Picto RGB
#define OUTPUTS_TIMER_URN		TIM14	//Pwm Urna Solenoide
#define OUTPUTS_TIMER_MT2_LOCK	TIM16
#define OUTPUTS_TIMER_MT1_LOCK	TIM17
//#define OUTPUTS_TIMER_DISPLED	TIM16	//Pwm Led Display
//#define OUTPUTS_TIMER_BUZZER	TIM17	//Pwm Buzzer

 #endif

#define TIM_MOTOR_1		0
#define TIM_MOTOR_2		1

#define TIM_DIR_LEFT    0
#define TIM_DIR_RIGHT   1


#define TIM_MOTOR_PRESCALER	1//
#define TIM_MOTOR_PERIOD	20000
#define TIM_MOTOR_PULSE		(TIM_MOTOR_PERIOD/2)

/*** ** * Funcs * ** ***/

void TIM_1_Init(void);
void TIM_3_Init(void);
void TIM_6_Init(void);
void TIM_14_Init(void);
void TIM_15_Init(void);
void TIM_16_Init(void);
void TIM_17_Init(void);
void TIM_Init_All(void);
void TIM_MOTOR_STOP(uint8_t p_u8Motor);
void TIM_MOTOR_RUN(uint8_t p_u8Motor, uint16_t p_u16Volocidade, uint8_t p_u8Dir);

void TIM_DisableInterruptMotor(uint8_t p_u8Motor);
void TIM_EnableInterruptMotor(uint8_t p_u8Motor);


/****novo*///
void TIM_STPMT_STOP	(uint8_t p_u8Motor	);
void TIM_STPMT_START(uint8_t p_u8Motor , uint8_t 	p_u8Dir );
void TIM_STPMT_SET	(uint8_t p_u8Motor , uint16_t 	p_period);


#ifdef __cplusplus
}
#endif
#endif /*__tim_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
