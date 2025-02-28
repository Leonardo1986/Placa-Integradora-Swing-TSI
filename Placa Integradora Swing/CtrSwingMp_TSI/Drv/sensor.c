/**
  ******************************************************************************
  * @file    sensor.c
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

/*** ** * Includes * ** ***/
#include "sensor.h"
#include "task_manager.h"
#include "inputs.h"
#include "outputs.h"

/*** ** * Defines * ** ***/
#define TMSEN	_e_TaskManager_Sensor

/*** ** * Types * ** ***/
typedef enum{
	_e_SensorBit_Read = 0,
	_e_SensorBit_Mem,
	_e_SensorBit_Qty,
}tSensorBit;

/*** ** * Variables * ** ***/
struct{

	tSensorMode mode;
	tSensorSts	status;

	union{
		uint8_t	senGroup;
		struct{
			uint8_t	in 	: 1;
			uint8_t	mU	: 1;
			uint8_t	mD	: 1;
			uint8_t	out	: 1;
			uint8_t	bla	: 4;
		}senbit;
	}senByte[_e_SensorBit_Qty];

}sSensor;

/*** ** * Local Funcs * ** ***/
uint8_t _sensor_get_sensor(void)
{
	uint8_t sensorReturn = _e_SensorSts_None;

	bool senIn    = (bool)!Inputs_Get( _e_Inputs_SenIn   );
	bool senOut   = (bool)!Inputs_Get( _e_Inputs_SenOut  );
	bool senMidUp = (bool)!Inputs_Get( _e_Inputs_SenMidU );
	bool senMidDw = (bool)!Inputs_Get( _e_Inputs_SenMidD );
	sensorReturn  = (uint8_t)( senIn + (senMidUp << 1) + (senMidDw << 2) + (senOut << 3) );

	return sensorReturn;
}

/*** ** * TASK * ** ***/
typedef enum
{
	_e_sensor_task_idle,
	_e_sensor_task_alarm,
	_e_sensor_task_entrance,
	_e_sensor_task_exit,
}t_sensor_task;

void _sensor_task_idle(void);
void _sensor_task_alarm(void);
void _sensor_task_entrance(void);
void _sensor_task_exit(void);

void _sensor_task_idle(void)
{
	TaskManager_Set( TMSEN , _e_sensor_task_idle );

	if( TaskManager_First_Get( TMSEN ) == false )
	{
		TaskManager_First_Set( TMSEN );
		TaskManager_Step_Set( TMSEN, 0 );
		sSensor.mode = _e_SensorMode_Idle;
	}
}

void _sensor_task_alarm(void)
{
	TaskManager_Set( TMSEN , _e_sensor_task_alarm );

	if( TaskManager_First_Get( TMSEN ) == false )
	{
		TaskManager_First_Set( TMSEN );
		TaskManager_Step_Set( TMSEN, 0 );
		sSensor.mode = _e_SensorMode_Alarm;
	}

	sSensor.senByte[_e_SensorBit_Read].senGroup = _sensor_get_sensor();

	if( sSensor.senByte[_e_SensorBit_Read].senGroup > 0 )
	{
		if( (sSensor.senByte[_e_SensorBit_Read].senbit.out == true ) &&
			( sSensor.senByte[_e_SensorBit_Read].senbit.in == true ) ) {
			sSensor.status =  _e_SensorSts_Both;
			return;
		}

		if( sSensor.senByte[_e_SensorBit_Read].senbit.out == true ){
			sSensor.status =  _e_SensorSts_AlarmExit;
		}
		else if( sSensor.senByte[_e_SensorBit_Read].senbit.in == true ){
			sSensor.status =  _e_SensorSts_AlarmEntrance;
		}
		else{
			sSensor.status =  _E_SensorSts_AlarmMid;
		}
	}
	else{
		sSensor.status =   _e_SensorSts_None;
	}
}

