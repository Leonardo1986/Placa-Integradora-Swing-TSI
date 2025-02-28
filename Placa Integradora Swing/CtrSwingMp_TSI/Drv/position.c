/**
  ******************************************************************************
  * @file    position.c
  * @author  Arthur Murata
  * @version V0.1
  * @date    19/11/2021
  * @brief   Position Source
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) TSI.
  *
  ******************************************************************************
  */

/*** ** * Includes * ** ***/
#include "position.h"
#include "encoder.h"
#include "task_manager.h"
#include "timing.h"
#include "sw_config.h"

/*** ** * Defines * ** ***/

#define TMPOS 		_e_TaskManager_Position
#define TMPOSMT1	_e_TaskManager_Position1
#define TMPOSMT2	_e_TaskManager_Position2
#define TMPOSMTX	_e_TaskManager_PositionX

/*** ** * Types * ** ***/

typedef void (*_PosMtTask_t)(tPosMtNum encNum);

/*** ** * Variables * ** ***/

struct{
	_PosMtTask_t	TASK;
}sPosMtTask[_e_PosMtNum_Qty];

//struct{
//	tEncCalib mtCalib;
//}sPosMtSts[_e_PosMtNum_Qty];

struct{
	bool		cfgMotorCalibFlag;
	bool		cgfModeChangeFlag;
	tPosMtCfg 	cfgMotor;
	tPosModCfg 	cfgMode;
}sPosConf;

struct{
	tPosRet 	status;
	uint8_t		calibAttempts;
}sPosSts;

struct{
	float	valSys[_e_PosMtNum_Qty];
	float	valSysMin[_e_PosMtNum_Qty];
	float	valSysMax[_e_PosMtNum_Qty];
	float	valUser[_e_PosMtNum_Qty];
	float	valUserLast[_e_PosMtNum_Qty];
}sPosRun;

/*** ** * Local Funcs * ** ***/

/*** ** * TASK MT * ** ***/

typedef enum
{
	_e_pos_mt_task_init,
	_e_pos_mt_task_run,
}t_pos_mt_task;

static void _pos_mt_task_init( tPosMtNum posNum );
static void _pos_mt_task_run( tPosMtNum posNum );

static void _pos_mt_task_init( tPosMtNum posNum )
{
	tTaskManager TMPOSMT = ( TMPOSMT1 + (tTaskManager)posNum );
	TaskManager_Set( TMPOSMT , _e_pos_mt_task_init );
	if( TaskManager_First_Get( TMPOSMT ) == false ){
		TaskManager_First_Set( TMPOSMT );
		TaskManager_Step_Set( TMPOSMT, 0 );
	}
	switch( TaskManager_Step_Get( TMPOSMT ) )
	{
	default:
		TaskManager_Step_Set( TMPOSMT, 0 );
		break;
	}
}

static void _pos_mt_task_run( tPosMtNum posNum )
{
	tTaskManager TMPOSMT = ( TMPOSMT1 + (tTaskManager)posNum );
	TaskManager_Set( TMPOSMT , _e_pos_mt_task_run );
	if( TaskManager_First_Get( TMPOSMT ) == false ){
		TaskManager_First_Set( TMPOSMT );
		TaskManager_Step_Set( TMPOSMT, 0 );

		if(sPosRun.valUser[posNum] < 20 )sPosRun.valUser[posNum] = 100;

		//Timing_Set( _e_Timing_Position_CorrectionTime_Mt1+posNum , 1500 );
	}

	sPosRun.valSys[posNum] = Encoder_Move_Get( (tEncNum) posNum );

	if( sPosRun.valSys[posNum] >= POSITION_ANGPER_TOL ){
		sPosRun.valSysMin[posNum] = sPosRun.valSys[posNum] - POSITION_ANGPER_TOL;
	}
	else{
		sPosRun.valSysMin[posNum] = 0;
	}

	if(sPosRun.valSys[posNum] < (POSITION_MAX_ANG-POSITION_ANGPER_TOL) ){
		sPosRun.valSysMax[posNum] = sPosRun.valSys[posNum] + POSITION_ANGPER_TOL;
	}
	else{
		sPosRun.valSysMax[posNum] = POSITION_MAX_ANG;
	}

	switch( TaskManager_Step_Get( TMPOSMT ) )
	{
	case 0:
		if( sPosRun.valUser[posNum] != sPosRun.valUserLast[posNum])
		{
			sPosRun.valUserLast[posNum] = sPosRun.valUser[posNum];
			TaskManager_Step_Inc(TMPOSMT);
		}
		else
		{
			if( Encoder_Stat((tEncNum)posNum) == _e_EncSts_Idle && Timing_Get_Bool( _e_Timing_Position_CorrectionTime_Mt1+posNum ))
			{
				if( sPosRun.valUser[posNum] < sPosRun.valSysMin[posNum] ||
					sPosRun.valUser[posNum] > sPosRun.valSysMax[posNum]	)
				{
					TaskManager_Step_Inc(TMPOSMT);
					Timing_Set( _e_Timing_Position_CorrectionTime_Mt1+posNum , POSITION_CORRECT_TIME );
				}
			}
		}
		break;

	case 1:
		Encoder_Move_Set( (tEncNum)posNum , sPosRun.valUser[posNum] , _e_EncStopMode_On );
		TaskManager_Step_Set( TMPOSMT, 0 );
		break;

	default:
		TaskManager_Step_Set( TMPOSMT, 0 );
		break;
	}
}

