/**
  ******************************************************************************
  * @file    stepmotor.h
  * @author  Arthur Murata
  * @version V0.1
  * @date    11/11/2020
  * @brief   Step Motor Header
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) TSI.
  *
  ******************************************************************************
  */

#ifndef STEPMOTOR_H_
#define STEPMOTOR_H_

/*** ** * Includes * ** ***/

/*** ** * Defines * ** ***/




/*** ** * Types * ** ***/

typedef enum{
	_e_StpMt_Pri,
	_e_StpMt_Sec,
	_e_StpMt_Qty,
}tStpMtNum;

typedef enum{
	_e_StpMtSts_Stop,
	_e_StpMtSts_Acce,
	_e_StpMtSts_Plat,
	_e_StpMtSts_Deac,
}tStpMtSts;

typedef enum{
	_e_StMtDir_Left,
	_e_StMtDir_Right,
}tStpMtDir;

typedef enum{
	_e_StpMtPower_OFF,
	_e_StpMtPower_ON,
}tStpMtPower;

/*** ** * Funcs Ext * ** ***/

void StepMotor_Init(void);

void StepMotor_IT( tStpMtNum stpMtNum );

void StepMotor_GO( tStpMtNum stpMtNum , tStpMtDir stpMtDir , float ang , uint16_t speed , uint8_t accel , uint16_t speedStart );
void StepMotor_STOP( tStpMtNum stpMtNum );
void StepMotor_POWER( tStpMtPower power );

tStpMtSts 	StepMotor_GetSts( tStpMtNum stpMtNum );
uint16_t 	StepMotor_GetStepEnc( tStpMtNum stpMtNum );
uint16_t 	StepMotor_GetStepPos( tStpMtNum stpMtNum );
tStpMtDir	StepMotor_GetDir( tStpMtNum stpMtNum );

#endif /* STEPMOTOR_H_ */