void _sensor_task_entrance(void)
{
	TaskManager_Set( TMSEN , _e_sensor_task_entrance );

	if( TaskManager_First_Get( TMSEN ) == false )
	{
		TaskManager_First_Set( TMSEN );
		TaskManager_Step_Set( TMSEN, 0 );

		sSensor.senByte[_e_SensorBit_Read].senGroup = 0;
		sSensor.senByte[_e_SensorBit_Mem].senGroup = 0;
		sSensor.status =  _e_SensorSts_PassWait;
		sSensor.mode = _e_SensorMode_Entrance;
	}

	sSensor.senByte[_e_SensorBit_Read].senGroup = _sensor_get_sensor();

	// DSB/LBM - 24/07/2024: Considera acesso passagem OKAY apenas após passar pelo ultimo sensor
	switch( TaskManager_Step_Get( TMSEN ) )
	{
		case 0:
			if (sSensor.senByte[_e_SensorBit_Mem].senGroup == 8)
			{
				sSensor.status =  _e_SensorSts_AlarmExit;
			}
			else
			{
				sSensor.senByte[_e_SensorBit_Mem].senGroup |= sSensor.senByte[_e_SensorBit_Read].senGroup;
				sSensor.status =  _e_SensorSts_PassWait;
			}

			if(sSensor.senByte[_e_SensorBit_Mem].senGroup >= 15)
			{
				TaskManager_Step_Inc( TMSEN );
			}
		break;

		case 1:
			if(sSensor.senByte[_e_SensorBit_Read].senGroup == 0)
			{
				sSensor.status =  _e_SensorSts_PassOk;
				TaskManager_Go(TMSEN, _sensor_task_idle);
			}
		break;

		default:
			TaskManager_Step_Set(TMSEN, 0);
		break;
	}

}

void _sensor_task_exit(void)
{
	TaskManager_Set( TMSEN , _e_sensor_task_exit );

	if( TaskManager_First_Get( TMSEN ) == false )
	{
		TaskManager_First_Set( TMSEN );
		TaskManager_Step_Set( TMSEN, 0 );

		sSensor.senByte[_e_SensorBit_Read].senGroup = 0;
		sSensor.senByte[_e_SensorBit_Mem].senGroup = 0;
		sSensor.status =  _e_SensorSts_PassWait;
		sSensor.mode = _e_SensorMode_Exit;
	}

	sSensor.senByte[_e_SensorBit_Read].senGroup = _sensor_get_sensor();

	// DSB/LBM - 24/07/2024: Considera acesso passagem OKAY apenas após passar pelo ultimo sensor
	switch( TaskManager_Step_Get( TMSEN ) )
	{
		case 0:
			if (sSensor.senByte[_e_SensorBit_Mem].senGroup == 1)
			{
				//sSensor.senByte[_e_SensorBit_Mem].senGroup = 0;
				sSensor.status =  _e_SensorSts_AlarmEntrance;
			}
			else
			{
				sSensor.senByte[_e_SensorBit_Mem].senGroup |= sSensor.senByte[_e_SensorBit_Read].senGroup;
				sSensor.status =  _e_SensorSts_PassWait;
			}

			if( sSensor.senByte[_e_SensorBit_Mem].senGroup >= 15)
			{
				TaskManager_Step_Inc( TMSEN );
			}
		break;

		case 1:
			if( sSensor.senByte[_e_SensorBit_Read].senGroup == 0 )
			{
				sSensor.status =  _e_SensorSts_PassOk;
				TaskManager_Go( TMSEN  , _sensor_task_idle );
			}
		break;

		default:
			TaskManager_Step_Set( TMSEN, 0 );
		break;
	}
}

/*** ** * EXT Funcs * ** ***/
void Sensor_Init(void)
{
	TaskManager_Clear( TMSEN );
	TaskManager_Go( TMSEN  , _sensor_task_idle );
}

tSensorSts Sensor_Get(void)
{
	return sSensor.status;
}

void Sensor_Set( tSensorMode senMode )
{
	switch( (uint8_t)senMode )
	{
		case _e_SensorMode_Entrance:
			sSensor.mode = _e_SensorMode_Entrance;
			TaskManager_Go( TMSEN  , _sensor_task_entrance );
		break;

		case _e_SensorMode_Exit:
			sSensor.mode = _e_SensorMode_Exit;
			TaskManager_Go( TMSEN  , _sensor_task_exit );
		break;

		case _e_SensorMode_Alarm:
			sSensor.mode = _e_SensorMode_Alarm;
			TaskManager_Go( TMSEN  , _sensor_task_alarm );
		break;

		default:
			sSensor.mode = _e_SensorMode_Idle;
			TaskManager_Go( TMSEN  , _sensor_task_idle );
		break;
	}
}

tSensorMode Sensor_GetMode(void)
{
	return sSensor.mode;
}

//END OF FILE **
