/**
  ******************************************************************************
  * @file    position.h
  * @author  Arthur Murata
  * @version V0.1
  * @date    19/11/2021
  * @brief   Position Header
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) TSI.
  *
  ******************************************************************************
  */

#ifndef POSITION_H_
#define POSITION_H_

/*** ** * Includes * ** ***/

#include "main.h"

/*** ** * Defines * ** ***/



#define POSITION_PC_MAX_ANG		48//45

#define POSITION_MAX_ANG		100


/*** ** * Types * ** ***/

typedef enum{
	_e_PosMtNum_Pri,
	_e_PosMtNum_Sec,
	_e_PosMtNum_Qty,
}tPosMtNum;

typedef enum{
	_e_PosMtCfg_Single,
	_e_PosMtCfg_Dual,
	_e_PosMtCfg_Unknown,
}tPosMtCfg;

typedef enum{
	_e_PosModCfg_PC,
	_e_PosModCfg_NA,
	_e_PosModCfg_NF,
	_e_PosModCfg_Unknown,
}tPosModCfg;

typedef enum{
	_e_PosRet_Ok,
	_e_PosRet_MotorCfgNotExist,
	_e_PosRet_MotorNotConfig,
	_e_PosRet_ModeNotExist,
	_e_PosRet_ModeWrong,
	_e_PosRet_ValueOutOfLimit,
	_e_PosRet_FlagAlreadyClosed,
}tPosRet;

typedef enum{
	_e_PosSts_WaitCfgMotor,
	_e_PosSts_CalibRunning,
	_e_PosSts_CalibERROR_WaitCfgMotor,
	_e_PosSts_WaitCfgMode,
	_e_PosSts_ModePcRunning,
	_e_PosSts_ModeNaRunning,
	_e_PosSts_ModeNfRunning,
}tPosSts;

typedef enum{
	_e_PosModeNfSts_Free,
	_e_PosModeNfSts_Block,
}tPosModeNaSts;

typedef enum{
	_e_PosModeNfSts_Close,
	_e_PosModeNfSts_Entrance,
	_e_PosModeNfSts_Exit,
}tPosModeNfSts;

/*** ** * Funcs Ext * ** ***/

void Position_Init(void);

//tPosRet = _e_PosRet_MtCfgNotExist | _e_PosRet_Ok
tPosRet 	Position_Config_Motor( tPosMtCfg motorQty );

//tPosMtCfg
tPosMtCfg 	Position_Config_Motor_Get(void);

//tPosRet = _e_PosRet_MotorNotConfig | _e_PosRet_ModeNotExist | _e_PosRet_Ok
tPosRet 	Position_Config_Mode( tPosModCfg modeConfig );

//tPosModCfg
tPosModCfg	Position_Config_Mode_Get(void);

//tPosSts
tPosSts		Position_Status(void);

//Mode PC
tPosRet		Position_PC_Set(uint8_t valMt1 , uint8_t ValMt2 );

//Mode NA
tPosRet		Position_NA_Set(tPosModeNaSts stsEntrance , tPosModeNaSts stsExit );
tPosRet		Position_NA_Entrance_Set(tPosModeNaSts stsEntrance );
tPosRet		Position_NA_Exit_Set(tPosModeNaSts stsExit );

//Mode NF
tPosRet		Position_NF_Set(tPosModeNfSts sts);

//END

#endif /* POSITION_H_ */
