/**
  ******************************************************************************
  * @file    encoder.c
  * @author  Arthur Murata
  * @version V0.1
  * @date    11/11/2020
  * @brief   Encoder Source
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) TSI.
  *
  ******************************************************************************
  */

/*** ** * Includes * ** ***/
#include "encoder.h"
#include "task_manager.h"
//#include "main.h"
#include "inputs.h"
#include "stepmotor.h"
#include "timing.h"
#include "sw_config.h"

/*** ** * Defines * ** ***/
#define ENCDISK		_e_TaskManager_EncDisk
#define ENC 		_e_TaskManager_Encoder

#define ENCSTPMT1	_e_TaskManager_EncStpMt1
#define ENCSTPMT2	_e_TaskManager_EncStpMt2
#define ENCSTPMTX	_e_TaskManager_EncStpMtX

#define ENC_DISK_POS_QTY	3
#define ENC_DISK_ENC_QTY	16

//Disk Param

#define ENC_DISK_PARAM_TOT	214
#define ENC_DISK_PARAM_OUT	190
#define ENC_DISK_PARAM_IN	40
#define ENC_DISK_PARAM_SEN	30


#define ENC_ANG_MIN	22//24.5 //27
#define ENC_ANG_MAX 187//189.5//187

/*** ** * Variables * ** ***/

//const uint8_t 	constDiscPos[ENC_DISK_POS_QTY] = {   2,   3,   1,   0,   2,   3,   1 };
//const uint8_t	constDiscVal[ENC_DISK_POS_QTY] = {   0,  45,  75,  85, 115, 160, 160 };
//const uint8_t	constDiscVa1[ENC_DISK_POS_QTY] = {  27,  72, 102, 112, 142, 187, 187 };
//const uint8_t	constDiscPer[ENC_DISK_POS_QTY] = {   0,  28,  46, 112, 142, 100, 100 };

typedef enum{
	_e_EcnDiscPos_Error = 0,
	_e_EcnDiscPos_Left,
	_e_EcnDiscPos_Right,
	_e_EcnDiscPos_Middle,

}tEcnDiscPos;


struct{
	tEcnDiscPos		discPos;
	uint16_t	encCount[ENC_DISK_POS_QTY];
	uint16_t	stepCount[ENC_DISK_POS_QTY];
	float		ang[ENC_DISK_POS_QTY];
	float		encRes[ENC_DISK_POS_QTY];
	uint8_t		encSeq[4];
	bool		encSeqFlag;
	tEncCalib	status;

	float		angMin;
	float		angMax;
}sEncCalib[_e_EncNum_Qty];

struct{
	//position disk
	bool	diskPosChange;
	uint8_t	buffDiskPos[ENC_DISK_POS_QTY];
	uint8_t	buffIndex;
	uint8_t	position;
	//encoder disk
	uint8_t	buffDiskEnc[ENC_DISK_ENC_QTY];
	uint8_t	buffDiskEncInd;
	uint8_t	lastDiskEnc;
	bool	diskEncChange;
}sEncDisk[_e_EncNum_Qty];

struct{
	float	ang;
	float	angCalc;
	float	angPercent;
	float	angPercentSys;
	uint8_t	index;
	uint8_t	indexLast;
	tEcnDiscPos diskPosNow;
	tEcnDiscPos diskPosLast;
	tEncDir dir;
	uint8_t	encCount[ENC_DISK_POS_QTY];
}sEncPosMap[_e_EncNum_Qty];

struct{
	bool		state[4];
	uint8_t		stateCount;
	uint16_t	stepAcc;
	bool		breakFlag;
}sEncBreak[_e_EncNum_Qty];

struct{
	tEncSts			status;
	tEncCmd			command;
	tEncStopMode 	stopMode;
	uint8_t			angPercentUser;
	uint8_t			angUser;
	//uint8_t			angUserCmp;

	float			angToMove;
	tStpMtDir 		dirToMove;
	uint16_t		spdToMove;
	uint16_t		spsToMove;
	uint16_t		accToMove;
	tEncDir 		direction;
}sEncCtrl[_e_EncNum_Qty];

struct{
	tStpMtDir 	dir;
	tStpMtDir 	dirMem;
	uint8_t 	autoCalib;
}sEncTest[_e_EncNum_Qty];

struct _EncMoveParam
{
  float param[_e_EncMoveParam_Qty];
};

const struct _EncMoveParam sEncMoveParam[2] =
{
	{{
		ENC_MOVE_PARAM_L_SPEED_MAX,
		ENC_MOVE_PARAM_L_ACC_MAX,
		ENC_MOVE_PARAM_L_ANG_SPEED_MAX,
		ENC_MOVE_PARAM_L_RAMP_SPEED_MIN,
		ENC_MOVE_PARAM_L_RAMP_ACC_MIN,
		ENC_MOVE_PARAM_L_CORRECT_PER
	}},
	{{
		ENC_MOVE_PARAM_R_SPEED_MAX,
		ENC_MOVE_PARAM_R_ACC_MAX,
		ENC_MOVE_PARAM_R_ANG_SPEED_MAX,
		ENC_MOVE_PARAM_R_RAMP_SPEED_MIN,
		ENC_MOVE_PARAM_R_RAMP_ACC_MIN,
		ENC_MOVE_PARAM_R_CORRECT_PER
	}},
};

//Motors

typedef void (*_EncMtTask_t)(tEncNum encNum);

struct{
	_EncMtTask_t	TASK;
}sEncMtask[_e_StpMt_Qty];

/*** ** * Local Funcs * ** ***/

