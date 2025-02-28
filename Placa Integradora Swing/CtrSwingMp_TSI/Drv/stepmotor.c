/**
  ******************************************************************************
  * @file    stepmotor.c
  * @author  Arthur Murata
  * @version V0.1
  * @date    12/11/2020
  * @brief   Step Motor Source
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) TSI.
  *
  ******************************************************************************
  */

/*** ** * Includes * ** ***/
#include "task_manager.h"
#include "main.h"
#include "stepmotor.h"
#include "inputs.h"
#include "tim.h"
#include "gpio.h"
#include "outputs.h"
#include "sw_config.h"

/*** ** * Defines * ** ***/
#define STPMTX				_e_TaskManager_StepMotorX
#define STPMT1				_e_TaskManager_StepMotor1
#define STPMT2				_e_TaskManager_StepMotor2

#define STPMT_TIM_PERIOD	TIM_MOTOR_PERIOD//10000

#define STPMT_MT_ENABLE()		_pin_set_l(GPIO_MT_ENN_Port, GPIO_MT_ENN_Pin)
#define STPMT_MT_DISABLE()		_pin_set_h(GPIO_MT_ENN_Port, GPIO_MT_ENN_Pin)

#define STPMT_MT1_TOGGLE()		_pin_tg(GPIO_MT1_STEP_Port, GPIO_MT1_STEP_Pin)
#define STPMT_MT2_TOGGLE()		_pin_tg(GPIO_MT2_STEP_Port, GPIO_MT2_STEP_Pin)

#define STPMT_MT_TOGGLE(x)		(x)?( STPMT_MT1_TOGGLE() ) : ( STPMT_MT2_TOGGLE() )

/*** ** * Types * ** ***/
typedef void (*_StpMtTask_t)(tStpMtNum motorNum);

/*** ** * Variables * ** ***/

struct{
	_StpMtTask_t	TASK;
}sMtask[_e_StpMt_Qty];

struct{
	tStpMtSts 		stsRun;
	tStpMtSts 		stsRunRet;
	tStpMtDir		direction;
//	tStpMtPower		stopMode;

	uint32_t		stepTot;
	uint32_t		stepSys;
	uint32_t		stepSysRampCounter[3];
	uint32_t		stepSysRampCompare[3];
	bool			firstRampFlag;
	//uint16_t		stepCalcInc;
	//uint16_t		stepCalcDec;
	uint32_t		period;
	uint32_t		speed;
	uint32_t		speedStart;
	uint16_t		accel;
	uint16_t		accelCompare;
}sMtSys[_e_StpMt_Qty];

struct{
	bool			ActionRequest;
	uint32_t		speedUser;
	tStpMtNum 		stpMtNum;
	tStpMtDir 		stpMtDir;
	float 			ang;
	uint32_t		speed;
	//uint32_t		speedStart;
	uint16_t		accel;
}sMtCtrl[_e_StpMt_Qty];

struct{
	uint32_t	stepEncAcc;
	uint32_t	stepPosAcc;
}sMtEncoder[_e_StpMt_Qty];

/*** ** * Local Funcs * ** ***/

void _stpmt_func_motor_stop(tStpMtNum stpMtNum)
{
	TIM_STPMT_STOP((uint8_t)stpMtNum);
}

void _stpmt_func_motor_start(tStpMtNum stpMtNum , tStpMtDir stpMtDir)
{
	STPMT_MT_ENABLE();
	TIM_STPMT_START((uint8_t)stpMtNum , (uint8_t)stpMtDir);
}

void _stpmt_func_motor_set(tStpMtNum stpMtNum , uint16_t period )
{
	TIM_STPMT_SET((uint8_t)stpMtNum, period);
}