//MAIN ENC MOTOR TASK

void _Pos_TASK(void)
{
	sPosMtTask[_e_PosMtNum_Pri].TASK(_e_PosMtNum_Pri);
	if( sPosConf.cfgMotor == _e_PosMtCfg_Dual ){
		sPosMtTask[_e_PosMtNum_Sec].TASK(_e_PosMtNum_Sec);
	}
}

/*** ** * TASK CTRL * ** ***/

typedef enum{
	_e_pos_task_init,
	_e_pos_task_mode,
	_e_pos_task_mode_pc,
	_e_pos_task_mode_nf,
	_e_pos_task_mode_na,
}t_pos_task;

static void _pos_task_init(void);
static void _pos_task_mode(void);
static void _pos_task_mode_pc(void);
static void _pos_task_mode_nf(void);
static void _pos_task_mode_na(void);


typedef enum{
	_e_pos_task_init_wait_cmd,
	_e_pos_task_init_calib_mt_dual_align,
	_e_pos_task_init_calib_mt_dual_align_wait,
	_e_pos_task_init_calib_mt1_start,
	_e_pos_task_init_calib_mt1_wait,
	_e_pos_task_init_calib_mt1_check,
	_e_pos_task_init_calib_mt2_start,
	_e_pos_task_init_calib_mt2_wait,
	_e_pos_task_init_calib_mt2_check,
	_e_pos_task_init_calib_OK,
	_e_pos_task_init_calib_ERROR,
}t_pos_task_init;

