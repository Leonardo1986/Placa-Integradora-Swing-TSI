/**
  ******************************************************************************
  * @file    inputs.c
  * @author  Arthur Murata
  * @version V0.1
  * @date    26/08/2020
  * @brief   Inputs Source
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) TSI.
  *
  ******************************************************************************
  */

/*** ** * Includes * ** ***/

#include "inputs.h"
#include "gpio.h"
#include "encoder.h"

#include "timing.h"
#include "outputs.h"
/*** ** * Defines * ** ***/

#define _pinGet_Sen_In()			_pin_get(GPIO_IOs_Sen_In_Port,			GPIO_IOs_Sen_In_Pin     )
#define _pinGet_Sen_Out()			_pin_get(GPIO_IOs_Sen_Out_Port,			GPIO_IOs_Sen_Out_Pin    )
#define _pinGet_Sen_MidUp()			_pin_get(GPIO_IOs_Sen_MidUp_Port,		GPIO_IOs_Sen_MidUp_Pin  )
#define _pinGet_Sen_MidDown()		_pin_get(GPIO_IOs_Sen_MidDown_Port,		GPIO_IOs_Sen_MidDown_Pin)
#define _pinGet_Com_In1()			_pin_get(GPIO_COM_IN1_Port,				GPIO_COM_IN1_Pin        )
#define _pinGet_Com_In2()			_pin_get(GPIO_COM_IN2_Port,				GPIO_COM_IN2_Pin        )
#define _pinGet_Mt1_Enc2()			_pin_get(GPIO_MT1_Enc2_Port,			GPIO_MT1_Enc2_Pin       )
#define _pinGet_Mt1_Enc1()			_pin_get(GPIO_MT1_Enc1_Port,			GPIO_MT1_Enc1_Pin       )
#define _pinGet_Mt1_Disc1()			_pin_get(GPIO_MT1_Disc1_Port,			GPIO_MT1_Disc1_Pin      )
#define _pinGet_Mt1_Disc2()			_pin_get(GPIO_MT1_Disc2_Port,			GPIO_MT1_Disc2_Pin      )
#define _pinGet_Mt2_Enc2()			_pin_get(GPIO_MT2_Enc2_Port,			GPIO_MT2_Enc2_Pin       )
#define _pinGet_Mt2_Enc1()			_pin_get(GPIO_MT2_Enc1_Port,			GPIO_MT2_Enc1_Pin       )
#define _pinGet_Mt2_Disc1()			_pin_get(GPIO_MT2_Disc1_Port,			GPIO_MT2_Disc1_Pin      )
#define _pinGet_Mt2_Disc2()			_pin_get(GPIO_MT2_Disc2_Port,			GPIO_MT2_Disc2_Pin      )
#define _pinGet_Urn_Det()			_pin_get(GPIO_IOs_Urn_Det_Port,			GPIO_IOs_Urn_Det_Pin    )

#define _pinGet_Config_MotorBit2()	   HAL_GPIO_ReadPin(GPIO_HMI_BUS_D4_Port, GPIO_HMI_BUS_D4_Pin)
#define _pinGet_Config_MotorBit1()	   HAL_GPIO_ReadPin(GPIO_HMI_BUS_D5_Port, GPIO_HMI_BUS_D5_Pin)
#define _pinGet_Config_Barrier()       HAL_GPIO_ReadPin(GPIO_HMI_BUS_D6_Port, GPIO_HMI_BUS_D6_Pin)
#define _pinGet_Config_Sensor()        HAL_GPIO_ReadPin(GPIO_HMI_BUS_D7_Port, GPIO_HMI_BUS_D7_Pin)

#define IN_COM_DEB	1000

/*** ** * Variables * ** ***/