void _stpmt_func_motor_move( tStpMtNum stpMtNum, tStpMtDir stpMtDir, float ang, uint16_t speed , uint16_t accel )
{
	uint16_t periodDelta, accTot;


	//Degrees in steps
	sMtSys[stpMtNum].stepTot = STPMT_PARAM_STEPS_PER_REV / 360 * ang;
	sMtSys[stpMtNum].stepSys = sMtSys[stpMtNum].stepTot;

	//Speed Verification
	if( speed > STPMT_MT_SPEED_MAX ){
		//sMtSys[stpMtNum].speed = STPMT_TIM_PERIOD - STPMT_MT_SPEED_MAX;
		sMtSys[stpMtNum].speed = sMtSys[stpMtNum].speedStart - STPMT_MT_SPEED_MAX;
	}
	else{
		//sMtSys[stpMtNum].speed = STPMT_TIM_PERIOD - speed;
		sMtSys[stpMtNum].speed = sMtSys[stpMtNum].speedStart - speed;
	}

	//Acceleration Verification
	if( accel > STPMT_MT_ACC_MAX )
		sMtSys[stpMtNum].accel = STPMT_MT_ACC_MAX;
	else
		sMtSys[stpMtNum].accel = accel;

	//Acc Calc
	if( sMtSys[stpMtNum].period >= sMtSys[stpMtNum].speed )
	{	//accel in first ramp
		sMtSys[stpMtNum].firstRampFlag = false;
		periodDelta = sMtSys[stpMtNum].period - sMtSys[stpMtNum].speed;
	}
	else
	{	//deaccel in first ramp
		sMtSys[stpMtNum].firstRampFlag = true;
		periodDelta = sMtSys[stpMtNum].speed - sMtSys[stpMtNum].period;
	}
		//first ramp
	sMtSys[stpMtNum].stepSysRampCounter[0] = 0;
	sMtSys[stpMtNum].stepSysRampCompare[0] = periodDelta / sMtSys[stpMtNum].accel;
		//deaccel raamp
	sMtSys[stpMtNum].stepSysRampCounter[2] = 0;
	//sMtSys[stpMtNum].stepSysRampCompare[2] = sMtSys[stpMtNum].speed / sMtSys[stpMtNum].accel;
	sMtSys[stpMtNum].stepSysRampCompare[2] = periodDelta / sMtSys[stpMtNum].accel;
		//plateau ramp
	accTot = sMtSys[stpMtNum].stepSysRampCompare[0] + sMtSys[stpMtNum].stepSysRampCompare[2];
	sMtSys[stpMtNum].stepSysRampCounter[1] = 0;
	sMtSys[stpMtNum].stepSysRampCompare[1] = sMtSys[stpMtNum].stepTot - accTot;

	//START
	sMtSys[stpMtNum].stsRun = _e_StpMtSts_Acce;
	sMtSys[stpMtNum].direction = stpMtDir;
	_stpmt_func_motor_start(stpMtNum , sMtSys[stpMtNum].direction);
}

/*** ** * TASK * ** ***/
typedef enum
{
	_e_stpmt_task_ctrl,
}t_stpmt_task_ctrl;

static void _stpmt_task_ctrl( tStpMtNum stpMtNum )
{
	tTaskManager STPMTASK = ( STPMT1 + (tTaskManager)stpMtNum );
	TaskManager_Set( STPMTASK , _e_stpmt_task_ctrl );
	if( TaskManager_First_Get( STPMTASK ) == false ){
		TaskManager_First_Set( STPMTASK );
		TaskManager_Step_Set( STPMTASK, 0 );
	}

	switch( (uint8_t)TaskManager_Step_Get( STPMTASK ) )
	{

	case 0://_e_stpmt_task_ctrl_idle

		if( sMtCtrl[stpMtNum].ActionRequest == true )
		{
			if( sMtSys[stpMtNum].stsRun == _e_StpMtSts_Stop )
			{
				_stpmt_func_motor_move(stpMtNum,sMtCtrl[stpMtNum].stpMtDir,sMtCtrl[stpMtNum].ang,sMtCtrl[stpMtNum].speed,sMtCtrl[stpMtNum].accel);
				sMtCtrl[stpMtNum].ActionRequest = false;
			}
			else
			{
				if( sMtCtrl[stpMtNum].stpMtDir != sMtSys[stpMtNum].direction )
				{
					//sMtCtrl[stpMtNum].TASK = _stpmt_task_ctrl_move_reverse;
					TaskManager_Step_Inc(STPMTASK);
				}
				else
				{
					_stpmt_func_motor_move(stpMtNum,sMtCtrl[stpMtNum].stpMtDir,sMtCtrl[stpMtNum].ang,sMtCtrl[stpMtNum].speed,sMtCtrl[stpMtNum].accel);
					sMtCtrl[stpMtNum].ActionRequest = false;
				}
			}
		}

		break;

	case 1://reverse direction
		sMtSys[stpMtNum].stsRun = _e_StpMtSts_Deac;
		sMtSys[stpMtNum].stepSys = sMtSys[stpMtNum].stepSysRampCompare[2];
		TaskManager_Step_Inc(STPMTASK);
		break;

	case 2://wait stop
		if( sMtSys[stpMtNum].stsRun == _e_StpMtSts_Stop )
		{
			TaskManager_Step_Set( STPMTASK, 0 );
		}
		break;

	default:
		TaskManager_Step_Set( STPMTASK , 0 );
		break;

	}
}