static void _pos_task_init(void)
{
	TaskManager_Set( TMPOS , _e_pos_task_init );
	if( TaskManager_First_Get( TMPOS ) == false ){
		TaskManager_First_Set( TMPOS );
		TaskManager_Step_Set( TMPOS, _e_pos_task_init_wait_cmd );
		sPosSts.calibAttempts = 0;
		sPosSts.status = _e_PosSts_WaitCfgMotor;
	}

	switch( TaskManager_Step_Get( TMPOS ) )
	{
	case _e_pos_task_init_wait_cmd:
		if( sPosConf.cfgMotor != _e_PosMtCfg_Unknown )
		{
			if( sPosConf.cfgMotor == _e_PosMtCfg_Single ){
				TaskManager_Step_Set( TMPOS , _e_pos_task_init_calib_mt1_start );
			}
			else{
				TaskManager_Step_Set( TMPOS ,_e_pos_task_init_calib_mt_dual_align );
			}
			sPosSts.status = _e_PosSts_CalibRunning;
		}
		break;

	case _e_pos_task_init_calib_mt_dual_align:
//		Encoder_Move_ToStop( _e_EncNum_Pri , _e_EncDir_Left );
//		Encoder_Move_ToStop( _e_EncNum_Sec , _e_EncDir_Right );
		TaskManager_Step_Inc( TMPOS );
		break;

	case _e_pos_task_init_calib_mt_dual_align_wait:
		if( Encoder_Stat( _e_EncNum_Pri ) == _e_EncSts_Idle &&
			Encoder_Stat( _e_EncNum_Sec ) == _e_EncSts_Idle	)
		{
			TaskManager_Step_Set( TMPOS , _e_pos_task_init_calib_mt1_start );
		}
		break;

	case _e_pos_task_init_calib_mt1_start:
		Encoder_Cali_Set( _e_EncNum_Pri , _e_EncStopMode_On );
		TaskManager_Step_Inc( TMPOS );
		break;

	case _e_pos_task_init_calib_mt1_wait:
		if( Encoder_Cali_Get(_e_EncNum_Pri) != _e_EncCalib_Running )
		{
			if( sPosConf.cfgMotor == _e_PosMtCfg_Single ){
				TaskManager_Step_Inc( TMPOS );
			}
			else{
				TaskManager_Step_Set( TMPOS, _e_pos_task_init_calib_mt2_start );
			}
		}
		break;

	case _e_pos_task_init_calib_mt1_check:
		if( Encoder_Cali_Get(_e_EncNum_Pri) == _e_EncCalib_Ok ){
			TaskManager_Step_Set( TMPOS, _e_pos_task_init_calib_OK );
		}
		else{
			sPosSts.calibAttempts++;
			if( sPosSts.calibAttempts >= POS_CALIB_ATTEMPTS){
				TaskManager_Step_Set( TMPOS, _e_pos_task_init_calib_ERROR );
			}
			else{
				TaskManager_Step_Set( TMPOS, _e_pos_task_init_calib_mt1_start );
			}
		}
		break;

	case _e_pos_task_init_calib_mt2_start:
		Encoder_Cali_Set( _e_EncNum_Sec , _e_EncStopMode_On );
		TaskManager_Step_Inc( TMPOS );
		break;

	case _e_pos_task_init_calib_mt2_wait:
		if( Encoder_Cali_Get(_e_EncNum_Sec) != _e_EncCalib_Running ){
			TaskManager_Step_Set( TMPOS, _e_pos_task_init_calib_mt2_check );
		}
		break;

	case _e_pos_task_init_calib_mt2_check:
		if( Encoder_Cali_Get(_e_EncNum_Pri) == _e_EncCalib_Ok &&
			Encoder_Cali_Get(_e_EncNum_Sec) == _e_EncCalib_Ok	)
		{
			TaskManager_Step_Set( TMPOS, _e_pos_task_init_calib_OK );
		}
		else{
			sPosSts.calibAttempts++;
			if( sPosSts.calibAttempts >= POS_CALIB_ATTEMPTS){
				TaskManager_Step_Set( TMPOS, _e_pos_task_init_calib_ERROR );
			}
			else{
				//TaskManager_Step_Set( TMPOS, _e_pos_task_init_calib_mt1_start );
				TaskManager_Step_Set( TMPOS, _e_pos_task_init_wait_cmd );
			}
		}
		break;

	case _e_pos_task_init_calib_OK:
		TaskManager_Go( TMPOS  , _pos_task_mode );
		break;

	case _e_pos_task_init_calib_ERROR:
		sPosConf.cfgMotor = _e_PosMtCfg_Unknown;
		sPosSts.status = _e_PosSts_CalibERROR_WaitCfgMotor;
		TaskManager_Step_Set( TMPOS, _e_pos_task_init_wait_cmd );
		break;

	default:
		TaskManager_Step_Set( TMPOS, _e_pos_task_init_wait_cmd );
		break;
	}
}

static void _pos_task_mode(void)
{
	TaskManager_Set( TMPOS , _e_pos_task_mode );
	if( TaskManager_First_Get( TMPOS ) == false ){
		TaskManager_First_Set( TMPOS );
		TaskManager_Step_Set( TMPOS, 0 );
		sPosMtTask[_e_PosMtNum_Pri].TASK = _pos_mt_task_init;
		sPosMtTask[_e_PosMtNum_Sec].TASK = _pos_mt_task_init;
	}

	switch( TaskManager_Step_Get( TMPOS ) )
	{
	case 0:
		if( sPosConf.cfgMode != _e_PosModCfg_Unknown ){
			TaskManager_Step_Inc( TMPOS );
		}
		else{
			sPosSts.status = _e_PosSts_WaitCfgMode;
		}
		break;

	case 1:
		if( sPosConf.cfgMode == _e_PosModCfg_PC )
			TaskManager_Go( TMPOS  , _pos_task_mode_pc );
		else if( sPosConf.cfgMode == _e_PosModCfg_NF )
			TaskManager_Go( TMPOS  , _pos_task_mode_nf );
		else if( sPosConf.cfgMode == _e_PosModCfg_NA )
			TaskManager_Go( TMPOS  , _pos_task_mode_na );
		else
			TaskManager_Step_Set( TMPOS, 0 );
		break;

	default:
		TaskManager_Step_Set( TMPOS, 0 );
		break;
	}
}