void _encdisk_func_get_pos(tEncNum encNum , uint8_t* pos , uint8_t* lastPos)
{
	*pos = sEncDisk[encNum].buffDiskPos[sEncDisk[encNum].buffIndex];

	if( sEncDisk[encNum].buffIndex == 0 ){
		//return (ENC_DISK_POS_QTY - 1);
		*lastPos  	= sEncDisk[encNum].buffDiskPos[ENC_DISK_POS_QTY - 1];
	}
	else{
		//return ( sEncDisk[encNum].buffIndex - 1 );
		*lastPos  	= sEncDisk[encNum].buffDiskPos[sEncDisk[encNum].buffIndex - 1];
	}
}

void _encdisk_func_inc_buffindex(tEncNum encNum)
{
	if( sEncDisk[encNum].buffIndex == (ENC_DISK_POS_QTY - 1) ){
		sEncDisk[encNum].buffIndex = 0;
	}
	else{
		sEncDisk[encNum].buffIndex++;
	}
}

void _encdisk_func_break_clear(tEncNum encNum)
{
	for( uint8_t i = 0 ; i < 4 ; i++ )
	{
		sEncBreak[encNum].state[i] = false;
	}
	sEncBreak[encNum].stepAcc = 0;
	sEncBreak[encNum].breakFlag = false;
}

bool _encdisk_func_break_get(tEncNum encNum)
{
	return sEncBreak[encNum].breakFlag;
}

void _enc_stop_mode(tEncNum encNum)
{
	if( sEncCtrl[encNum].stopMode == _e_EncStopMode_On ){
		StepMotor_POWER( _e_StpMtPower_ON );
	}
	else{
		StepMotor_POWER( _e_StpMtPower_OFF );
	}
}

/*** ** * TASK ENC MAP * ** ***/
typedef enum
{
	_e_enc_task,
}t_enc_task;


static void _enc_task(void)//tarefa disco posição
{
	TaskManager_Set( ENC , _e_enc_task );

	if( TaskManager_First_Get( ENC ) == false )
	{
		TaskManager_First_Set( ENC );
		TaskManager_Step_Set( ENC, 0 );
	}

	switch( TaskManager_Step_Get( ENC ) )
	{

	case 0:

		for( tEncNum encNum = _e_EncNum_Pri ; encNum < _e_EncNum_Qty ; encNum++ )
		{
			sEncPosMap[encNum].index = 0xFF;
			sEncPosMap[encNum].dir = _e_EncDir_Unknown;
			sEncPosMap[encNum].ang = 0xFF;
			sEncDisk[encNum].diskPosChange = true;
//			if( Inputs_Motor_Disc_Get((tInMt)encNum) == 0 )
//				sEncPosMap[encNum].index = 0;
		}
		TaskManager_Step_Inc( ENC );

		break;

	case 1:

		for( tEncNum encNum = _e_EncNum_Pri ; encNum < _e_EncNum_Qty ; encNum++ )
		{
			if( sEncDisk[encNum].diskPosChange == true )
			{
				sEncDisk[encNum].diskPosChange = false;

				_encdisk_func_get_pos(encNum , &sEncPosMap[encNum].diskPosNow , &sEncPosMap[encNum].diskPosLast );

				//sEncPosMap[encNum].indexLast = sEncPosMap[encNum].index;

				switch((uint8_t) sEncPosMap[encNum].diskPosNow )
				{
				case _e_EcnDiscPos_Left:
					sEncPosMap[encNum].index = 0;

					if(sEncPosMap[encNum].diskPosLast == _e_EcnDiscPos_Middle){
						sEncPosMap[encNum].dir = _e_EncDir_Left;
						sEncPosMap[encNum].ang = ENC_ANG_MIN;
					}
					else{
						sEncPosMap[encNum].dir = _e_EncDir_Unknown;
					}
					break;

				case _e_EcnDiscPos_Right:

					sEncPosMap[encNum].index = 2;

					if(sEncPosMap[encNum].diskPosLast == _e_EcnDiscPos_Middle){
						sEncPosMap[encNum].dir = _e_EncDir_Right;
						sEncPosMap[encNum].ang = ENC_ANG_MAX;
					}
					else{
						sEncPosMap[encNum].dir = _e_EncDir_Unknown;
					}
					break;

				case _e_EcnDiscPos_Middle:
					sEncPosMap[encNum].index = 1;

					if(sEncPosMap[encNum].diskPosLast == _e_EcnDiscPos_Right){
						sEncPosMap[encNum].dir = _e_EncDir_Left;
						sEncPosMap[encNum].ang = ENC_ANG_MAX;
					}
					else if(sEncPosMap[encNum].diskPosLast == _e_EcnDiscPos_Left){
						sEncPosMap[encNum].dir = _e_EncDir_Right;
						sEncPosMap[encNum].ang = ENC_ANG_MIN;
					}
					else{
						sEncPosMap[encNum].dir = _e_EncDir_Unknown;
					}
					break;

				case _e_EcnDiscPos_Error:
				default:

					__NOP();

					break;

				}

			}
		}

		break;

	default:
		TaskManager_Step_Set( ENC, 0 );
		break;
	}
}

/*** ** * TASK ENC DISK * ** ***/

typedef enum
{
	_e_encdisk_task,
}t_encdisk_task;