//STEP MOTOR MAIN TASK

static void _stpmt_TASK(void)
{
	sMtask[_e_StpMt_Pri].TASK(_e_StpMt_Pri);
	sMtask[_e_StpMt_Sec].TASK(_e_StpMt_Sec);
}

/*** ** * IRQ Funcs * ** ***/

void StepMotor_IT(tStpMtNum stpMtNum)
{
	if( sMtSys[stpMtNum].stepSys > 0 )
	{
		//ramp0 (acc/deac)
		if( sMtSys[stpMtNum].stsRun == _e_StpMtSts_Acce )
		{
			sMtSys[stpMtNum].stsRunRet = _e_StpMtSts_Acce;
			//ramp step control
			sMtSys[stpMtNum].stepSysRampCounter[0]++;
			if( sMtSys[stpMtNum].stepSysRampCounter[0] >= sMtSys[stpMtNum].stepSysRampCompare[0] ){
				sMtSys[stpMtNum].stsRun = _e_StpMtSts_Plat;
			}
			//period
			if( sMtSys[stpMtNum].firstRampFlag == false )//accel
			{
				if( sMtSys[stpMtNum].period > sMtSys[stpMtNum].speed ){
					sMtSys[stpMtNum].period -= sMtSys[stpMtNum].accel;
				}
				else{
					sMtSys[stpMtNum].period = sMtSys[stpMtNum].speed;
				}
			}
			else//deaccel
			{
				if( sMtSys[stpMtNum].period < sMtSys[stpMtNum].speed ){
					sMtSys[stpMtNum].period += sMtSys[stpMtNum].accel;
				}
				else{
					sMtSys[stpMtNum].period = sMtSys[stpMtNum].speed;
				}
			}
		}
		//ramp1 (plateau)
		else if( sMtSys[stpMtNum].stsRun == _e_StpMtSts_Plat )
		{
			sMtSys[stpMtNum].stsRunRet = _e_StpMtSts_Plat;
			//ramp step control
			sMtSys[stpMtNum].stepSysRampCounter[1]++;
			if( sMtSys[stpMtNum].stepSysRampCounter[1] >= sMtSys[stpMtNum].stepSysRampCompare[1] ){
				sMtSys[stpMtNum].stsRun = _e_StpMtSts_Deac;
			}
		}
		//ramp2 (deac)
		else
		{
			sMtSys[stpMtNum].stsRunRet = _e_StpMtSts_Deac;
			//ramp step control
			sMtSys[stpMtNum].stepSysRampCounter[2]++;
			if( sMtSys[stpMtNum].stepSysRampCounter[2] >= sMtSys[stpMtNum].stepSysRampCompare[2] ){
				//sMtSys[stpMtNum].stsRun = _e_StpMtSts_Stop;
				__NOP();
			}
			//period
			if( sMtSys[stpMtNum].period < STPMT_TIM_PERIOD ){
				sMtSys[stpMtNum].period += sMtSys[stpMtNum].accel;
			}
			else{
				sMtSys[stpMtNum].period = STPMT_TIM_PERIOD;
			}
		}

		sMtSys[stpMtNum].stepSys--;

		_stpmt_func_motor_set( stpMtNum , sMtSys[stpMtNum].period );

		sMtEncoder[stpMtNum].stepEncAcc++;
		sMtEncoder[stpMtNum].stepPosAcc++;

#warning "melhorar metralhadora na hora do debug, só descomentar a linha abaixo"
//		STPMT_MT_TOGGLE(stpMtNum);
	}
	else
	{
		//sMtSys[stpMtNum].period = STPMT_TIM_PERIOD;
		sMtSys[stpMtNum].period = sMtSys[stpMtNum].speedStart;
		_stpmt_func_motor_set( stpMtNum , sMtSys[stpMtNum].period );
		_stpmt_func_motor_stop(stpMtNum);
		sMtSys[stpMtNum].stsRun = _e_StpMtSts_Stop;
		sMtSys[stpMtNum].stsRunRet = _e_StpMtSts_Stop;
		__NOP();
	}
}