static void _pos_task_mode_pc(void)
{
	TaskManager_Set( TMPOS , _e_pos_task_mode_pc );
	if( TaskManager_First_Get( TMPOS ) == false ){
		TaskManager_First_Set( TMPOS );
		TaskManager_Step_Set( TMPOS, 0 );
		sPosSts.status = _e_PosSts_ModePcRunning;
	}

	switch( TaskManager_Step_Get( TMPOS ) )
	{
	case 0:
		sPosRun.valSys[_e_PosMtNum_Pri] = Encoder_Move_Get( _e_EncNum_Pri );
		sPosRun.valSys[_e_PosMtNum_Sec] = Encoder_Move_Get( _e_EncNum_Sec );
		TaskManager_Step_Inc(TMPOS);
		break;
	case 1:
		sPosMtTask[_e_PosMtNum_Pri].TASK = _pos_mt_task_run;
		sPosMtTask[_e_PosMtNum_Sec].TASK = _pos_mt_task_run;
		TaskManager_Step_Inc(TMPOS);
		break;
	case 2:
		break;

	default:
		TaskManager_Step_Set( TMPOS, 0 );
		break;
	}
}

static void _pos_task_mode_nf(void)
{
	TaskManager_Set( TMPOS , _e_pos_task_mode_nf );
	if( TaskManager_First_Get( TMPOS ) == false ){
		TaskManager_First_Set( TMPOS );
		TaskManager_Step_Set( TMPOS, 0 );
		sPosSts.status = _e_PosSts_ModeNfRunning;
	}

	switch( TaskManager_Step_Get( TMPOS ) )
	{
	case 0:
		sPosRun.valSys[_e_PosMtNum_Pri] = Encoder_Move_Get( _e_EncNum_Pri );
		sPosRun.valSys[_e_PosMtNum_Sec] = Encoder_Move_Get( _e_EncNum_Sec );
		TaskManager_Step_Inc(TMPOS);
		break;
	case 1:
		sPosMtTask[_e_PosMtNum_Pri].TASK = _pos_mt_task_run;
		sPosMtTask[_e_PosMtNum_Sec].TASK = _pos_mt_task_run;
		TaskManager_Step_Inc(TMPOS);
		break;
	case 2:
		break;
	default:
		TaskManager_Step_Set( TMPOS, 0 );
		break;
	}
}

static void _pos_task_mode_na(void)
{
	TaskManager_Set( TMPOS , _e_pos_task_mode_na );
	if( TaskManager_First_Get( TMPOS ) == false ){
		TaskManager_First_Set( TMPOS );
		TaskManager_Step_Set( TMPOS, 0 );
		sPosSts.status = _e_PosSts_ModeNaRunning;
	}

	switch( TaskManager_Step_Get( TMPOS ) )
	{
	case 0:
		sPosRun.valSys[_e_PosMtNum_Pri] = Encoder_Move_Get( _e_EncNum_Pri );
		sPosRun.valSys[_e_PosMtNum_Sec] = Encoder_Move_Get( _e_EncNum_Sec );
		TaskManager_Step_Inc(TMPOS);
		break;
	case 1:
		sPosMtTask[_e_PosMtNum_Pri].TASK = _pos_mt_task_run;
		sPosMtTask[_e_PosMtNum_Sec].TASK = _pos_mt_task_run;
		TaskManager_Step_Inc(TMPOS);
		break;
	case 2:
		break;
	default:
		TaskManager_Step_Set( TMPOS, 0 );
		break;
	}
}


/*** ** * Exported Functions * ** ***/