static void _encdisk_task(void)//encbreak
{
	TaskManager_Set( ENCDISK , _e_encdisk_task );

	if( TaskManager_First_Get( ENCDISK ) == false )
	{
		TaskManager_First_Set( ENCDISK );
		TaskManager_Step_Set( ENCDISK, 0 );
	}

	switch( TaskManager_Step_Get( ENCDISK ) )
	{
	case 0://zera buffer

		for( tEncNum encNum = _e_EncNum_Pri ; encNum < _e_EncNum_Qty ; encNum++ )
		{
			sEncDisk[encNum].buffIndex = 0;

			for( uint8_t i = 0 ; i < ENC_DISK_POS_QTY ; i++ ){
				sEncDisk[encNum].buffDiskPos[i] = 0xFF;
			}
			for( uint8_t i = 0 ; i < ENC_DISK_ENC_QTY ; i++ ){
				sEncDisk[encNum].buffDiskEnc[i] = 0xFF;
			}
			for( uint8_t i = 0 ; i < 4 ; i++ ){
				sEncBreak[encNum].state[i] = false;
			}

			sEncDisk[encNum].buffDiskPos[sEncDisk[encNum].buffIndex] = Inputs_Motor_Disc_Get((tInMt)encNum);
			sEncDisk[encNum].buffDiskEnc[sEncDisk[encNum].buffIndex] = Inputs_Motor_Enc_Get((tInMt)encNum);

			sEncDisk[encNum].diskPosChange = false;
		}

		TaskManager_Step_Inc( ENCDISK );

		break;

	case 1:

		for( tEncNum encNum = _e_EncNum_Pri ; encNum < _e_EncNum_Qty ; encNum++ )
		{
			//ENCODER BREAK
			sEncBreak[encNum].stepAcc += StepMotor_GetStepEnc( (tStpMtNum)encNum );
			if( sEncBreak[encNum].stepAcc > ENC_BREAK_COUNT )
			{
				sEncBreak[encNum].breakFlag = true;
				StepMotor_STOP((tStpMtNum)encNum);
			}



			if( sEncCalib[encNum].encCount[0] >= ENC_ALIGN_OPEN )
			{
				sEncCalib[encNum].angMin = sEncCalib[encNum].ang[1] - ( sEncCalib[encNum].encRes[1] * ( sEncCalib[encNum].encCount[0] - 2 ) );
				sEncCalib[encNum].angMax = sEncCalib[encNum].ang[2] + ( sEncCalib[encNum].encRes[1] * ( sEncCalib[encNum].encCount[2] - 2 ) );
			}
			else
			{
				sEncCalib[encNum].angMin = sEncCalib[encNum].ang[1] - ( sEncCalib[encNum].encRes[1] * ( sEncCalib[encNum].encCount[0] ));
				sEncCalib[encNum].angMax = sEncCalib[encNum].ang[2] + ( sEncCalib[encNum].encRes[1] * ( sEncCalib[encNum].encCount[2] ));
			}

			if( sEncPosMap[encNum].ang < sEncCalib[encNum].angMin )
			{
				sEncPosMap[encNum].angPercent = 0;
			}
			else if( sEncPosMap[encNum].ang > sEncCalib[encNum].angMax )
			{
				sEncPosMap[encNum].angPercent = 100;
			}
			else
			{
				sEncPosMap[encNum].angCalc = sEncPosMap[encNum].ang - sEncCalib[encNum].ang[1];
				sEncPosMap[encNum].angPercent =  (sEncPosMap[encNum].ang - sEncCalib[encNum].angMin) * 100 / (sEncCalib[encNum].angMax-sEncCalib[encNum].angMin) ;
			}
		}

		break;

	default:
		TaskManager_Step_Set( ENCDISK, 0 );
		break;
	}
}

/*** ** * TASK ENC STEP MOTOR * ** ***/

typedef enum
{
	_e_encmt_task_init,
	_e_encmt_task_idle,
	_e_encmt_task_move,
	_e_encmt_task_cali,
	_e_encmt_task_move_to_stop,
}t_encmt_task;

static void _encmt_task_init( tEncNum encNum );
static void _encmt_task_idle( tEncNum encNum );
static void _encmt_task_move( tEncNum encNum );
static void _encmt_task_cali( tEncNum encNum );
static void _encmt_task_move_to_stop( tEncNum encNum );

static void _encmt_task_init( tEncNum encNum )
{
	tTaskManager ENCMTASK = ( ENCSTPMT1 + (tTaskManager)encNum );
	TaskManager_Set( ENCMTASK , _e_encmt_task_init );
	if( TaskManager_First_Get( ENCMTASK ) == false ){
		TaskManager_First_Set( ENCMTASK );
		TaskManager_Step_Set( ENCMTASK, 0 );
		StepMotor_STOP((tStpMtNum)encNum);
		sEncCtrl[encNum].status = _e_EncSts_Init;
	}

	switch( TaskManager_Step_Get( ENCMTASK ) )
	{
	case 0:
		sEncCalib[encNum].status = _e_EncCalib_Nok;
		TaskManager_Step_Inc(ENCMTASK);
		break;
	case 1:
		sEncMtask[encNum].TASK = _encmt_task_idle;
		break;
	default:
		TaskManager_Step_Set( ENCMTASK, 0 );
		break;
	}
}

static void _encmt_task_idle( tEncNum encNum )
{
	tTaskManager ENCMTASK = ( ENCSTPMT1 + (tTaskManager)encNum );
	TaskManager_Set( ENCMTASK , _e_encmt_task_idle );
	if( TaskManager_First_Get( ENCMTASK ) == false ){
		TaskManager_First_Set( ENCMTASK );
		TaskManager_Step_Set( ENCMTASK, 0 );
		//sEncCtrl[encNum].command = _e_EncCmd_None;
		//sEncCtrl[encNum].status = _e_EncSts_Idle;
	}

	if( sEncCtrl[encNum].command == _e_EncCmd_None )
	{
		if( StepMotor_GetSts( (tStpMtNum)encNum ) != _e_StpMtSts_Stop ){
			sEncCtrl[encNum].status = _e_EncSts_Move;
		}
		else{
			sEncCtrl[encNum].status = _e_EncSts_Idle;
		}
	}

	switch( (uint8_t)sEncCtrl[encNum].command )
	{
	case _e_EncCmd_Cali:
		sEncMtask[encNum].TASK = _encmt_task_cali;
		break;
	case _e_EncCmd_Move:
		sEncMtask[encNum].TASK = _encmt_task_move;
		break;
	case _e_EncCmd_MoveToStop:
		sEncMtask[encNum].TASK = _encmt_task_move_to_stop;
		break;
	case _e_EncCmd_Stop:
		StepMotor_STOP((tStpMtNum)encNum);
		_enc_stop_mode(encNum);
		sEncCtrl[encNum].command = _e_EncCmd_None;
		break;
	}

}

