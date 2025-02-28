/**
  ******************************************************************************
  * @file    outputs.c
  * @author  Arthur Murata
  * @version V0.1
  * @date    26/08/2020
  * @brief   Outputs Source
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) TSI.
  *
  ******************************************************************************
  */

/*** ** * Includes * ** ***/

#include "outputs.h"
#include "gpio.h"
#include "timing.h"
#include "tim.h"
#include "sw_config.h"

/*** ** * Defines * ** ***/

#define BUZZ_OFF					_pin_set_l(GPIO_HMI_Buzz_Port,GPIO_HMI_Buzz_Pin)
#define BUZZ_ON						_pin_set_h(GPIO_HMI_Buzz_Port,GPIO_HMI_Buzz_Pin)

//#define BUZZ_FREQ(x)				(OUTPUTS_TIMER_BUZZER->PSC 	= x)//(OUTPUTS_TIMER_BUZZER->ARR 	= x)//PSC
//#define ENABLE_MOTORS				_pin_set_h(GPIO_MT_ENN_Port, GPIO_MT_ENN_Pin);
//#define DISABLE_MOTORS				_pin_set_l(GPIO_MT_ENN_Port, GPIO_MT_ENN_Pin);



#define MT1_DUTY_RIGHT(x)			(x == 0 ? TIM_MOTOR_STOP(TIM_MOTOR_1) : TIM_MOTOR_RUN(TIM_MOTOR_1, x, TIM_DIR_RIGHT))
#define MT1_DUTY_LEFT(x)			(x == 0 ? TIM_MOTOR_STOP(TIM_MOTOR_1) : TIM_MOTOR_RUN(TIM_MOTOR_1, x, TIM_DIR_LEFT))
#define MT2_DUTY_RIGHT(x)			(x == 0 ? TIM_MOTOR_STOP(TIM_MOTOR_2) : TIM_MOTOR_RUN(TIM_MOTOR_2, x, TIM_DIR_RIGHT))
#define MT2_DUTY_LEFT(x)			(x == 0 ? TIM_MOTOR_STOP(TIM_MOTOR_2) : TIM_MOTOR_RUN(TIM_MOTOR_2, x, TIM_DIR_LEFT))

//#define MT1_DUTY_RIGHT(x)			( (OUTPUTS_TIMER_MOTOR_1->ARR = x);GPIO_MT1_DIR_RIGHT)
//#define MT1_DUTY_LEFT(x)			((OUTPUTS_TIMER_MOTOR_1->ARR = x);GPIO_MT1_DIR_LEFT)
//#define MT2_DUTY_RIGHT(x)			((OUTPUTS_TIMER_MOTOR_2->ARR = x);GPIO_MT2_DIR_RIGHT)
//#define MT2_DUTY_LEFT(x)			((OUTPUTS_TIMER_MOTOR_2->ARR = x);GPIO_MT2_DIR_LEFT)

//#define MT1_DUTY_RIGHT_TEST(x)		((OUTPUTS_TIMER_MOTOR_1->AAR = (100-x))(OUTPUTS_TIMER_MOTOR_1->CCR2 = (100-x)/2))

//#define RGB1_R_DUTY(x)				(OUTPUTS_TIMER_PICTO1->CCR1 = x)
//#define RGB1_G_DUTY(x)				(OUTPUTS_TIMER_PICTO1->CCR2 = x)
//#define RGB1_B_DUTY(x)				(OUTPUTS_TIMER_PICTO1->CCR3 = x)
//#define RGB2_R_DUTY(x)				(OUTPUTS_TIMER_PICTO1->CCR4 = x)
//#define RGB2_G_DUTY(x)				(OUTPUTS_TIMER_PICTO2->CCR1 = x)
//#define RGB2_B_DUTY(x)				(OUTPUTS_TIMER_PICTO2->CCR2 = x)

#define RGB1_R_DUTY(x)				(OUTPUTS_TIMER_PICTO1->CCR2 = x)
#define RGB1_G_DUTY(x)				(OUTPUTS_TIMER_PICTO1->CCR3 = x)
#define RGB1_B_DUTY(x)				(OUTPUTS_TIMER_PICTO1->CCR4 = x)
#define RGB2_B_DUTY(x)				(OUTPUTS_TIMER_PICTO1->CCR1 = x)
#define RGB2_R_DUTY(x)				(OUTPUTS_TIMER_PICTO2->CCR3 = x)
#define RGB2_G_DUTY(x)				(OUTPUTS_TIMER_PICTO2->CCR4 = x)
	
#define URN_DUTY(x)					(OUTPUTS_TIMER_URN->CCR1 = x)

#define MT1_LOCK_DUTY(x)			(OUTPUTS_TIMER_MT1_LOCK->CCR1 = x)
#define MT2_LOCK_DUTY(x)			(OUTPUTS_TIMER_MT2_LOCK->CCR1 = x)
	
#define DISPLED_OFF					_pin_set_l(GPIO_HMI_Disp_Led_Port,GPIO_HMI_Disp_Led_Pin)
#define DISPLED_ON					_pin_set_h(GPIO_HMI_Disp_Led_Port,GPIO_HMI_Disp_Led_Pin)

