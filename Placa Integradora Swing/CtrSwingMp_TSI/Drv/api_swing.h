/**
  ******************************************************************************
  * @file    Api_Swing.h
  * @author  Jose Faustino
  * @version V0.1
  * @date    16/11/2020
  * @brief   Api_Swing Header
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) TSI.
  *
  ******************************************************************************
  */

#ifndef API_SWING_H_
#define API_SWING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
/* Private typedef -----------------------------------------------------------*/
typedef enum {
	_E_APISW_SUCCESS = 0,
	_E_APISW_ERROR      ,

	_E_APISW_ERROR_MAX

}e_ApiSwing_ErrorCodes_t;

/* Exported functions prototypes ---------------------------------------------*/

void Api_Swing_Init(void);
void Api_Swing_Task_Alarme(void);
void Api_Swing_MotorCtrl_Init(void);


void Api_Swing_Rx_CallBack( uint8_t * p_pu8Buffer);


#ifdef __cplusplus
}
#endif

#endif /* API_SWING_H_ */