const uint16_t inDebConst[_e_Inputs_QTY]= { 50,	//	_e_Inputs_SenIn
											50,	//	_e_Inputs_SenOut
											50,	//	_e_Inputs_SenMidU
											50,	//	_e_Inputs_SenMidD
											500,//	_e_Inputs_ComIn1
											500,//	_e_Inputs_ComIn2
											2,	//	_e_Inputs_Mt1Sen
											2,	//	_e_Inputs_Mt1Enc1
											20,	//	_e_Inputs_Mt1Disc1
											20,	//	_e_Inputs_Mt1Disc2
											2,	//	_e_Inputs_Mt2Sen
											2,	//	_e_Inputs_Mt2Enc1
											20,	//	_e_Inputs_Mt2Disc1
											20,	//	_e_Inputs_Mt2Disc2
											5 	//	_e_Inputs_UrnDet
											};

//uint8_t	inDebCount[_e_Inputs_QTY];
//bool		inState[_e_Inputs_QTY];

struct{
	uint16_t	count;
	bool		state;
}sInPin[_e_Inputs_QTY];
							
struct{
	tInCom		state;
	tInCom		stateNoDeb;
	tInCom		stateNoDebMem;
	uint16_t	debCount;
}sInCom;

struct{
	bool			pinSen;
	bool			pinEnc;
	bool			pinDisc1;
	bool			pinDisc2;
	
	uint8_t			enc;
	uint8_t			encCmp;
	bool			encChange;
	uint8_t			disc;
	bool			discChange;
}sInMt[2];

struct{
	bool	cardDetFlag;
}sInUrn;
/*** ** * Funcs Int * ** ***/

uint32_t inputs_get( tInputs in )
{
	switch( (uint8_t)in )
	{
		case _e_Inputs_SenIn: 		return _pinGet_Sen_In();
		case _e_Inputs_SenOut: 		return _pinGet_Sen_Out();
		case _e_Inputs_SenMidU: 	return _pinGet_Sen_MidUp();
		case _e_Inputs_SenMidD: 	return _pinGet_Sen_MidDown();
		case _e_Inputs_ComIn1: 		return _pinGet_Com_In1();
		case _e_Inputs_ComIn2: 		return _pinGet_Com_In2();

		case _e_Inputs_Mt1Enc2: 	return _pinGet_Mt1_Disc2();	//está trocado os pinos do encoder com posição
		case _e_Inputs_Mt1Enc1: 	return _pinGet_Mt1_Disc1();	//algum dia alguem tem que corrigir isso
		case _e_Inputs_Mt1Disc1: 	return _pinGet_Mt1_Enc2();
		case _e_Inputs_Mt1Disc2: 	return _pinGet_Mt1_Enc1();

		case _e_Inputs_Mt2Enc2: 	return _pinGet_Mt2_Disc2();
		case _e_Inputs_Mt2Enc1: 	return _pinGet_Mt2_Disc1();
		case _e_Inputs_Mt2Disc1: 	return _pinGet_Mt2_Enc2();
		case _e_Inputs_Mt2Disc2: 	return _pinGet_Mt2_Enc1();

//		case _e_Inputs_Mt1Enc2: 	return _pinGet_Mt1_Enc2();
//		case _e_Inputs_Mt1Enc1: 	return _pinGet_Mt1_Enc1();
//		case _e_Inputs_Mt1Disc1: 	return _pinGet_Mt1_Disc2();
//		case _e_Inputs_Mt1Disc2: 	return _pinGet_Mt1_Disc1();
//		case _e_Inputs_Mt2Enc2: 	return _pinGet_Mt2_Enc2();
//		case _e_Inputs_Mt2Enc1: 	return _pinGet_Mt2_Enc1();
//		case _e_Inputs_Mt2Disc1: 	return _pinGet_Mt2_Disc2();
//		case _e_Inputs_Mt2Disc2: 	return _pinGet_Mt2_Disc1();
		case _e_Inputs_UrnDet: 		return _pinGet_Urn_Det();
		default: 									return 0;
	}
}