#define SOL_URN_PERCENT				35
#define SOL_URN_PERCENT_MAX			100
#define SOL_URN_ONMAXTIME			2500

#define SOL_PIC_OFF					_pin_set_l(GPIO_IOs_Urn_Pic_Port,GPIO_IOs_Urn_Pic_Pin)
#define SOL_PIC_ON					_pin_set_h(GPIO_IOs_Urn_Pic_Port,GPIO_IOs_Urn_Pic_Pin)
#define SOL_PIC_BLINK_TIME			250

//#define DISPLED_DUTY_MAX			100

#define LOCK_PERCENT_MIN			3
#define LOCK_PERCENT_MAX			50
#define LOCK_ONMAXTIME				100
#define LOCK_SPEED_RAMP_UP			0
#define LOCK_SPEED_RAMP_DW			20


#define MOTOR_STOP_DELAY			250

#define COM_IO_PASS_NOP				_pin_set_l(GPIO_COM_OUT1_Port,GPIO_COM_OUT1_Pin)
#define COM_IO_PASS_OK				_pin_set_h(GPIO_COM_OUT1_Port,GPIO_COM_OUT1_Pin)

#define COM_IO_CHEAT_NONE			_pin_set_l(GPIO_COM_OUT2_Port,GPIO_COM_OUT2_Pin)
#define COM_IO_CHEAT_SHIT			_pin_set_h(GPIO_COM_OUT2_Port,GPIO_COM_OUT2_Pin)

#define BUZZER_SIREN_DEF			48
#define BUZZER_SIREN_MAX			200//60
#define BUZZER_SIREN_MIN			100//1
#define BUZZER_SIREN_SPD			7

/*** ** * CONFIG * ** ***/

const uint16_t buzz_beep_time = 500;

/*** ** * Types * ** ***/

typedef enum{
	_e_BuzzTask_Idle = 0,
	_e_BuzzTask_Off,
	_e_BuzzTask_On,
	_e_BuzzTask_BeepIni,
	_e_BuzzTask_BeepEnd,
	_e_BuzzTask_SirenUp,
	_e_BuzzTask_SirenDw,
}tBuzzTask;

typedef enum{
	_e_MotorTask_Idle = 0,
	_e_MotorTask_RightAcc,
	_e_MotorTask_Right,
	_e_MotorTask_LeftAcc,
	_e_MotorTask_Left,
	_e_MotorTask_Stop,
}tMotorTask;

typedef enum{
	_e_Rgb_R = 0,
	_e_Rgb_G,
	_e_Rgb_B,
}tRgb;

typedef enum{
	_e_UrnSolState_Off = 0,
	_e_UrnSolState_RampUp,
	_e_UrnSolState_OnMax,
	_e_UrnSolState_OnMin,
	_e_UrnSolState_RampDw,
}tUrnSolState;

typedef enum{
	_e_LockState_Off = 0,
	_e_LockState_RampUp,
	_e_LockState_OnMax,
	_e_LockState_OnMin,
	_e_LockState_RampDw,
}tLockState;

/*** ** * Variables * ** ***/

struct{
	tBuzzTask 	taskBuzz;
	uint8_t		sirenCount;
	uint8_t		sirenSpeed;
	tOutBuzz	cmdMem;
	tOutBuzz	cmdCmp;
}sOutBuzz;

struct{
	tMotorTask 	taskMotor;
	uint8_t		dutyUser;
	uint8_t		dutySys;
	tOutMtAct	actUser;
	tOutMtAct	actSys;
	uint8_t		accUser;
	uint8_t		accSys;
	bool		change;
	uint16_t	stopDelay;
}sOutMotor[2];

struct{
	uint8_t			dutySys[3];
	uint8_t			dutyUser[3];
	tOutPicMode		mode;
	tOutPicSpeed	speedSys;
	tOutPicSpeed	speedUser;
	bool 			upDown;
	bool			blinkMode[3];
	bool			blinkModeFlag;
	tOutPicColor	color;
}sOutRgb[2];

struct{
	tOutUrnSol		solUser;
	tUrnSolState	solState;
	uint8_t			dutySys;
	uint16_t		time;
	tOutUrnPic		picSys;
	bool			blinkState;
	uint16_t		blinkTime;
}sOutUrn;

struct{
	uint8_t			dutySys;
	uint8_t			dutyUser;
}sOutDled;

struct{
	tLockState 		stateSys;
	tOutLockState	stateUser;
	uint8_t			dutySys;
	uint8_t			dutyUser;
	uint8_t			timeStepSys;
	uint8_t			timeStepUser;
	uint16_t		timeOnMax;
	uint8_t			rampSpeed;
	tOutLockState	pinState;
}sOutLock[2];

struct{
	tOutCom_Pass	passSts;
	tOutCom_Cheat	cheatSts;
}sOutCom;

/*** ** * Funcs Int * ** ***/

//Buzzer