static void _encmt_task_move_to_stop( tEncNum encNum )
{
	tTaskManager ENCMTASK = ( ENCSTPMT1 + (tTaskManager)encNum );
	TaskManager_Set( ENCMTASK , _e_encmt_task_move_to_stop );
	if( TaskManager_First_Get( ENCMTASK ) == false ){
		TaskManager_First_Set( ENCMTASK );
		TaskManager_Step_Set( ENCMTASK, 0 );
		sEncCtrl[encNum].status = _e_EncSts_Move;
	}

	switch( TaskManager_Step_Get( ENCMTASK ) )
	{
	case 0://Joga para o lado setado
		Timing_Set( _e_Timing_Encoder_Break , TIMEOUT_CALIB_DINAM * 2 /*10000 */);
		_encdisk_func_break_clear(encNum);
		StepMotor_GO( (tStpMtNum)encNum , (tStpMtDir)sEncCtrl[encNum].direction , 200 , STPMT_MT_SPEED_CAL , STPMT_MT_ACC_CAL , STPMT_MT_SPEED_INI_CAL );
		TaskManager_Step_Inc(ENCMTASK);
		break;

	case 1://aguarda fim de curso
		if( _encdisk_func_break_get(encNum) )
		{
			StepMotor_STOP((tStpMtNum)encNum);
			TaskManager_Step_Inc(ENCMTASK);
		}
		else if( Timing_Get_Bool( _e_Timing_Encoder_Break ) ){
			sEncCalib[encNum].status = _e_EncCalib_TimeOut1;
			TaskManager_Step_Inc(ENCMTASK);
		}
		break;

	case 2:
		sEncCtrl[encNum].command = _e_EncCmd_None;
		sEncMtask[encNum].TASK = _encmt_task_idle;
		break;

	default:
		TaskManager_Step_Set( ENCMTASK, 0 );
		break;
	}

}