void inputs_motor_get( tInMt mtNum, bool* retEnc2, bool* retEnc1, bool* retDisc1, bool* retDisc2 )
{
	if( mtNum == _e_InMt_Pri ){
		*retEnc2 	= sInPin[_e_Inputs_Mt1Enc2].state;
		//*retSen 	= sInPin[_e_Inputs_Mt1Enc].state;
		*retEnc1 	= sInPin[_e_Inputs_Mt1Enc1].state;
		*retDisc1   = sInPin[_e_Inputs_Mt1Disc1].state;
		*retDisc2   = sInPin[_e_Inputs_Mt1Disc2].state;
	}
	else{
		*retEnc2 	= sInPin[_e_Inputs_Mt2Enc2].state;
		//*retSen 	= sInPin[_e_Inputs_Mt2Enc1].state;
		*retEnc1 	= sInPin[_e_Inputs_Mt2Enc1].state;
		*retDisc1   = sInPin[_e_Inputs_Mt2Disc1].state;
		*retDisc2   = sInPin[_e_Inputs_Mt2Disc2].state;
	}
}	

/*** ** * Funcs Ext * ** ***/

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch( GPIO_Pin )
	{
	case GPIO_IOs_Urn_Det_Pin:
		sInUrn.cardDetFlag = true;
		break;

	case GPIO_MT1_Enc1_Pin:
	case GPIO_MT1_Enc2_Pin:
		sInMt[0].encCmp = (bool)HAL_GPIO_ReadPin(GPIO_MT1_Enc1_Port,GPIO_MT1_Enc1_Pin) + ( (bool)HAL_GPIO_ReadPin(GPIO_MT1_Enc2_Port,GPIO_MT1_Enc2_Pin) << 1 );
		if( sInMt[0].enc != sInMt[0].encCmp ){
			sInMt[0].enc = sInMt[0].encCmp;
			Encoder_Refresh_IT( _e_EncNum_Pri , sInMt[0].enc );
		}
		break;
//	case GPIO_MT1_Enc2_Pin:
//		sInMt[0].enc = (bool)HAL_GPIO_ReadPin(GPIO_MT1_Enc1_Port,GPIO_MT1_Enc1_Pin) + ( (bool)HAL_GPIO_ReadPin(GPIO_MT1_Enc2_Port,GPIO_MT1_Enc2_Pin) << 1 );
//		break;
	case GPIO_MT2_Enc2_Pin:
	case GPIO_MT2_Enc1_Pin:
		sInMt[1].encCmp = (bool)HAL_GPIO_ReadPin(GPIO_MT2_Enc1_Port,GPIO_MT2_Enc1_Pin) + ( (bool)HAL_GPIO_ReadPin(GPIO_MT2_Enc2_Port,GPIO_MT2_Enc2_Pin) << 1 );
		if( sInMt[1].enc != sInMt[1].encCmp ){
			sInMt[1].enc = sInMt[1].encCmp;
			Encoder_Refresh_IT( _e_EncNum_Sec , sInMt[1].enc );
		}
		break;
//	case GPIO_MT2_Enc2_Pin:
//		sInMt[1].enc = (bool)HAL_GPIO_ReadPin(GPIO_MT2_Enc1_Port,GPIO_MT2_Enc1_Pin) + ( (bool)HAL_GPIO_ReadPin(GPIO_MT2_Enc2_Port,GPIO_MT2_Enc2_Pin) << 1 );
//		break;

	}

}

