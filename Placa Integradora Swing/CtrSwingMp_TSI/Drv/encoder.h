/**
  ******************************************************************************
  * @file    encoder.h
  * @author  Arthur Murata
  * @version V0.1
  * @date    11/11/2020
  * @brief   Encoder Header
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) TSI.
  *
  ******************************************************************************
  */

#ifndef ENCODER_H_
#define ENCODER_H_

/*** ** * Includes * ** ***/

#include "main.h"

/*** ** * Defines * ** ***/

#define ENC_ALIGN_OPEN	2

/*** ** * Types * ** ***/

typedef enum{
	_e_EncNum_Pri,
	_e_EncNum_Sec,
	_e_EncNum_Qty,
}tEncNum;

typedef enum{
	_e_EncDir_Left,
	_e_EncDir_Right,
	_e_EncDir_Unknown,
}tEncDir;

typedef enum{
	_e_EncCalib_Ok,
	_e_EncCalib_ErrIndex,
	_e_EncCalib_TimeOut1,
	_e_EncCalib_TimeOut2,
	_e_EncCalib_Nok,
	_e_EncCalib_Running,
}tEncCalib;

typedef enum{
	_e_EncSts_Init,
	_e_EncSts_Idle,
	_e_EncSts_Move,
	_e_EncSts_Cali,
}tEncSts;

typedef enum{
	_e_EncCmd_None,
	_e_EncCmd_Cali,
	_e_EncCmd_Move,
	_e_EncCmd_Stop,
	_e_EncCmd_MoveToStop,
}tEncCmd;

typedef enum{
	_e_EncStopMode_Off,
	_e_EncStopMode_On,
}tEncStopMode;

/*** ** * Funcs Ext * ** ***/

//Init

void 		Encoder_Init(void);

//System

void 		Encoder_Refresh_IT( tEncNum encNum , uint8_t encVal );
void 		Encoder_Position_IT( tEncNum encNum , uint8_t posVal );

//User

tEncCalib	Encoder_Cali_Get( tEncNum encNum );
void		Encoder_Cali_Set( tEncNum encNum , tEncStopMode stopMode );

void 		Encoder_Move_Set( tEncNum encNum , uint8_t ang , tEncStopMode stopMode );
float	 	Encoder_Move_Get( tEncNum encNum );

void		Encoder_Move_ToStop( tEncNum encNum , tEncDir encDir );

void		Encoder_Stop( tEncNum encNum , tEncStopMode stopMode );
tEncSts		Encoder_Stat( tEncNum encNum );

void 		_encdisk_func_break_clear(tEncNum encNum);
bool 		_encdisk_func_break_get(tEncNum encNum);

//END

#endif /* ENCODER_H_ */