static void _encmt_task_move( tEncNum encNum )
{
	tEncDir dir;

	tTaskManager ENCMTASK = ( ENCSTPMT1 + (tTaskManager)encNum );
	TaskManager_Set( ENCMTASK , _e_encmt_task_move );
	if( TaskManager_First_Get( ENCMTASK ) == false ){
		TaskManager_First_Set( ENCMTASK );
		TaskManager_Step_Set( ENCMTASK, 0 );
		sEncCtrl[encNum].status = _e_EncSts_Move;
	}

	if(encNum == _e_EncNum_Pri){
		__NOP();
	}
	else{
		__NOP();
	}

	switch( TaskManager_Step_Get( ENCMTASK ) )
	{
	case 0:

		if( sEncCtrl[encNum].angUser > sEncPosMap[encNum].ang ){
			sEncCtrl[encNum].angToMove = sEncCtrl[encNum].angUser - sEncPosMap[encNum].ang;
			sEncCtrl[encNum].dirToMove = _e_StMtDir_Right;
			dir = _e_StMtDir_Right;
		}
		else if( sEncCtrl[encNum].angUser < sEncPosMap[encNum].ang ){
			sEncCtrl[encNum].angToMove = sEncPosMap[encNum].ang - sEncCtrl[encNum].angUser;
			sEncCtrl[encNum].dirToMove = _e_StMtDir_Left;
			dir = _e_StMtDir_Left;
		}
		else{
			sEncCtrl[encNum].command = _e_EncCmd_None;
			sEncMtask[encNum].TASK = _encmt_task_idle;
			return;
		}

/*
		if( sEncCtrl[encNum].angToMove > ENC_MOVE_PARAM_ANG_SPEED_MAX ){
			sEncCtrl[encNum].spdToMove = STPMT_MT_SPEED_MAX;
			sEncCtrl[encNum].accToMove = STPMT_MT_ACC_MAX;
		}
		else{
			uint16_t speedMinCalc = STPMT_MT_SPEED_MAX * ENC_MOVE_PARAM_RAMP_SPEED_MIN;
			uint16_t speedBaseCalc = STPMT_MT_SPEED_MAX - speedMinCalc;
			sEncCtrl[encNum].spdToMove = ( sEncCtrl[encNum].angToMove * speedBaseCalc / ENC_MOVE_PARAM_ANG_SPEED_MAX ) + speedMinCalc;

			uint8_t	accMinCalc = STPMT_MT_ACC_MAX * ENC_MOVE_PARAM_RAMP_ACC_MIN;
			uint8_t	accBaseCalc = STPMT_MT_ACC_MAX - accMinCalc;
			sEncCtrl[encNum].accToMove = ( sEncCtrl[encNum].angToMove * accBaseCalc / ENC_MOVE_PARAM_ANG_SPEED_MAX ) + accMinCalc;
		}

		sEncCtrl[encNum].angToMove = sEncCtrl[encNum].angToMove * ENC_MOVE_PARAM_CORRECTION_PERCENT;
*/
		if( sEncCtrl[encNum].angToMove > sEncMoveParam[dir].param[_e_EncMoveParam_AngSpeedMax] ){
			sEncCtrl[encNum].spdToMove = sEncMoveParam[dir].param[_e_EncMoveParam_SpeedMax];
			sEncCtrl[encNum].accToMove = sEncMoveParam[dir].param[_e_EncMoveParam_AccMax];
		}
		else{
			uint16_t speedMinCalc = sEncMoveParam[dir].param[_e_EncMoveParam_SpeedMax] * sEncMoveParam[dir].param[_e_EncMoveParam_RampSpeedMin];
			uint16_t speedBaseCalc = sEncMoveParam[dir].param[_e_EncMoveParam_SpeedMax] - speedMinCalc;
			sEncCtrl[encNum].spdToMove = ( sEncCtrl[encNum].angToMove * speedBaseCalc / sEncMoveParam[dir].param[_e_EncMoveParam_AngSpeedMax] ) + speedMinCalc;

			uint16_t	accMinCalc = sEncMoveParam[dir].param[_e_EncMoveParam_AccMax] * sEncMoveParam[dir].param[_e_EncMoveParam_RampAccMin];
			uint16_t	accBaseCalc = sEncMoveParam[dir].param[_e_EncMoveParam_AccMax] - accMinCalc;
			sEncCtrl[encNum].accToMove = ( sEncCtrl[encNum].angToMove * accBaseCalc / sEncMoveParam[dir].param[_e_EncMoveParam_AngSpeedMax] ) + accMinCalc;

			uint16_t startSpeedDelta = STPMT_MT_SPEED_INI_MAX - STPMT_MT_SPEED_INI_MIN;
			sEncCtrl[encNum].spsToMove = sEncCtrl[encNum].angToMove * startSpeedDelta / sEncMoveParam[dir].param[_e_EncMoveParam_AngSpeedMax];
		}

		sEncCtrl[encNum].angToMove = sEncCtrl[encNum].angToMove * sEncMoveParam[dir].param[_e_EncMoveParam_CorrectPer];

		StepMotor_GO( 	(tStpMtNum)encNum ,
						sEncCtrl[encNum].dirToMove ,
						sEncCtrl[encNum].angToMove ,
						sEncCtrl[encNum].spdToMove ,
						sEncCtrl[encNum].accToMove ,
						sEncCtrl[encNum].spsToMove );

		TaskManager_Step_Inc(ENCMTASK);
		break;

	case 1://zera comando
		sEncCtrl[encNum].command = _e_EncCmd_None;
		TaskManager_Step_Inc(ENCMTASK);
		break;

	case 2:
		//verific se tem comando
		/*
		if( sEncCtrl[encNum].command != _e_EncCmd_None ){
			TaskManager_Step_Set( ENCMTASK, 5 );
		}
		*/
		/*
		if( sEncCtrl[encNum].command == _e_EncCmd_Stop ){
			StepMotor_STOP((tStpMtNum)encNum);
		}
		*/

		//Verifica motor
		if( StepMotor_GetSts( (tStpMtNum)encNum ) != _e_StpMtSts_Stop )
		{
			if( sEncCtrl[encNum].dirToMove == _e_StMtDir_Right )
			{
				if(/* sEncPosMap[encNum].ang > ENC_ANG_MAX ||*/ sEncPosMap[encNum].ang >= sEncCtrl[encNum].angUser || sEncPosMap[encNum].ang >= sEncCalib[encNum].angMax ){
					StepMotor_STOP((tStpMtNum)encNum);
					_enc_stop_mode(encNum);
					TaskManager_Step_Set( ENCMTASK, 5 );
				}
			}
			else if( sEncCtrl[encNum].dirToMove == _e_StMtDir_Left )
			{
				if(/* sEncPosMap[encNum].ang < ENC_ANG_MIN ||*/ sEncPosMap[encNum].ang <= sEncCtrl[encNum].angUser || sEncPosMap[encNum].ang <= sEncCalib[encNum].angMin ){
					StepMotor_STOP((tStpMtNum)encNum);
					_enc_stop_mode(encNum);
					TaskManager_Step_Set( ENCMTASK, 5 );
				}
			}
		}
		else{
			TaskManager_Step_Set( ENCMTASK, 5 );
		}
		break;

	case 5:
		sEncCtrl[encNum].command = _e_EncCmd_None;
		sEncMtask[encNum].TASK = _encmt_task_idle;
		break;

	default:
		TaskManager_Step_Set( ENCMTASK, 0 );
		break;
	}
}


typedef enum{
	_e_encmt_task_cali_turn_left,
	_e_encmt_task_cali_turn_left_wait,
	_e_encmt_task_cali_wait_stop,
	_e_encmt_task_cali_turn_right,
	_e_encmt_task_cali_read_values,
	_e_encmt_task_cali_calc_values,

	_e_encmt_task_cali_ok,
	_e_encmt_task_cali_error,

	_e_encmt_task_cali_test1,
	_e_encmt_task_cali_test2,
	_e_encmt_task_cali_test3,
}t_encmt_task_cali;

float encCounterTestCalib = 0;
float encCounterTest = 0;