void outputs_Buzz_Task(void)
{
	switch( (uint8_t)sOutBuzz.taskBuzz )
	{
		case _e_BuzzTask_Idle:
			sOutBuzz.sirenCount = BUZZER_SIREN_DEF;
			break;
		case _e_BuzzTask_Off:
			sOutBuzz.sirenCount = BUZZER_SIREN_DEF;
			BUZZ_OFF;
			sOutBuzz.taskBuzz = _e_BuzzTask_Idle;
			break;
		case _e_BuzzTask_On:
			sOutBuzz.sirenCount = BUZZER_SIREN_DEF;
			BUZZ_ON;
			sOutBuzz.taskBuzz = _e_BuzzTask_Idle;
			break;
		case _e_BuzzTask_BeepIni:
			sOutBuzz.sirenCount = BUZZER_SIREN_DEF;
			BUZZ_ON;
			Timing_Set( _e_Timing_Buzz , buzz_beep_time );
			sOutBuzz.taskBuzz = _e_BuzzTask_BeepEnd;
			break;
		case _e_BuzzTask_BeepEnd:
			sOutBuzz.sirenCount = BUZZER_SIREN_DEF;
			if( Timing_Get_Bool(_e_Timing_Buzz) ){
				BUZZ_OFF;
				sOutBuzz.taskBuzz = _e_BuzzTask_Idle;
			}
			break;
			
		case _e_BuzzTask_SirenUp:
			BUZZ_ON;
			if( sOutBuzz.sirenSpeed < BUZZER_SIREN_SPD )sOutBuzz.sirenSpeed++;
			else
			{
				sOutBuzz.sirenSpeed = 0;
				if( sOutBuzz.sirenCount < BUZZER_SIREN_MAX )sOutBuzz.sirenCount++;
				else{
					//sOutBuzz.sirenCount = BUZZER_SIREN_MAX;
					sOutBuzz.taskBuzz = _e_BuzzTask_SirenDw;
				}
			}
			break;
		case _e_BuzzTask_SirenDw:
			BUZZ_OFF;
			if( sOutBuzz.sirenSpeed < BUZZER_SIREN_SPD )sOutBuzz.sirenSpeed++;
			else
			{
				sOutBuzz.sirenSpeed = 0;
				if( sOutBuzz.sirenCount > BUZZER_SIREN_MIN )sOutBuzz.sirenCount--;
				else{
					//sOutBuzz.sirenCount = BUZZER_SIREN_MIN;
					sOutBuzz.taskBuzz = _e_BuzzTask_SirenUp;
				}
			}
			break;
		default:
			sOutBuzz.taskBuzz = _e_BuzzTask_Off;
			break;
	}
	//BUZZ_FREQ(sOutBuzz.sirenCount);
}

//Motor


void outputs_Motor_Duty( tOutMt mtNum, tOutMtAct mtSide, uint8_t dutyVal )
{
	if( mtNum == _e_OutMt_Pri && mtSide == _e_OutMtAct_Out ){
//		MT1_DUTY_LEFT(0);
		MT1_DUTY_RIGHT(dutyVal);
	}
	else if	( mtNum == _e_OutMt_Pri && mtSide == _e_OutMtAct_In ){
//		MT1_DUTY_RIGHT(0);
		MT1_DUTY_LEFT(dutyVal);
	}
	else if	( mtNum == _e_OutMt_Pri && mtSide == _e_OutMtAct_Stop ){
//		MT1_DUTY_RIGHT(0);
		MT1_DUTY_LEFT(0);
	}
	else if	( mtNum == _e_OutMt_Sec && mtSide == _e_OutMtAct_Out ){
//		MT2_DUTY_RIGHT(0);
		MT2_DUTY_LEFT(dutyVal);
	}
	else if	( mtNum == _e_OutMt_Sec && mtSide == _e_OutMtAct_In ){
//		MT2_DUTY_LEFT(0);
		MT2_DUTY_RIGHT(dutyVal);
	}
	else if	( mtNum == _e_OutMt_Sec && mtSide == _e_OutMtAct_Stop ){
//		MT2_DUTY_RIGHT(0);
		MT2_DUTY_LEFT(0);
	}
	else if	( mtNum == _e_OutMt_Both && mtSide == _e_OutMtAct_Out ){
//		MT1_DUTY_LEFT(0);
//		MT2_DUTY_RIGHT(0);
		MT1_DUTY_RIGHT(dutyVal);
		MT2_DUTY_LEFT(dutyVal);
	}
	else if	( mtNum == _e_OutMt_Both && mtSide == _e_OutMtAct_In ){
//		MT1_DUTY_RIGHT(0);
//		MT2_DUTY_LEFT(0);
		MT1_DUTY_LEFT(dutyVal);
		MT2_DUTY_RIGHT(dutyVal);
	}
	else if	( mtNum == _e_OutMt_Both && mtSide == _e_OutMtAct_Stop ){
		MT1_DUTY_LEFT(0);
//		MT2_DUTY_LEFT(0);
//		MT1_DUTY_RIGHT(0);
		MT2_DUTY_RIGHT(0);
	}
}

