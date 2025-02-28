/**
  ******************************************************************************
  * @file    outputs.h
  * @author  Arthur Murata
  * @version V0.1
  * @date    26/08/2020
  * @brief   Outputs Header
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) TSI.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __outputs_H
#define __outputs_H
#ifdef __cplusplus
 extern "C" {
#endif

/*** ** * Includes * ** ***/
#include "main.h"
#include "gpio.h"

/*** ** * Defines * ** ***/

#define ENABLE_MOTORS()				_pin_set_l(GPIO_MT_ENN_Port, GPIO_MT_ENN_Pin);
#define DISABLE_MOTORS()			_pin_set_h(GPIO_MT_ENN_Port, GPIO_MT_ENN_Pin);


#define _pin_set_en_h_MotorOn		_pin_set_l(GPIO_MT_ENN_Port, GPIO_MT_ENN_Pin)
#define _pin_set_en_l_MotorOff		_pin_set_h(GPIO_MT_ENN_Port, GPIO_MT_ENN_Pin)


#define SWING_ENABLE_CONFIG		    _pin_set_h(GPIO_HMI_Sel_Disp_Key_Port,GPIO_HMI_Sel_Disp_Key_Pin)
#define SWING_DISABLE_CONFIG	    _pin_set_l(GPIO_HMI_Sel_Disp_Key_Port,GPIO_HMI_Sel_Disp_Key_Pin)


/*** ** * Types * ** ***/

//Buzzer

typedef enum{
	_e_OutBuzz_Off			= 1,
	_e_OutBuzz_On			= 2,
	_e_OutBuzz_Beep			= 3,
	_e_OutBuzz_Siren		= 4,
}tOutBuzz;

//Backlight display

typedef enum{
	_e_OutDled_Off			= 0,
	_e_OutDled_On			= 1,
	_e_OutDled_Blink		= 2,
}tOutDled;

//Pictogramas laterais rgb

typedef enum{
	_e_OutPic_In			= 0,
	_e_OutPic_Out			= 1,
	_e_OutPic_Both			= 2,
}tOutPic;

typedef enum{
	_e_OutPicMode_Off 		= 0,
	_e_OutPicMode_Static 	= 1,
	_e_OutPicMode_Blink		= 2,
}tOutPicMode;

typedef enum{
	_e_OutPicSpeed_Fast		= 0,
	_e_OutPicSpeed_Aver		= 3,
	_e_OutPicSpeed_Slow		= 5,
}tOutPicSpeed;

typedef enum{
	_e_OutPicColor_Red		= 0,
	_e_OutPicColor_Green	= 1,
	_e_OutPicColor_Blue		= 2,
	_e_OutPicColor_Yellow	= 3,
	_e_OutPicColor_Pink		= 4,
	_e_OutPicColor_Lblue	= 5,
	_e_OutPicColor_TSIgr	= 6,
	_e_OutPicColor_White	= 7,
}tOutPicColor;

//Motores

typedef enum{
	_e_OutMt_Pri			= 0,
	_e_OutMt_Sec			= 1,
	_e_OutMt_Both			= 2,
}tOutMt;

typedef enum{
	_e_OutMtAct_Stop		= 0,
	_e_OutMtAct_In			= 1,
	_e_OutMtAct_Out			= 2,
	_e_OutMtAct_Busy		= 3,
}tOutMtAct;

//Travas motores

typedef enum{
	_e_OutLock_Pri			= 0,
	_e_OutLock_Sec			= 1,
	_e_OutLock_Both 		= 2,
}tOutLock;

typedef enum{
	_e_OutLockState_Off		= 0,
	_e_OutLockState_On		= 1,
}tOutLockState;

//Urna

typedef enum{
	_e_OutUrn_Close			= 0,
	_e_OutUrn_Open			= 1,
}tOutUrnSol;

typedef enum{
	_e_OutUrnPic_Off		= 0,
	_e_OutUrnPic_On			= 1,
	_e_OutUrnPic_Blink		= 2,
}tOutUrnPic;

//Comunica��o IO

typedef enum{
	_e_OutCom_Pass_Nop		= 0,
	_e_OutCom_Pass_OK		= 1,
}tOutCom_Pass;

typedef enum{
	_e_OutCom_Cheat_None	= 0,
	_e_OutCom_Cheat_Shit	= 1,
}tOutCom_Cheat;

typedef enum{
	_e_OutDispLed_Off		= 0,
	_e_OutDispLed_On		= 1,
}tOutDispLed;

/*** ** * Funcs Ext * ** ***/

void outputs_Motor_Duty( tOutMt mtNum, tOutMtAct mtSide, uint8_t dutyVal );

void Outputs_Task(void);		//colocar no main
void Outputs_Task_IT(void);	//colocar em alguma interrup��o de timer

void Outputs_Buzz_Set( tOutBuzz buzzSet );
void Outputs_Motor_Set( tOutMt mtNum, tOutMtAct mtAct, uint8_t mtDuty, uint8_t mtAcc );
//void Outputs_Motor_Speed_Set( tOutMt mtNum, uint8_t mtDuty );
void Outputs_Motor_Speed_Set( tOutMt mtNum, uint8_t mtDuty, uint8_t mtAcc );
void Outputs_RGB_Set( tOutPic picNum, tOutPicMode picMode, tOutPicColor picColor, tOutPicSpeed picSpeed );
void Outputs_Urn_Sol_Set( tOutUrnSol urnSolAct );
void Outputs_Urn_Pic_Set( tOutUrnPic picMOde );

void Outputs_DispLed_Set( tOutDispLed dispLedState );
void Outputs_Lock_Set( tOutLock lockNum , tOutLockState lockSts );

void Outputs_Com_Pass( tOutCom_Pass passState );
void Outputs_Com_Cheat( tOutCom_Cheat cheatState );

tOutCom_Pass Outputs_Com_Pass_Get( void );
tOutCom_Cheat Outputs_Com_Cheat_Get( void );

void Outputs_Motor_ForTest(tOutMt mtNum, tOutMtAct mtDir);

#ifdef __cplusplus
}
#endif
#endif /*__outputs_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