static void _encmt_task_cali( tEncNum encNum )
{
	tTaskManager ENCMTASK = ( ENCSTPMT1 + (tTaskManager)encNum );
	TaskManager_Set( ENCMTASK , _e_encmt_task_cali );
	if( TaskManager_First_Get( ENCMTASK ) == false ){
		TaskManager_First_Set( ENCMTASK );
		TaskManager_Step_Set( ENCMTASK, _e_encmt_task_cali_turn_left );
		StepMotor_STOP((tStpMtNum)encNum);
		sEncCtrl[encNum].status = _e_EncSts_Cali;
		sEncCalib[encNum].status = _e_EncCalib_Running;
	}

	switch( TaskManager_Step_Get( ENCMTASK ) )
	{
	case _e_encmt_task_cali_turn_left://Joga pra esquerda

		Timing_Set( _e_Timing_Encoder_Break , TIMEOUT_CALIB_DINAM /*5000*/ );
		_encdisk_func_break_clear(encNum);
		StepMotor_GO( (tStpMtNum)encNum , _e_StMtDir_Left , 250 , STPMT_MT_SPEED_CAL , STPMT_MT_ACC_CAL , STPMT_MT_SPEED_INI_CAL );
		TaskManager_Step_Inc(ENCMTASK);
		break;

	case _e_encmt_task_cali_turn_left_wait://aguarda fim de curso

//		if( _encdisk_func_break_get(encNum) )//para quando fim de curso que agora vai ser pelo diskPos
		if(sEncPosMap[encNum].diskPosNow == _e_EcnDiscPos_Left)
		{
			StepMotor_STOP((tStpMtNum)encNum);
			TaskManager_Step_Inc(ENCMTASK);
			Timing_Set( _e_Timing_Encoder_WaitStop , 500 );
		}
		else if( Timing_Get_Bool( _e_Timing_Encoder_Break ) ){//para quando timeout
			sEncCalib[encNum].status = _e_EncCalib_TimeOut1;
			TaskManager_Step_Set( ENCMTASK, _e_encmt_task_cali_error );
		}
		break;

	case _e_encmt_task_cali_wait_stop://aguarda um tempinho de estabilização
		if( Timing_Get_Bool( _e_Timing_Encoder_WaitStop ) ){
			TaskManager_Step_Inc(ENCMTASK);
		}
		break;

	case _e_encmt_task_cali_turn_right://Joga pra direita

		//Limpa variaveis de calibra��o
		sEncPosMap[encNum].index = 0;
		sEncPosMap[encNum].ang = 0;
		sEncCalib[encNum].ang[sEncPosMap[encNum].index] = 0;
		sEncDisk[encNum].buffDiskEncInd = 0;
		for( uint8_t i = 0 ; i < ENC_DISK_POS_QTY ; i++ ){
			sEncCalib[encNum].encCount[i] = 0;
			sEncCalib[encNum].stepCount[i] = 0;
		}
		sEncCalib[encNum].encSeqFlag = false;
		StepMotor_GetStepPos( (tStpMtNum)encNum );

		//Prepara para mover
		Timing_Set( _e_Timing_Encoder_Break , TIMEOUT_CALIB_DINAM /*5000*/ );
		_encdisk_func_break_clear(encNum);
		StepMotor_GO( (tStpMtNum)encNum , _e_StMtDir_Right , 250 , STPMT_MT_SPEED_CAL , STPMT_MT_ACC_CAL , STPMT_MT_SPEED_INI_CAL );
		TaskManager_Step_Inc(ENCMTASK);
		encCounterTestCalib=0;
		break;

	case _e_encmt_task_cali_read_values:

		//Mapa de posi��es
			//pega quantidade de steps acumulados lá do módulo stepmotor
		sEncCalib[encNum].stepCount[sEncPosMap[encNum].index] += StepMotor_GetStepPos( (tStpMtNum)encNum );

		sEncCalib[encNum].ang[sEncPosMap[encNum].index] = sEncPosMap[encNum].ang;

		//Leitura resolução do encoder
		if( sEncDisk[encNum].diskEncChange == true && sEncPosMap[encNum].diskPosNow == _e_EcnDiscPos_Middle ){
			encCounterTestCalib++;
			sEncCalib[encNum].encCount[sEncPosMap[encNum].index]++;
			sEncDisk[encNum].diskEncChange = false;
		}
		//Sequencia do Encoder
		if( sEncCalib[encNum].encSeqFlag == false && sEncPosMap[encNum].index >=1 && sEncDisk[encNum].buffDiskEncInd >= 12 )
		{
			for( uint8_t i = 0 ; i < 4 ; i++ ){
				sEncCalib[encNum].encSeq[i] = sEncDisk[encNum].buffDiskEnc[i+7];
			}

		}
		//Aguarda Fim de Curso
		if( _encdisk_func_break_get(encNum) ){
			StepMotor_STOP((tStpMtNum)encNum);
			sEncCalib[encNum].status = _e_EncCalib_ErrIndex;
			TaskManager_Step_Set( ENCMTASK, _e_encmt_task_cali_error );
		}
		else if( Timing_Get_Bool( _e_Timing_Encoder_Break ) ){
			StepMotor_STOP((tStpMtNum)encNum);
			sEncCalib[encNum].status = _e_EncCalib_TimeOut2;
			TaskManager_Step_Set( ENCMTASK, _e_encmt_task_cali_error );
		}
		else if( sEncPosMap[encNum].index >= 2 )
		{
			StepMotor_STOP((tStpMtNum)encNum);
			TaskManager_Step_Inc(ENCMTASK);
		}

		break;

	case _e_encmt_task_cali_calc_values://CALIBRA��O FINALIZADA - Calcula resolu��o do encoder
		StepMotor_STOP((tStpMtNum)encNum);
		_enc_stop_mode(encNum);

		sEncCalib[encNum].encCount[2] = sEncCalib[encNum].encCount[0];

		for( uint8_t i = 0 ; i < ENC_DISK_POS_QTY ; i++ )
		{
			sEncPosMap[encNum].encCount[i] = sEncCalib[encNum].encCount[i];

			if( i == 2 ){
				sEncCalib[encNum].encRes[i] = ( ENC_DISK_PARAM_TOT - sEncCalib[encNum].ang[i] ) / sEncCalib[encNum].encCount[i];
			}
			else{
				sEncCalib[encNum].encRes[i] = ( sEncCalib[encNum].ang[i+1] - sEncCalib[encNum].ang[i] ) / sEncCalib[encNum].encCount[i];
			}
		}

		sEncCalib[encNum].encRes[0] = sEncCalib[encNum].encRes[1];
		sEncCalib[encNum].encRes[2] = sEncCalib[encNum].encRes[1];

		sEncCalib[encNum].encSeqFlag = true;

		TaskManager_Step_Inc(ENCMTASK);
		break;

	case _e_encmt_task_cali_test1:
		StepMotor_GO( (tStpMtNum)encNum , _e_StMtDir_Left , 250 , STPMT_MT_SPEED_CAL , STPMT_MT_ACC_CAL , STPMT_MT_SPEED_INI_CAL );
		TaskManager_Step_Inc(ENCMTASK);
		encCounterTest=0;
		break;

	case _e_encmt_task_cali_test2:
		if(sEncPosMap[encNum].diskPosNow == _e_EcnDiscPos_Middle)
		{
			if( sEncDisk[encNum].diskEncChange == true )
			{
				encCounterTest++;
				sEncDisk[encNum].diskEncChange = false;
			}
		}
		else if(sEncPosMap[encNum].diskPosNow == _e_EcnDiscPos_Left)
		{
			StepMotor_STOP((tStpMtNum)encNum);
			TaskManager_Step_Inc(ENCMTASK);
		}
		break;

	case _e_encmt_task_cali_test3:
		__NOP();
		break;

	case _e_encmt_task_cali_ok://OK
		sEncCalib[encNum].status = _e_EncCalib_Ok;
		sEncCtrl[encNum].command = _e_EncCmd_None;
//		sEncCalib[encNum].encSeqFlag = true;
		sEncMtask[encNum].TASK = _encmt_task_idle;
		break;

	case _e_encmt_task_cali_error://ERROR
		StepMotor_STOP((tStpMtNum)encNum);
		sEncCtrl[encNum].command = _e_EncCmd_None;
		sEncMtask[encNum].TASK = _encmt_task_idle;
		break;

	default:
		TaskManager_Step_Set( ENCMTASK, 0 );
		break;
	}
}

