/**
  ******************************************************************************
  * @file    inputs.h
  * @author  Arthur Murata
  * @version V0.1
  * @date    26/08/2020
  * @brief   inputs Header
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) TSI.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __inputs_H
#define __inputs_H
#ifdef __cplusplus
 extern "C" {
#endif

/*** ** * Includes * ** ***/
#include "main.h"

/*** ** * Defines * ** ***/

typedef enum
{
	_e_Inputs_SenIn			= 0,
	_e_Inputs_SenOut,
	_e_Inputs_SenMidU,
	_e_Inputs_SenMidD,
	_e_Inputs_ComIn1,
	_e_Inputs_ComIn2,
	_e_Inputs_Mt1Enc2,
	_e_Inputs_Mt1Enc1,
	_e_Inputs_Mt1Disc1,
	_e_Inputs_Mt1Disc2,
	_e_Inputs_Mt2Enc2,
	_e_Inputs_Mt2Enc1,
	_e_Inputs_Mt2Disc1,
	_e_Inputs_Mt2Disc2,
	_e_Inputs_UrnDet,
	_e_Inputs_QTY,
}tInputs;

typedef enum{
	_e_InCom_None			= 0,
	_e_InCom_OpenIn	,
	_e_InCom_OpenOut,
	_e_InCom_UrnPicto,
}tInCom;

typedef enum{
	_e_InMt_Pri		= 0,
	_e_InMt_Sec		= 1,
}tInMt;

typedef enum{
	_e_UrnCardDet_Get	= 0,
	_e_UrnCardDet_Clr	= 1,
}tUrnCardDet;

typedef enum{
	_e_Motor_Qty_Bit1	   = 0,
	_e_Motor_Qty_Bit2,
	_e_Barrier_Type,
	_e_Sensor_Type,
	_e_Config_Qty,
}tSwing_config;

/*** ** * Funcs Ext * ** ***/
void 	Inputs_IT(void);
void 	Inputs_Task(void);
void	Inputs_Init(void);

bool 	Inputs_Get( tInputs in );

bool	Inputs_Get_Config( tSwing_config in );
bool    Input_PinConf_Get( uint8_t pin );

tInCom 	Inputs_Com_Get(void);
bool 	Inputs_Motor_Enc_Get(tInMt motorNum);
bool 	Inputs_Motor_EncChange_Get(tInMt motorNum);

uint8_t	Inputs_Motor_Disc_Get(tInMt motorNum);
	
bool 	Inputs_Urn_CardDet( tUrnCardDet getCear );


#ifdef __cplusplus
}
#endif
#endif /*__inputs_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