/*** ** * Exported Funcs * ** ***/

void StepMotor_Init(void)
{
	sMtSys[_e_StpMt_Pri].period = STPMT_TIM_PERIOD;
	sMtSys[_e_StpMt_Sec].period = STPMT_TIM_PERIOD;

	sMtask[_e_StpMt_Pri].TASK = _stpmt_task_ctrl;
	sMtask[_e_StpMt_Sec].TASK = _stpmt_task_ctrl;

	TaskManager_Clear( STPMT1 );
	TaskManager_Clear( STPMT2 );
	TaskManager_Clear( STPMTX);

	TaskManager_Go( STPMT1 , NULL );
	TaskManager_Go( STPMT2 , NULL );
	TaskManager_Go( STPMTX , _stpmt_TASK );

	_stpmt_func_motor_stop(_e_StpMt_Pri);
	_stpmt_func_motor_stop(_e_StpMt_Sec);

	sMtEncoder[_e_StpMt_Pri].stepEncAcc = 0;
	sMtEncoder[_e_StpMt_Sec].stepEncAcc = 0;

	sMtEncoder[_e_StpMt_Pri].stepPosAcc = 0;
	sMtEncoder[_e_StpMt_Sec].stepPosAcc = 0;
}

tStpMtSts StepMotor_GetSts( tStpMtNum stpMtNum )
{
	return sMtSys[stpMtNum].stsRunRet;
}

void StepMotor_GO( tStpMtNum stpMtNum , tStpMtDir stpMtDir , float ang , uint16_t speed , uint8_t accel , uint16_t speedStart )
{
	if( sMtCtrl[stpMtNum].ActionRequest == true )return;

	sMtCtrl[stpMtNum].ActionRequest = true;
	sMtCtrl[stpMtNum].stpMtDir = stpMtDir;
	sMtCtrl[stpMtNum].ang = ang;
	sMtCtrl[stpMtNum].speed = speed;
	sMtCtrl[stpMtNum].accel = accel;
	sMtSys[stpMtNum].stsRunRet = _e_StpMtSts_Acce;
	sMtSys[stpMtNum].speedStart = STPMT_TIM_PERIOD - speedStart;
}

void StepMotor_STOP( tStpMtNum stpMtNum )
{
	sMtSys[stpMtNum].stepSys = 0;
//	if( sMtSys[stpMtNum].stsRun != _e_StpMtSts_Deac )
//	{
//		sMtSys[stpMtNum].stepSys = sMtSys[stpMtNum].stepSysRampCompare[2];
//	}
}

void StepMotor_POWER( tStpMtPower power )
{
	if( power == _e_StpMtPower_ON ){
		ENABLE_MOTORS();
	}
	else{
		DISABLE_MOTORS();
	}
}

uint16_t StepMotor_GetStepEnc( tStpMtNum stpMtNum )
{
	uint16_t stepEncAcc = sMtEncoder[stpMtNum].stepEncAcc;
	sMtEncoder[stpMtNum].stepEncAcc = 0;
	return stepEncAcc;
}

uint16_t StepMotor_GetStepPos( tStpMtNum stpMtNum )
{
	uint16_t stepPosAcc = sMtEncoder[stpMtNum].stepPosAcc;
	sMtEncoder[stpMtNum].stepPosAcc = 0;
	return stepPosAcc;
}

tStpMtDir	StepMotor_GetDir( tStpMtNum stpMtNum )
{
	return sMtCtrl[stpMtNum].stpMtDir;
}