//MAIN ENC MOTOR TASK

void _EncMt_TASK(void)
{
	sEncMtask[_e_EncNum_Pri].TASK(_e_EncNum_Pri);
	sEncMtask[_e_EncNum_Sec].TASK(_e_EncNum_Sec);
}

/*** Exported Functions ***/

//Ext Int

void Encoder_Init(void)
{
	TaskManager_Clear( ENCDISK );
	TaskManager_Go( ENCDISK , _encdisk_task );

	TaskManager_Clear( ENC );
	TaskManager_Go( ENC , _enc_task );

	sEncMtask[_e_EncNum_Pri].TASK = _encmt_task_init;
	sEncMtask[_e_EncNum_Sec].TASK = _encmt_task_init;

	TaskManager_Clear( ENCSTPMT1 );
	TaskManager_Clear( ENCSTPMT2 );
	TaskManager_Clear( ENCSTPMTX);

	TaskManager_Go( ENCSTPMT1 , NULL );
	TaskManager_Go( ENCSTPMT2 , NULL );
	TaskManager_Go( ENCSTPMTX , _EncMt_TASK );

	StepMotor_Init();

	//memset(sEncDisk[_e_EncNum_Pri].buffDiskEnc,0,ENC_DISK_ENC_QTY);
	//memset(sEncDisk[_e_EncNum_Sec].buffDiskEnc,0,ENC_DISK_ENC_QTY);

	sEncDisk[_e_EncNum_Pri].buffDiskEncInd = 0;
	sEncDisk[_e_EncNum_Sec].buffDiskEncInd = 0;
	for(uint8_t i=0 ; i<ENC_DISK_ENC_QTY ; i++)
	{
		sEncDisk[_e_EncNum_Pri].buffDiskEnc[i] = 0;
		sEncDisk[_e_EncNum_Sec].buffDiskEnc[i] = 0;
	}
	__NOP();
}

//Ext System