void outputs_Motor_Task(void)
{
	for( uint8_t i = 0 ; i < 2 ; i++ )
	{	
		switch( (uint8_t)sOutMotor[i].taskMotor )
		{
			case _e_MotorTask_Idle:
				outputs_Motor_Duty( (tOutMt)i, _e_OutMtAct_Stop, 0 );
				if( sOutMotor[i].actSys == _e_OutMtAct_Out ){
					sOutMotor[i].taskMotor = _e_MotorTask_RightAcc;
				}
				if( sOutMotor[i].actSys == _e_OutMtAct_In ){
					sOutMotor[i].taskMotor = _e_MotorTask_LeftAcc;
				}
				sOutMotor[i].dutySys = 0;
				sOutMotor[i].accSys = 0;
				break;
				
			case _e_MotorTask_RightAcc:
				outputs_Motor_Duty( (tOutMt)i, _e_OutMtAct_Out, sOutMotor[i].dutySys );
				if( sOutMotor[i].dutySys == sOutMotor[i].dutyUser ){
					sOutMotor[i].taskMotor = _e_MotorTask_Right;
				}
				break;
			
			case _e_MotorTask_Right:
				outputs_Motor_Duty( (tOutMt)i, _e_OutMtAct_Out, sOutMotor[i].dutySys );
				break;
			
			case _e_MotorTask_LeftAcc:
				outputs_Motor_Duty( (tOutMt)i, _e_OutMtAct_In, sOutMotor[i].dutySys );
				if( sOutMotor[i].dutySys == sOutMotor[i].dutyUser ){
					sOutMotor[i].taskMotor = _e_MotorTask_Left;
				}
				break;
			
			case _e_MotorTask_Left:
				outputs_Motor_Duty( (tOutMt)i, _e_OutMtAct_In, sOutMotor[i].dutySys );
				break;
			
			case _e_MotorTask_Stop:
				outputs_Motor_Duty( (tOutMt)i, _e_OutMtAct_Stop, 0 );
				if( sOutMotor[i].stopDelay < MOTOR_STOP_DELAY )sOutMotor[i].stopDelay++;
				else{
					sOutMotor[i].stopDelay = 0;
					sOutMotor[i].taskMotor = _e_MotorTask_Idle;
				}
				break;
			
			default:
				sOutMotor[i].taskMotor = _e_MotorTask_Idle;
				break;
		}
	}
}

void outputs_Motor_IT(void)
{
	for( uint8_t i = 0 ; i < 2 ; i++ )
	{
		if( sOutMotor[i].accSys > sOutMotor[i].accUser )
		{
			sOutMotor[i].accSys = 0;
			
			if( sOutMotor[i].dutySys < sOutMotor[i].dutyUser ){
				sOutMotor[i].dutySys++;
			}
			else if( sOutMotor[i].dutySys > sOutMotor[i].dutyUser ){
				sOutMotor[i].dutySys--;
			}
		}
		else{
			sOutMotor[i].accSys++;
		}
	}
}

//Picto RGB

void output_RGB_Set( tOutPic inOut, uint8_t valR, uint8_t valG, uint8_t valB )
{
	if			( inOut == _e_OutPic_In ){
		//RGB1_R_DUTY(valR); RGB1_G_DUTY(valG); RGB1_B_DUTY(valB);
		RGB2_R_DUTY(valR); RGB2_G_DUTY(valG); RGB2_B_DUTY(valB);
	}
	else if	( inOut == _e_OutPic_Out ){
		//RGB2_R_DUTY(valR); RGB2_G_DUTY(valG); RGB2_B_DUTY(valB);
		RGB1_R_DUTY(valR); RGB1_G_DUTY(valG); RGB1_B_DUTY(valB);
	}
	else if	( inOut == _e_OutPic_Both ){
		RGB1_R_DUTY(valR); RGB1_G_DUTY(valG); RGB1_B_DUTY(valB);
		RGB2_R_DUTY(valR); RGB2_G_DUTY(valG); RGB2_B_DUTY(valB);
	}
}

void output_RGB_ctrl( tOutPic picNum, bool uD )
{
	if( uD )
	{
		if( sOutRgb[picNum].speedSys < sOutRgb[picNum].speedUser ){
			sOutRgb[picNum].speedSys++;
		}
		else{
			sOutRgb[picNum].speedSys = (tOutPicSpeed)0;
			for( uint8_t x = 0 ; x < 3 ; x++ ){
				if ( sOutRgb[picNum].dutySys[x] < sOutRgb[picNum].dutyUser[x] )sOutRgb[picNum].dutySys[x]++;
				else if	( sOutRgb[picNum].dutySys[x] > sOutRgb[picNum].dutyUser[x] )sOutRgb[picNum].dutySys[x]--;
				else sOutRgb[picNum].blinkMode[x] = true;
			}
		}
		if( sOutRgb[picNum].blinkMode[_e_Rgb_R] == true && 
				sOutRgb[picNum].blinkMode[_e_Rgb_G] == true && 
				sOutRgb[picNum].blinkMode[_e_Rgb_B] == true )
		{
			sOutRgb[picNum].blinkModeFlag = true;
		}
	}
	else
	{
		if( sOutRgb[picNum].speedSys < sOutRgb[picNum].speedUser ){
			sOutRgb[picNum].speedSys++;
		}
		else{
			sOutRgb[picNum].speedSys = (tOutPicSpeed)0;
			for( uint8_t x = 0 ; x < 3 ; x++ ){
				if( sOutRgb[picNum].dutySys[x] > 0 )sOutRgb[picNum].dutySys[x]--;
				else sOutRgb[picNum].blinkMode[x] = false;
			}
		}
		if( sOutRgb[picNum].blinkMode[_e_Rgb_R] == false && 
				sOutRgb[picNum].blinkMode[_e_Rgb_G] == false && 
				sOutRgb[picNum].blinkMode[_e_Rgb_B] == false )
		{
			sOutRgb[picNum].blinkModeFlag = false;
		}
	}
}