void Inputs_Init(void)
{
	HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
	HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void Inputs_IT(void)//Agora está dentro do Inputs_Task
{
	for( uint8_t i = 0 ; i < _e_Inputs_QTY ; i++ )
	{
		if ( inputs_get( (tInputs)i ) == GPIO_PIN_RESET && sInPin[i].count > 0 ) sInPin[i].count--;
		else if	( inputs_get( (tInputs)i ) != GPIO_PIN_RESET && sInPin[i].count < inDebConst[i] ) sInPin[i].count++;

		if ( sInPin[i].count == 0 ) sInPin[i].state = false;
		else if	( sInPin[i].count >= inDebConst[i] ) sInPin[i].state = true;
	}
}	


void Inputs_Task(void)
{
	Inputs_IT();

	//Comunicacao IO
	if		( sInPin[_e_Inputs_ComIn1].state == true  && sInPin[_e_Inputs_ComIn2].state == true  ) sInCom.stateNoDeb = _e_InCom_None;
	else if	( sInPin[_e_Inputs_ComIn1].state == false && sInPin[_e_Inputs_ComIn2].state == true  ) sInCom.stateNoDeb = _e_InCom_OpenIn;
	else if	( sInPin[_e_Inputs_ComIn1].state == true  && sInPin[_e_Inputs_ComIn2].state == false ) sInCom.stateNoDeb = _e_InCom_OpenOut;
	else if	( sInPin[_e_Inputs_ComIn1].state == false && sInPin[_e_Inputs_ComIn2].state == false ) sInCom.stateNoDeb = _e_InCom_UrnPicto;
		
	if( sInCom.stateNoDeb != sInCom.stateNoDebMem )
	{
		sInCom.debCount 			= 0;
		sInCom.stateNoDebMem 	= sInCom.stateNoDeb;
		sInCom.state = _e_InCom_None;
	}

	if	( sInCom.debCount < IN_COM_DEB ) sInCom.debCount++;
	else sInCom.state = sInCom.stateNoDeb;
	
	//Motores
	for( uint8_t i = 0 ; i < 2 ; i++ )
	{
		inputs_motor_get( (tInMt)i, &sInMt[i].pinSen, &sInMt[i].pinEnc, &sInMt[i].pinDisc1, &sInMt[i].pinDisc2 );

#if(GPIO_ENCODER == GPIO_ENCODER_PULL)
		if( sInMt[i].enc != sInMt[i].pinSen + ( sInMt[i].pinEnc << 1 ) )
		{
			sInMt[i].encChange = true;
			sInMt[i].enc  = sInMt[i].pinSen + ( sInMt[i].pinEnc << 1 );
			Encoder_Refresh_IT( (tEncNum)i , sInMt[i].enc );
		}
#endif
		//Disco
		if( sInMt[i].disc != sInMt[i].pinDisc1 + ( sInMt[i].pinDisc2 << 1 ) )
		{
			sInMt[i].discChange = true;
			sInMt[i].disc = sInMt[i].pinDisc1 + ( sInMt[i].pinDisc2 << 1 );
			Encoder_Position_IT( (tEncNum)i , sInMt[i].disc );
		}

		//sInMt[i].disc = sInMt[i].pinDisc1 + ( sInMt[i].pinDisc2 << 1 );
	}
}

bool Inputs_Get( tInputs in )
{
	return sInPin[in].state;
}

tInCom 	Inputs_Com_Get(void)
{
	return sInCom.state;
}

uint8_t Inputs_Motor_Enc_Get(tInMt motorNum)
{
	return sInMt[motorNum].enc;
}

bool Inputs_Motor_EncChange_Get(tInMt motorNum)
{
	bool encChgMem = sInMt[motorNum].encChange;
	sInMt[motorNum].encChange = false;
	return encChgMem;
}

uint8_t	Inputs_Motor_Disc_Get(tInMt motorNum)
{
	return sInMt[motorNum].disc;
}

bool Inputs_Urn_CardDet( tUrnCardDet getCear )
{
	if( getCear == _e_UrnCardDet_Get )
		return sInUrn.cardDetFlag;
	else{
		//HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);///EXTI0_1_IRQn
		HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
		sInUrn.cardDetFlag = false; 
	}
	return false;
}

bool Inputs_Get_Config( tSwing_config in )
{
	switch( (uint8_t)in )
	{
		case _e_Motor_Qty_Bit1: 	return _pinGet_Config_MotorBit1();
		case _e_Motor_Qty_Bit2: 	return _pinGet_Config_MotorBit2();
		case _e_Barrier_Type: 		return _pinGet_Config_Barrier();
		case _e_Sensor_Type: 	    return _pinGet_Config_Sensor();
		default: 					return 0;
	}
}

bool Input_PinConf_Get( uint8_t pin )
{
	switch(pin)
	{
	case 0:
		if(Inputs_Get_Config(_e_Motor_Qty_Bit1))return true;
		else return false;
	case 1:
		if(Inputs_Get_Config(_e_Motor_Qty_Bit2))return true;
		else return false;
	case 2:
		if(Inputs_Get_Config(_e_Barrier_Type))return true;
		else return false;
	case 3:
		if(Inputs_Get_Config(_e_Sensor_Type))return true;
		else return false;
	}
	return false;
}

/*****************************END OF FILE****/