//ultima versão antes do encoder bom
void Encoder_Refresh_IT( tEncNum encNum , uint8_t encVal )
{
	uint8_t posIndex=0,posIndexLast=0;
#if(ENC_TYPE == ENC_TYPE_INC)
	bool encFlagTest[4];
#endif
	bool encFlagClear = false;

	//Encoder State Memory
	//sEncDisk[encNum].lastDiskEnc = encVal;
	sEncDisk[encNum].buffDiskEnc[sEncDisk[encNum].buffDiskEncInd++] = encVal;
	sEncDisk[encNum].diskEncChange = true;
	if( sEncDisk[encNum].buffDiskEncInd >= ENC_DISK_ENC_QTY )sEncDisk[encNum].buffDiskEncInd = 0;

	//Encoder break engine
	sEncBreak[encNum].state[encVal] = true;

	sEncBreak[encNum].stateCount = 0;
	for( uint8_t i = 0 ; i < 4 ; i++ ){
		sEncBreak[encNum].stateCount += (uint8_t)sEncBreak[encNum].state[i];
	}

#if(ENC_TYPE == ENC_TYPE_DISC)

	if( sEncBreak[encNum].stateCount >= 4 ){
		_encdisk_func_break_clear(encNum);
	}
	else{
		sEncBreak[encNum].stepAcc += StepMotor_GetStepEnc( (tStpMtNum)encNum );
	}

#elif(ENC_TYPE == ENC_TYPE_INC)
	//NOVO MECANISMO DE BREAK
	encFlagClear = false;
	for( uint8_t i = 3 ; i < ENC_DISK_ENC_QTY ; i++ )
	{
		//limpa flags
		for( uint8_t a = 0 ; a < 4 ; a++ ){
			encFlagTest[a] = false;
		}
		//guarda ultimas 4 posições
		for( uint8_t a = 0 ; a < 4 ; a++ ){
			if(sEncDisk[encNum].buffDiskEnc[i-a] <4 ){
				encFlagTest[ sEncDisk[encNum].buffDiskEnc[i-a] ] = true;
			}
		}
		//verifica se há repetição
		for( uint8_t a = 0 ; a < 4 ; a++ ){
			if( encFlagTest[a] == false ){
				encFlagClear = true;
			}
		}
	}
#endif

	if( encFlagClear == true ){
		sEncBreak[encNum].stepAcc += StepMotor_GetStepEnc( (tStpMtNum)encNum );
	}
	else{
		_encdisk_func_break_clear(encNum);
	}

	//Encoder Map
	if( sEncCalib[encNum].encSeqFlag == true )
	{
		//Salva os indices de posi��o
		for( uint8_t i = 0 ; i < 4 ; i++ )
		{
			if( encVal == sEncCalib[encNum].encSeq[i] ){
				posIndex = i;
			}
			else if( sEncDisk[encNum].lastDiskEnc == sEncCalib[encNum].encSeq[i] ){
				posIndexLast = i;
			}
		}
		//Verifica dire��o
		if( posIndex == posIndexLast ){
			sEncPosMap[encNum].dir = _e_EncDir_Unknown;
		}
		else
		{
			if( posIndex == posIndexLast+1 )
				sEncPosMap[encNum].dir = _e_EncDir_Right;
			else if( posIndex == posIndexLast-1 )
				sEncPosMap[encNum].dir = _e_EncDir_Left;
			else
			{
				if( posIndex == 0 && posIndexLast == 3 ){
					sEncPosMap[encNum].dir = _e_EncDir_Right;
				}
				else if( posIndex == 3 && posIndexLast == 0 ){
					sEncPosMap[encNum].dir = _e_EncDir_Left;
				}
				else{
					sEncPosMap[encNum].dir = _e_EncDir_Unknown;
				}
			}
		}
		//Faz incremento ou dercemento de posi��o
		if( sEncPosMap[encNum].dir == _e_EncDir_Left /*||
			( 	StepMotor_GetDir((tStpMtNum)encNum) == _e_StMtDir_Left &&
				StepMotor_GetSts((tStpMtNum)encNum) != _e_StpMtSts_Stop )*/
		  )
		{
			if( sEncPosMap[encNum].encCount[sEncPosMap[encNum].index] > 0 ){
				sEncPosMap[encNum].encCount[sEncPosMap[encNum].index]--;
			}
			sEncPosMap[encNum].ang -= sEncCalib[encNum].encRes[sEncPosMap[encNum].index];
		}
		else if( sEncPosMap[encNum].dir == _e_EncDir_Right /*||
				( 	StepMotor_GetDir((tStpMtNum)encNum) == _e_StMtDir_Right &&
					StepMotor_GetSts((tStpMtNum)encNum) != _e_StpMtSts_Stop )*/
			  )
		{
			if( sEncPosMap[encNum].encCount[sEncPosMap[encNum].index] < sEncCalib[encNum].encCount[sEncPosMap[encNum].index] ){
				sEncPosMap[encNum].encCount[sEncPosMap[encNum].index]++;
			}
			sEncPosMap[encNum].ang += sEncCalib[encNum].encRes[sEncPosMap[encNum].index];
		}


	}
	sEncDisk[encNum].lastDiskEnc = encVal;
}

void Encoder_Position_IT( tEncNum encNum , uint8_t posVal )
{
	_encdisk_func_inc_buffindex(encNum);
	sEncDisk[encNum].buffDiskPos[sEncDisk[encNum].buffIndex] = posVal;
	sEncDisk[encNum].diskPosChange = true;
}

//Ext User

tEncCalib Encoder_Cali_Get( tEncNum encNum )
{
	return sEncCalib[encNum].status;
}

void Encoder_Cali_Set( tEncNum encNum , tEncStopMode stopMode )
{
	sEncCtrl[encNum].stopMode = stopMode;
	sEncCalib[encNum].status = _e_EncCalib_Running;
	sEncCtrl[encNum].status = _e_EncCalib_Running;
	sEncCtrl[encNum].command = _e_EncCmd_Cali;
}



void Encoder_Move_Set( tEncNum encNum , uint8_t ang , tEncStopMode stopMode )
{

	//if( sEncCtrl[encNum].angPercentUser == ang )return;
	if( sEncPosMap[encNum].angPercent < (ang-ENC_MOVE_ANG_TOL) ||
		sEncPosMap[encNum].angPercent > (ang+ENC_MOVE_ANG_TOL)	)
	{
		sEncCtrl[encNum].stopMode = stopMode;
		sEncCtrl[encNum].angPercentUser = ang;
		//sEncCtrl[encNum].angUser = (sEncCtrl[encNum].angPercentUser * (sEncCalib[encNum].ang[6]-sEncCalib[encNum].ang[1]) / 100) + sEncCalib[encNum].ang[1];
		sEncCtrl[encNum].angUser = (sEncCtrl[encNum].angPercentUser * (sEncCalib[encNum].angMax-sEncCalib[encNum].angMin) / 100) + sEncCalib[encNum].angMin;
		sEncCtrl[encNum].command = _e_EncCmd_Move;
		sEncCtrl[encNum].status = _e_EncSts_Move;
		_encdisk_func_break_clear(encNum);
	}
}

float Encoder_Move_Get( tEncNum encNum )
{
	if( sEncCalib[encNum].status != _e_EncCalib_Ok )
		return 0xFF;
	else
		return sEncPosMap[encNum].angPercent;
}

void Encoder_Move_ToStop( tEncNum encNum , tEncDir encDir )
{
	sEncCtrl[encNum].command = _e_EncCmd_MoveToStop;
	sEncCtrl[encNum].direction = encDir;
	sEncCtrl[encNum].status = _e_EncSts_Move;
}

void Encoder_Stop( tEncNum encNum , tEncStopMode stopMode )
{
	sEncCtrl[encNum].stopMode = stopMode;
	sEncCtrl[encNum].command = _e_EncCmd_Stop;
}

tEncSts	Encoder_Stat( tEncNum encNum )
{
	return sEncCtrl[encNum].status;
}