void outputs_RGB_IT(void)
{
	for( uint8_t i = 0 ; i < 2 ; i++ )
	{
		if( sOutRgb[i].mode == _e_OutPicMode_Off )
		{
			output_RGB_ctrl( (tOutPic)i , false );
		}
		else if( sOutRgb[i].mode == _e_OutPicMode_Static )
		{
			output_RGB_ctrl( (tOutPic)i , true );
		}
		else if( sOutRgb[i].mode == _e_OutPicMode_Blink )
		{
			if( sOutRgb[i].blinkModeFlag == true ) 	output_RGB_ctrl( (tOutPic)i , false );
			else output_RGB_ctrl( (tOutPic)i , true );
		}
		output_RGB_Set( (tOutPic)i, sOutRgb[i].dutySys[_e_Rgb_R], sOutRgb[i].dutySys[_e_Rgb_G], sOutRgb[i].dutySys[_e_Rgb_B] );
	}
}
 
void outputs_RGB_Color_Table( tOutPicColor color, uint8_t* colR, uint8_t* colG, uint8_t* colB )
{
	switch( (uint8_t)color )
	{
#if(SW_PICTOGRAMA_MODE == SW_PICTOGRAMA_FITA_LED)
		case _e_OutPicColor_Red			:   *colR=0xFF;	*colG=0x10;	*colB=0x10;	break;
		case _e_OutPicColor_Green		:	*colR=0x10;	*colG=0xFF;	*colB=0x10;	break;
		case _e_OutPicColor_Blue		:	*colR=0x10;	*colG=0x10;	*colB=0xFF;	break;
		case _e_OutPicColor_Yellow		:	*colR=0xFF;	*colG=0xFF;	*colB=0x00;	break;
		case _e_OutPicColor_Pink		:	*colR=0xFF;	*colG=0x00;	*colB=0xFF;	break;
		case _e_OutPicColor_Lblue		:	*colR=0x00;	*colG=0xFF;	*colB=0xFF;	break;
		case _e_OutPicColor_TSIgr		:	*colR=0x00;	*colG=0xFF;	*colB=0x50;	break;
		case _e_OutPicColor_White		:	*colR=0xFF;	*colG=0xFF;	*colB=0xFF;	break;
		default							:	*colR=0x00;	*colG=0x00;	*colB=0x00;	break;
#else
		case _e_OutPicColor_Red			:   *colR=0xFF;	*colG=0x00;	*colB=0x00;	break;
		case _e_OutPicColor_Green		:	*colR=0x00;	*colG=0xFF;	*colB=0x00;	break;
		case _e_OutPicColor_Blue		:	*colR=0x00;	*colG=0x00;	*colB=0xFF;	break;
		case _e_OutPicColor_Yellow		:	*colR=0xFF;	*colG=0xFF;	*colB=0x00;	break;
		case _e_OutPicColor_Pink		:	*colR=0xFF;	*colG=0x00;	*colB=0xFF;	break;
		case _e_OutPicColor_Lblue		:	*colR=0x00;	*colG=0xFF;	*colB=0xFF;	break;
		case _e_OutPicColor_TSIgr		:	*colR=0x00;	*colG=0xFF;	*colB=0x50;	break;
		case _e_OutPicColor_White		:	*colR=0xFF;	*colG=0xFF;	*colB=0xFF;	break;
		default							:	*colR=0x00;	*colG=0x00;	*colB=0x00;	break;
#endif
	}
}

//URN

