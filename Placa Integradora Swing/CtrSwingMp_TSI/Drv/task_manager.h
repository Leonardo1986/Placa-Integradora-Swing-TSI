/**
  ******************************************************************************
  * @file    task_manager.h
  * @author  Arthur Murata
  * @version V0.1
  * @date    26/08/2020
  * @brief   task_manager Header
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) TSI.
  *
  *

#include "task_manager.h"

#define ENC _e_TaskManager_Encoder

typedef enum
{
	_e_enc_task_init,
	_e_enc_task_run,
}t_enc_task;

static void _enc_task_init(void);


static void _enc_task_init(void)
{
	TaskManager_Set( ENC , _e_enc_task_init );

	if( TaskManager_First_Get( ENC ) == false )
	{
		TaskManager_First_Set( ENC );
		TaskManager_Step_Set( ENC, 0 );
	}

	switch( (uint8_t)TaskManager_Step_Get( ENC ) )
	{
		case 0:

			break;
		case 1:
			break;
		default:
			break;
	}
}

void Enc_Init(void)
{
	TaskManager_Clear( ENC );
	TaskManager_Go( ENC , _enc_task_init );
}


  *
  *
  *
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __task_manager_H
#define __task_manager_H
#ifdef __cplusplus
 extern "C" {
#endif

/*** ** * Includes * ** ***/
	 
#include "main.h"

/*** ** * Defines * ** ***/
	 
/*** ** * Types * ** ***/
	 
typedef void (*_TaskManager_t)(void);

typedef enum{
	_e_TaskManager_StepMotor1,
	_e_TaskManager_StepMotor2,
	_e_TaskManager_StepMotorX,

	_e_TaskManager_EncDisk,
	_e_TaskManager_Encoder,
	_e_TaskManager_EncStpMt1,
	_e_TaskManager_EncStpMt2,
	_e_TaskManager_EncStpMtX,

	_e_TaskManager_Position1,
	_e_TaskManager_Position2,
	_e_TaskManager_PositionX,
	_e_TaskManager_Position,

	_e_TaskManager_Sensor,

    _e_TaskManager_ApiSwing,
	_e_TaskManager_SwUrn,
	_e_TaskManager_TratPecket,

	_e_TaskManager_Qty,//<<só roda até o qty

	_e_TaskManager_Motor1,
	_e_TaskManager_Motor2,
	_e_TaskManager_MotorX,
	_e_TaskManager_Swing,


}tTaskManager;
	 
/*** ** * Funcs Ext * ** ***/

void 	TaskManager(void);

void 	TaskManager_Go( tTaskManager taskMod , _TaskManager_t taskToGo );
void 	TaskManager_Clear(tTaskManager taskMod );

void 	TaskManager_Set( tTaskManager taskMod , uint8_t taskRun );
uint8_t TaskManager_Get( tTaskManager taskMod );

void 	TaskManager_First_Set( tTaskManager taskMod );
void 	TaskManager_First_Clear( tTaskManager taskMod );
bool 	TaskManager_First_Get( tTaskManager taskMod );

void 	TaskManager_Step_Set( tTaskManager taskMod, uint8_t taskStep );
uint8_t TaskManager_Step_Get( tTaskManager taskMod );
void	TaskManager_Step_Inc( tTaskManager taskMod );
void 	TaskManager_Step_Dec( tTaskManager taskMod );

#ifdef __cplusplus
}
#endif
#endif /*__task_manager_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
