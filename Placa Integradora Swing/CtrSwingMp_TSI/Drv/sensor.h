/**
  ******************************************************************************
  * @file    sensor.h
  * @author  Arthur Murata
  * @version V0.1
  * @date    22/11/2020
  * @brief   Sensor Source
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) TSI.
  *
  ******************************************************************************
  */

#ifndef SENSOR_H_
#define SENSOR_H_

/*** ** * Includes * ** ***/

#include "main.h"

/*** ** * Defines * ** ***/


/*** ** * Types * ** ***/

typedef enum{
	_e_SensorSts_None,
	_e_SensorSts_AlarmEntrance,
	_e_SensorSts_AlarmExit,
	_e_SensorSts_Both,
	_E_SensorSts_AlarmMid,
	_e_SensorSts_PassOk,
	_e_SensorSts_PassWait,
}tSensorSts;

typedef enum{
	_e_SensorMode_Idle,
	_e_SensorMode_Alarm,
	_e_SensorMode_Entrance,
	_e_SensorMode_Exit,
}tSensorMode;

/*** ** * Funcs Ext * ** ***/

void Sensor_Init(void);
void Sensor_Set( tSensorMode senMode );
tSensorMode Sensor_GetMode(void);
tSensorSts Sensor_Get(void);
extern uint8_t _sensor_get_sensor(void);

#endif /* SENSOR_H_ */