void outputs_Urn_IT(void)
{
	switch( (uint8_t)sOutUrn.solState )
	{
	case _e_UrnSolState_Off		:
		sOutUrn.dutySys = 0;
		if( sOutUrn.solUser == _e_OutUrn_Open ) sOutUrn.solState = _e_UrnSolState_RampUp;
		break;
	case _e_UrnSolState_RampUp:
		if( sOutUrn.dutySys < SOL_URN_PERCENT_MAX )sOutUrn.dutySys++;
		else{
			sOutUrn.solState = _e_UrnSolState_OnMax;
			sOutUrn.time = SOL_URN_ONMAXTIME;
		}
		if( sOutUrn.solUser == _e_OutUrn_Close ) sOutUrn.solState = _e_UrnSolState_RampDw;
		break;
	case _e_UrnSolState_OnMax	:
		sOutUrn.dutySys = SOL_URN_PERCENT_MAX;
		if( sOutUrn.time > 0 )sOutUrn.time--;
		else sOutUrn.solState = _e_UrnSolState_OnMin;
		if( sOutUrn.solUser == _e_OutUrn_Close ) sOutUrn.solState = _e_UrnSolState_RampDw;
		break;
	case _e_UrnSolState_OnMin	:
		if( sOutUrn.dutySys > SOL_URN_PERCENT )sOutUrn.dutySys--;
		if( sOutUrn.solUser == _e_OutUrn_Close ) sOutUrn.solState = _e_UrnSolState_RampDw;
		break;
	case _e_UrnSolState_RampDw:
		if( sOutUrn.dutySys > 0 )sOutUrn.dutySys--;
		else sOutUrn.solState = _e_UrnSolState_Off;
		break;
	default:
		break;
	}
	URN_DUTY(sOutUrn.dutySys);
	
	switch( (uint8_t)sOutUrn.picSys )
	{
		case _e_OutUrnPic_Off:
			SOL_PIC_OFF;
			sOutUrn.blinkState = true;
			sOutUrn.blinkTime = 0;
			break;
		case _e_OutUrnPic_On:
			SOL_PIC_ON;
			sOutUrn.blinkState = true;
			sOutUrn.blinkTime = 0;
			break;
		case _e_OutUrnPic_Blink:
			if( sOutUrn.blinkTime > 0 ){ sOutUrn.blinkTime--; return; }
			else { sOutUrn.blinkTime = SOL_PIC_BLINK_TIME; }
			if( sOutUrn.blinkState == false ) SOL_PIC_OFF; 
			else SOL_PIC_ON;
			sOutUrn.blinkState = (bool)!sOutUrn.blinkState;
			break;
		default:
			sOutUrn.picSys = _e_OutUrnPic_Off;
			break;
	}
}

//Locks - Virtual PWM
/*
void outputs_Locks_Ctrl( tOutLock lockNum, tOutLockState lockState )
{
	if( lockNum == _e_OutLock_Pri ){
		if( lockState == _e_OutLockState_On )
			_pin_set_h(GPIO_MT1_Lock1_Port,GPIO_MT1_Lock1_Pin);
		else
			_pin_set_l(GPIO_MT1_Lock1_Port,GPIO_MT1_Lock1_Pin);
	}
	else{
		if( lockState == _e_OutLockState_On )
			_pin_set_h(GPIO_MT2_Lock1_Port,GPIO_MT2_Lock1_Pin);
		else
			_pin_set_l(GPIO_MT2_Lock1_Port,GPIO_MT2_Lock1_Pin);
	}
}
*/

void outputs_motor_lock_duty( tOutMt mtNum, uint8_t dutyVal )
{
	if( mtNum == _e_OutMt_Pri )	MT1_LOCK_DUTY(dutyVal);
	else						MT2_LOCK_DUTY(dutyVal);
}

void outputs_Locks_IT(void)
{
	for( uint8_t i = 0 ; i < 2 ; i++ )
	{
	
		switch( (uint8_t)sOutLock[i].stateSys )
		{
			case _e_LockState_Off:
				sOutLock[i].dutySys = 0;
				if( sOutLock[i].stateUser == _e_OutLockState_On ){
					sOutLock[i].stateSys = _e_LockState_RampUp;
					sOutLock[i].rampSpeed = LOCK_SPEED_RAMP_UP;
				}
				break;
			
			case _e_LockState_RampUp:
				if( sOutLock[i].rampSpeed > 0 ){
					sOutLock[i].rampSpeed--;
					return;
				}
				sOutLock[i].rampSpeed = LOCK_SPEED_RAMP_UP;

				if( sOutLock[i].dutySys < LOCK_PERCENT_MAX )sOutLock[i].dutySys++;
				else{
					sOutLock[i].stateSys = _e_LockState_OnMax;
					sOutLock[i].timeOnMax = LOCK_ONMAXTIME;
				}
				if( sOutLock[i].stateUser == _e_OutLockState_Off )sOutLock[i].stateSys = _e_LockState_Off;
				break;
			
			case _e_LockState_OnMax:
				sOutLock[i].dutySys = LOCK_PERCENT_MAX;
				if ( sOutLock[i].timeOnMax > 0 )sOutLock[i].timeOnMax--;
				else {
					sOutLock[i].stateSys = _e_LockState_RampDw;
					sOutLock[i].rampSpeed = LOCK_SPEED_RAMP_DW;
				}
				if( sOutLock[i].stateUser == _e_OutLockState_Off )sOutLock[i].stateSys = _e_LockState_Off;
				break;
			
			case _e_LockState_RampDw:
				if( sOutLock[i].rampSpeed > 0 ){
					sOutLock[i].rampSpeed--;
					return;
				}
				sOutLock[i].rampSpeed = LOCK_SPEED_RAMP_DW;

				if( sOutLock[i].dutySys > LOCK_PERCENT_MIN )sOutLock[i].dutySys--;
				else sOutLock[i].stateSys = _e_LockState_OnMin;
				if( sOutLock[i].stateUser == _e_OutLockState_Off )sOutLock[i].stateSys = _e_LockState_Off;
				break;
			
			case _e_LockState_OnMin	:
				if( sOutLock[i].stateUser == _e_OutLockState_Off )sOutLock[i].stateSys = _e_LockState_Off;
				break;

			default:
				sOutLock[i].stateSys = _e_LockState_Off;
				break;
		}
		
		outputs_motor_lock_duty( (tOutMt)i, sOutLock[i].dutySys );
	}
}