void Position_Init(void)
{
	TaskManager_Clear( TMPOSMT1 );
	TaskManager_Clear( TMPOSMT2 );
	TaskManager_Clear( TMPOSMTX );
	TaskManager_Clear( TMPOS  );

	TaskManager_Go( TMPOSMT1 , NULL );
	TaskManager_Go( TMPOSMT2 , NULL );
	TaskManager_Go( TMPOSMTX , _Pos_TASK );
	TaskManager_Go( TMPOS  , _pos_task_init );

	sPosMtTask[_e_PosMtNum_Pri].TASK = _pos_mt_task_init;
	sPosMtTask[_e_PosMtNum_Sec].TASK = _pos_mt_task_init;

	Encoder_Init();

	sPosConf.cfgMotorCalibFlag = false;
	sPosConf.cfgMotor = _e_PosMtCfg_Unknown;
	sPosConf.cfgMode = _e_PosModCfg_Unknown;
}


tPosRet Position_Config_Motor( tPosMtCfg motorQty )
{
	if( motorQty < _e_PosMtCfg_Unknown ){
		sPosConf.cfgMotorCalibFlag = true;
		sPosConf.cfgMotor = motorQty;
		TaskManager_Go( TMPOS  , _pos_task_init );
	}
	else{
		return _e_PosRet_MotorCfgNotExist;
	}
	return _e_PosRet_Ok;
}

tPosMtCfg Position_Config_Motor_Get(void)
{
	return sPosConf.cfgMotor;
}

tPosModCfg Position_Config_Mode_Get(void)
{
	return sPosConf.cfgMode;
}

tPosRet Position_Config_Mode( tPosModCfg modeConfig )
{
	if( modeConfig < _e_PosModCfg_Unknown ){
		sPosConf.cgfModeChangeFlag = true;
		sPosConf.cfgMode = modeConfig;

		if( sPosConf.cfgMotor == _e_PosMtCfg_Single ){
			if( Encoder_Cali_Get(_e_EncNum_Pri) == _e_EncCalib_Ok ){
				TaskManager_Go( TMPOS  , _pos_task_mode );
			}
			else{
				TaskManager_Go( TMPOS  , _pos_task_init );
			}
		}
		else if( sPosConf.cfgMotor == _e_PosMtCfg_Dual ){
			if( Encoder_Cali_Get(_e_EncNum_Pri) == _e_EncCalib_Ok &&
				Encoder_Cali_Get(_e_EncNum_Sec) == _e_EncCalib_Ok	){
				TaskManager_Go( TMPOS  , _pos_task_mode );
			}
			else{
				TaskManager_Go( TMPOS  , _pos_task_init );
			}
		}
		else{
			return _e_PosRet_MotorNotConfig;
		}
	}
	else{
		return _e_PosRet_ModeNotExist;
	}
	return _e_PosRet_Ok;
}

tPosSts	Position_Status(void)
{
	return sPosSts.status;
}


tPosRet	Position_PC_Set(uint8_t valMt1 , uint8_t valMt2 )
{
	if( sPosConf.cfgMode != _e_PosModCfg_PC ){
		sPosConf.cfgMode = _e_PosModCfg_PC;
		sPosRun.valUser[_e_PosMtNum_Pri] = 100 - ( valMt1 * POSITION_PC_MAX_ANG / 100 );
		sPosRun.valUser[_e_PosMtNum_Sec] = 100 - ( valMt2 * POSITION_PC_MAX_ANG / 100 );
		return _e_PosRet_ModeWrong;
	}

	if( valMt1 > 100 )return _e_PosRet_ValueOutOfLimit;
	if( valMt2 > 100 )return _e_PosRet_ValueOutOfLimit;

	sPosRun.valUser[_e_PosMtNum_Pri] = 100 - ( valMt1 * POSITION_PC_MAX_ANG / 100 );
	sPosRun.valUser[_e_PosMtNum_Sec] = 100 - ( valMt2 * POSITION_PC_MAX_ANG / 100 );

	//Timing_Set( _e_Timing_Position_CorrectionTime_Mt1+_e_PosMtNum_Pri , POSITION_CORRECT_TIME_M );
	//Timing_Set( _e_Timing_Position_CorrectionTime_Mt1+_e_PosMtNum_Sec , POSITION_CORRECT_TIME_M );

	return _e_PosRet_Ok;
}

