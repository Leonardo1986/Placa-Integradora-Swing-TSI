/**
  ******************************************************************************
  * @file    app_timing.h
  * @author  Arthur Murata
  * @version V0.1
  * @date    08/02/2019
  * @brief   app_timing.c header
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Hive</center></h2>
  *
  *
  ******************************************************************************
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMING_H
#define __TIMING_H
  
/* Includes ------------------------------------------------------------------*/

#include "main.h"

/* Exported types ------------------------------------------------------------*/
typedef enum
{
	_e_Timing_Buzz				,
	_e_Timing_Mt1				,
	_e_Timing_Swing_Generic		,
	_e_Timing_Swing_OpenTimeOut	,
	_e_Timing_Motor1_StepBack	,
	_e_Timing_Motor2_StepBack	,

	_e_Timing_Position_CorrectionTime_Mt1,
	_e_Timing_Position_CorrectionTime_Mt2,

	_e_Timing_Swing_UrnTimeOut	,
	_e_Timing_Swing_PictoUrn	,
	_e_Timing_Swing_AlarmDeb	,
	_e_Timing_Swing_Beep		,

	_e_Timing_Swing_CloseDelay,

	_e_Timing_Swing_CloseDelayEntrance,
	_e_Timing_Swing_CloseDelayExit,

	_e_Timing_Encoder_Break		,
	_e_Timing_Encoder_WaitStop,
	_e_Timing_Motor_calibration ,
	_e_Timing_Swing_Picto_RGB   ,

	_e_Timing_PassOk,

	_e_Timing_Burla,

	_e_Timing_Qty				,
}t_Timing;


/* Exported functions ------------------------------------------------------- */ 

//colocar na int do tim
void Timing_IT(void);

//carrega valores
void Timing_Set( t_Timing TimeType , uint16_t Time );

//rtorna valor restante para estourar
uint16_t Timing_Get( t_Timing TimeType );

//retorna "true" quando estoura
bool Timing_Get_Bool( t_Timing TimeType );

#endif /* __TIMING_H */
  
/************************** (C) COPYRIGHT Hive ************** END OF FILE *****/