/*** ** * Funcs Ext * ** ***/

//Buzzer

/*
	_e_BuzzTask_Idle = 0,
	_e_BuzzTask_Off,
	_e_BuzzTask_On,
	_e_BuzzTask_BeepIni,
	_e_BuzzTask_BeepEnd,
	_e_BuzzTask_SirenUp,
	_e_BuzzTask_SirenDw,
*/

void Outputs_Buzz_Set( tOutBuzz buzzSet )
{
	sOutBuzz.cmdCmp = buzzSet;

	if( sOutBuzz.cmdMem == sOutBuzz.cmdCmp )return;

	sOutBuzz.cmdMem = sOutBuzz.cmdCmp;

	switch( (uint8_t)buzzSet )
	{
		case _e_OutBuzz_Off		: sOutBuzz.taskBuzz = _e_BuzzTask_Off;			break;
		case _e_OutBuzz_On		:	sOutBuzz.taskBuzz = _e_BuzzTask_On;				break;
		case _e_OutBuzz_Beep	:	sOutBuzz.taskBuzz = _e_BuzzTask_BeepIni;	break;
		case _e_OutBuzz_Siren	:	
			if( sOutBuzz.taskBuzz != _e_BuzzTask_SirenUp &&	sOutBuzz.taskBuzz != _e_BuzzTask_SirenDw )
			{
				sOutBuzz.taskBuzz = _e_BuzzTask_SirenUp;
			}
			break;
		default:								sOutBuzz.taskBuzz = _e_BuzzTask_Off;			break;
	}
	
//#warning "BUZRE DESLIGADO"
	//sOutBuzz.taskBuzz = _e_BuzzTask_Off;
	//sOutBuzz.taskBuzz = (tBuzzTask)buzzSet;
}

//Motor

void Outputs_Motor_Set( tOutMt mtNum, tOutMtAct mtAct, uint8_t mtDuty, uint8_t mtAcc )
{
	if( mtNum >= _e_OutMt_Both ){
		sOutMotor[_e_OutMt_Pri].actUser = mtAct;
		sOutMotor[_e_OutMt_Sec].actUser = mtAct;
		sOutMotor[_e_OutMt_Pri].dutyUser = mtDuty;
		sOutMotor[_e_OutMt_Sec].dutyUser = mtDuty;
		sOutMotor[_e_OutMt_Pri].accUser = mtAcc;
		sOutMotor[_e_OutMt_Sec].accUser = mtAcc;
		sOutMotor[_e_OutMt_Pri].change = true;
		sOutMotor[_e_OutMt_Sec].change = true;
		if( sOutMotor[_e_OutMt_Pri].actUser != sOutMotor[_e_OutMt_Pri].actSys ){
			sOutMotor[_e_OutMt_Pri].actSys = sOutMotor[_e_OutMt_Pri].actUser;
			sOutMotor[_e_OutMt_Pri].taskMotor = _e_MotorTask_Stop;
		}
		if( sOutMotor[_e_OutMt_Sec].actUser != sOutMotor[_e_OutMt_Sec].actSys ){
			sOutMotor[_e_OutMt_Sec].actSys = sOutMotor[_e_OutMt_Sec].actUser;
			sOutMotor[_e_OutMt_Sec].taskMotor = _e_MotorTask_Stop;
		}
	}
	else{
		sOutMotor[mtNum].actUser = mtAct;
		sOutMotor[mtNum].dutyUser = mtDuty;
		sOutMotor[mtNum].accUser = mtAcc;
		sOutMotor[mtNum].change = true;
		if( sOutMotor[mtNum].actUser != sOutMotor[mtNum].actSys ) {
			sOutMotor[mtNum].actSys = sOutMotor[mtNum].actUser;
			sOutMotor[mtNum].taskMotor = _e_MotorTask_Stop;
		}
	}
}

void Outputs_Motor_Speed_Set( tOutMt mtNum, uint8_t mtDuty, uint8_t mtAcc )
{
	if( mtNum >= _e_OutMt_Both ){
		sOutMotor[_e_OutMt_Pri].dutyUser = mtDuty;
		sOutMotor[_e_OutMt_Sec].dutyUser = mtDuty;
		sOutMotor[_e_OutMt_Pri].accUser = mtAcc;
		sOutMotor[_e_OutMt_Sec].accUser = mtAcc;
		sOutMotor[_e_OutMt_Pri].change = true;
		sOutMotor[_e_OutMt_Sec].change = true;

	}
	else{
		sOutMotor[mtNum].dutyUser = mtDuty;
		sOutMotor[mtNum].accUser = mtAcc;
		sOutMotor[mtNum].change = true;
	}
}

//RGB Pic