tPosRet	Position_NA_Set(tPosModeNaSts stsEntrance , tPosModeNaSts stsExit )
{
	uint8_t valMt1,valMt2;

	if( sPosConf.cfgMode != _e_PosModCfg_NA ){
		return _e_PosRet_ModeWrong;
	}

	if( stsEntrance == _e_PosModeNfSts_Free )
		valMt1 = 0;
	else
		valMt1 = 100;

	if( stsExit == _e_PosModeNfSts_Free )
		valMt2 = 0;
	else
		valMt2 = 100;

	sPosRun.valUser[_e_PosMtNum_Pri] = 100 - ( valMt1 * POSITION_PC_MAX_ANG / 100 );
	sPosRun.valUser[_e_PosMtNum_Sec] = 100 - ( valMt2 * POSITION_PC_MAX_ANG / 100 );

	//Timing_Set( _e_Timing_Position_CorrectionTime_Mt1+_e_PosMtNum_Pri , POSITION_CORRECT_TIME_M );
	//Timing_Set( _e_Timing_Position_CorrectionTime_Mt1+_e_PosMtNum_Sec , POSITION_CORRECT_TIME_M );

	return _e_PosRet_Ok;
}

//*******************************************************************************
tPosRet	Position_NA_Entrance_Set(tPosModeNaSts stsEntrance )
{
	uint8_t valMt1;

	if( sPosConf.cfgMode != _e_PosModCfg_NA ){
		return _e_PosRet_ModeWrong;
	}

	if( Encoder_Stat( _e_EncNum_Sec ) != _e_EncSts_Idle	) {
		return _e_PosRet_FlagAlreadyClosed;
	}

	if( stsEntrance == _e_PosModeNfSts_Free )
		valMt1 = 0;
	else
		valMt1 = 100;

	sPosRun.valUser[_e_PosMtNum_Pri] = 100 - ( valMt1 * POSITION_PC_MAX_ANG / 100 );

	//Timing_Set( _e_Timing_Position_CorrectionTime_Mt1+_e_PosMtNum_Pri , POSITION_CORRECT_TIME_M );
	//Timing_Set( _e_Timing_Position_CorrectionTime_Mt1+_e_PosMtNum_Sec , POSITION_CORRECT_TIME_M );

	return _e_PosRet_Ok;
}

tPosRet	Position_NA_Exit_Set(tPosModeNaSts stsExit )
{
	uint8_t valMt2;

	if( sPosConf.cfgMode != _e_PosModCfg_NA ){
		return _e_PosRet_ModeWrong;
	}

	if( Encoder_Stat( _e_EncNum_Pri ) != _e_EncSts_Idle	) {
		return _e_PosRet_FlagAlreadyClosed;
	}

	if( stsExit == _e_PosModeNfSts_Free )
		valMt2 = 0;
	else
		valMt2 = 100;

	sPosRun.valUser[_e_PosMtNum_Sec] = 100 - ( valMt2 * POSITION_PC_MAX_ANG / 100 );

	//Timing_Set( _e_Timing_Position_CorrectionTime_Mt1+_e_PosMtNum_Pri , POSITION_CORRECT_TIME_M );
	//Timing_Set( _e_Timing_Position_CorrectionTime_Mt1+_e_PosMtNum_Sec , POSITION_CORRECT_TIME_M );

	return _e_PosRet_Ok;
}

//*******************************************************************************

tPosRet	Position_NF_Set(tPosModeNfSts sts)
{
	if( sPosConf.cfgMode != _e_PosModCfg_NF )
	{
		return _e_PosRet_ModeWrong;
	}

	switch( (uint8_t)sts)
	{
		case _e_PosModeNfSts_Close:
			sPosRun.valUser[_e_PosMtNum_Pri] = 50;
			sPosRun.valUser[_e_PosMtNum_Sec] = 50;
		break;

		case _e_PosModeNfSts_Entrance:
			sPosRun.valUser[_e_PosMtNum_Pri] = 100;
			sPosRun.valUser[_e_PosMtNum_Sec] = 0;
		break;

		case _e_PosModeNfSts_Exit:
			sPosRun.valUser[_e_PosMtNum_Pri] = 0;
			sPosRun.valUser[_e_PosMtNum_Sec] = 100;
		break;

		default:
			return _e_PosRet_ModeWrong;
		break;
	}

	//Timing_Set( _e_Timing_Position_CorrectionTime_Mt1+_e_PosMtNum_Pri , POSITION_CORRECT_TIME_M );
	//Timing_Set( _e_Timing_Position_CorrectionTime_Mt1+_e_PosMtNum_Sec , POSITION_CORRECT_TIME_M );
	return _e_PosRet_Ok;
}
//END OF FILE