void Outputs_RGB_Set( tOutPic picNum, tOutPicMode picMode, tOutPicColor picColor, tOutPicSpeed picSpeed )
{
	if( picNum >= _e_OutPic_Both ){
		for( uint8_t x = 0 ; x < _e_OutPic_Both ; x++ ){
			sOutRgb[x].mode = picMode;
			sOutRgb[x].color = picColor;
			sOutRgb[x].speedUser = picSpeed;
			outputs_RGB_Color_Table( picColor, &sOutRgb[x].dutyUser[_e_Rgb_R], &sOutRgb[x].dutyUser[_e_Rgb_G], &sOutRgb[x].dutyUser[_e_Rgb_B] );
		}
	}
	else{
		sOutRgb[picNum].mode = picMode;
		sOutRgb[picNum].color = picColor;
		sOutRgb[picNum].speedUser = picSpeed;
		outputs_RGB_Color_Table( sOutRgb[picNum].color, &sOutRgb[picNum].dutyUser[_e_Rgb_R], &sOutRgb[picNum].dutyUser[_e_Rgb_G], &sOutRgb[picNum].dutyUser[_e_Rgb_B] );
	}
}

//Urn

void Outputs_Urn_Sol_Set( tOutUrnSol urnSolAct )
{
	sOutUrn.solUser = urnSolAct;
}

void Outputs_Urn_Pic_Set( tOutUrnPic picMOde )
{
	sOutUrn.picSys = picMOde;
}

//Disp Led

void Outputs_DispLed_Set( tOutDispLed dispLedState )
{
	if( dispLedState )	DISPLED_ON;
	else				DISPLED_OFF;
}

//Locks

void Outputs_Lock_Set( tOutLock lockNum , tOutLockState lockSts )
{
	
	if( lockNum >= _e_OutLock_Both ){
		sOutLock[_e_OutLock_Pri].stateUser = lockSts;
		sOutLock[_e_OutLock_Sec].stateUser = lockSts;
	}
	else sOutLock[lockNum].stateUser = lockSts;
	
	if( lockNum == _e_OutLock_Pri ){
		if( lockSts == _e_OutLockState_On )	_pin_set_h(GPIO_MT1_Lock1_Port,GPIO_MT1_Lock1_Pin);
		else															_pin_set_l(GPIO_MT1_Lock1_Port,GPIO_MT1_Lock1_Pin);
	}
	else{
		if( lockSts == _e_OutLockState_On )	_pin_set_h(GPIO_MT2_Lock1_Port,GPIO_MT2_Lock1_Pin);
		else															_pin_set_l(GPIO_MT2_Lock1_Port,GPIO_MT2_Lock1_Pin);
	}
}

//IO COM

void Outputs_Com_Pass( tOutCom_Pass passState )
{
	sOutCom.passSts = passState;
	if( passState == _e_OutCom_Pass_Nop )
		COM_IO_PASS_NOP;
	else
		COM_IO_PASS_OK;
}

void Outputs_Com_Cheat( tOutCom_Cheat cheatState )
{
	sOutCom.cheatSts = cheatState;
	if( cheatState == _e_OutCom_Cheat_None )COM_IO_CHEAT_NONE;
	else COM_IO_CHEAT_SHIT;
}

tOutCom_Pass Outputs_Com_Pass_Get( void ){
	return sOutCom.passSts;
}

tOutCom_Cheat Outputs_Com_Cheat_Get( void ){
	return sOutCom.cheatSts;
}

/*** ** * Main Task * ** ***/

void Outputs_Task_IT(void)
{
//	outputs_Motor_IT();
	outputs_RGB_IT();
	outputs_Urn_IT();
	//outputs_DispLed_IT();
	outputs_Locks_IT();
}

void Outputs_Task(void)
{
	outputs_Buzz_Task();
	//outputs_Motor_Task();
}

void Outputs_Motor_ForTest(tOutMt mtNum, tOutMtAct mtDir)
{
	if( mtNum == _e_OutMt_Pri)
	{
		if(mtDir == _e_OutMtAct_In)
		{
			MT1_DUTY_RIGHT(0);
			MT1_DUTY_LEFT(100);
		}
		else if(mtDir == _e_OutMtAct_Out)
		{
			MT1_DUTY_LEFT(0);
			MT1_DUTY_RIGHT(100);
		}
		else
		{
			MT1_DUTY_LEFT(0);
			MT1_DUTY_RIGHT(0);
		}
	}
	else if( mtNum == _e_OutMt_Sec)
	{
		if(mtDir == _e_OutMtAct_In)
		{
			MT2_DUTY_LEFT(100);
			MT2_DUTY_RIGHT(0);
		}
		else if(mtDir == _e_OutMtAct_Out)
		{
			MT2_DUTY_LEFT(0);
			MT2_DUTY_RIGHT(100);
		}
		else
		{
			MT2_DUTY_LEFT(0);
			MT2_DUTY_RIGHT(0);
		}
	}
	else
	{
		if(mtDir == _e_OutMtAct_In)
		{
			MT1_DUTY_LEFT(100);
			MT1_DUTY_RIGHT(0);
			MT2_DUTY_LEFT(100);
			MT2_DUTY_RIGHT(0);
		}
		else if(mtDir == _e_OutMtAct_Out)
		{
			MT1_DUTY_LEFT(0);
			MT1_DUTY_RIGHT(100);
			MT2_DUTY_LEFT(0);
			MT2_DUTY_RIGHT(100);
		}
		else
		{
			MT1_DUTY_LEFT(0);
			MT1_DUTY_RIGHT(0);
			MT2_DUTY_LEFT(0);
			MT2_DUTY_RIGHT(0);
		}
	}
}
/*****************************END OF FILE****/
