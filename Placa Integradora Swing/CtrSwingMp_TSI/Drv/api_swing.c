
/**
  ******************************************************************************
  * @file    api_Swing.c
  * @author  José Faustino
  * @version V0.1
  * @date    16/11/2020
  * @brief   api_Swing Source
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) TSI.
  *
  ******************************************************************************
  */

/*** ** * Includes * ** ***/
#include <string.h>

#include "api_swing.h"
#include "protocolo_swing.h"
#include "usart.h"
#include "encoder.h"
#include "sw_config.h"
#include "task_manager.h"
#include "main.h"
#include "inputs.h"
#include "outputs.h"
#include "timing.h"
#include "position.h"
#include "sensor.h"

/* Private typedef -----------------------------------------------------------*/

/*** ** * TASK SWING * ** ***/
typedef enum
{
	_e_api_swuing_task_searchHome,
	_e_Api_Swuing_task_Home,
	_e_api_swuing_task_mode_pc,
	_e_api_swuing_task_mode_na,
	_e_api_swuing_task_mode_nf,
	_e_ApiSwuing_task_Qty

}t_ApiSwing_task;


/*** ** * TASK URNA * ** ***/
typedef enum
{
	_e_ApiUrna_task_Urna,
	_e_ApiUrna_task_Qty

}t_ApiSwUrna_task;


/*** ** * LISTA SENSORES * ** ***/
typedef enum{
	_e_Sensors_None = 0,
	_e_Sensors_In,
	_e_Sensors_Out,
	_e_Sensors_MidUp,
	_e_Sensors_MidDw,
	_e_Sensors_InMid,
	_e_Sensors_OutMid,
	_e_Sensors_InOut,
}t_ApiSw_Sensors;



typedef enum{
	_e_Sem_Acesso = 0,
	_e_Acesso_Valido_Pendente,
	_e_Acesso_Valido_Enviado,
}eStatusAcesso_t;

typedef struct sApiSwingVar {
	uint8_t u8ErrorCalibration;
	uint32_t m_u32TimeoutPolling;
	uint32_t m_u32TimeoutTrataPacket;
} sApiSwingVar_t;


typedef struct sApiSwingCmdInfo{
	uint8_t u8Buffer[PACKET_SIZE];
	uint8_t u8Quantidade;
	PortocolPack_t sInfo;
	bool bComandoRecebido;
	uint16_t u16QtdePacket;
	uint32_t u32SomaPosicao1;
	uint32_t u32SomaPosicao2;
	uint8_t u8Posicao_1_Mem;
	uint8_t u8Posicao_2_Mem;
	uint8_t configMode;

} sApiSwingCmdInfo_t;

typedef enum{
	_e_SwSenBit_Read = 0,
	_e_SwSenBit_Mem,
	_e_SwSenBit_Qty,
}tSwSenBit;

struct{
	tInCom		sendir;
	tOutCom_Pass passMem;
	bool alarm;
	bool singlePassFlag;
	union{
		uint8_t	senGroup;
		struct{
			uint8_t	in 	: 1;
			uint8_t	mU	: 1;
			uint8_t	mD	: 1;
			uint8_t	out	: 1;
			uint8_t	bla	: 4;
		}senbit;
	}senByte[_e_SwSenBit_Qty];

}sSwingSen;

typedef struct {
	uint8_t posicEnt;
	uint8_t posicSaida;

}sApiSwuingAcess_t;


struct{
	uint8_t stepBack;
	uint8_t stepPrevious;
}sApiSwTask;

typedef enum{
	_e_ApiSwUartFlag_None,
	_e_ApiSwUartFlag_PackOk,
	_e_ApiSwUartFlag_PackNok,
}tApiSwUartFlag;

tApiSwUartFlag u8ReceiveRx = _e_ApiSwUartFlag_None;
//uint8_t SensorStsDebug = 0;

/* Private define ------------------------------------------------------------*/

/*** ** * Defines * ** ***/

#define API_SWING 	                  _e_TaskManager_ApiSwing

#define API_URNA 	                  _e_TaskManager_SwUrn

#define MOTOR_CTRL1                   _e_TaskManager_MotorCtrl1
#define MOTOR_CTRL2                   _e_TaskManager_MotorCtrl2
#define MOTOR_CTRLX                   _e_TaskManager_MotorCtrlX

#define TRATA_PACKET                  _e_TaskManager_TratPecket

#define APISW_ERROR_CALIB_QTY         2
#define APISW_BUFFER_CIRC_SIZE        1
#define APISW_MEDIA_ANG_MOTOR         5
#define APISW_QTY_MOTOR               2

#define APISW_POSIC_ANG_0             0
#define APISW_POSIC_ANG_OFFSET        2
#define APISW_POSIC_ANG_BURLA         5
#define APISW_POSIC_ANG_90            100
#define APISW_POSIC_ANG_180           100
#define APISW_POSIC_ANG_90_MODO_PC    100
#define APISW_POSIC_ANG_90_MODO_NA    100
#define APISW_POSIC_ANG_90_MODO_NF    100

#define TIMEOUT_CLR                     0
#define TIMEOUT_BURLA 				  500
#define TIMEOUT_PASSOK                100 // Tempo para indicar retorno para o codin.
#define TIMEOUT_POOLING_CAMERA        5000
#define TIMEOUT_TRATA_PACKET          50

#define SWING_URN_TIMEOUT             4000
#define SWING_URN_SOLTIME             2000

#define APISWING_KEEP_CLOSE	          500

#if(SW_OPERATION_BARRIER == SW_OPERATION_METALIC_BARRIER)
	#define APISWING_DELAY_CLOSE	      7000
#elif(SW_OPERATION_BARRIER == SW_OPERATION_BIG_BARRIER)
	#define APISWING_DELAY_CLOSE	      7000
#elif(SW_OPERATION_BARRIER == SW_OPERATION_MEDIUM_L_BARRIER)
	#define APISWING_DELAY_CLOSE	      500
#elif(SW_OPERATION_BARRIER == SW_OPERATION_MEDIUM_BARRIER)
	#define APISWING_DELAY_CLOSE	      1000
#elif(SW_OPERATION_BARRIER == SW_OPERATION_SMALL_BARRIER)
	#define APISWING_DELAY_CLOSE	      500 //1000
#else
	#define APISWING_DELAY_CLOSE	      1000
#endif

#define APISWING_TIMEOUT_WAIT_ACCESS      15000 // Tempo máximo de acesso

#define APISW_CMD_MOVE_ENTRADA       'E'
#define APISW_CMD_MOVE_SAIDA         'S'

/*** ** * Defines * ** ***/

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/*** ** * Variables * ** ***/

sApiSwingCmdInfo_t sApiSwingCmdInfo = {0};
sApiSwingVar_t sApiSwingVar = {0};

uint8_t u8BufferCircPosic[SW_MOTOR_QTY][APISW_BUFFER_CIRC_SIZE] = {0};

bool flagBurlaArmisHc = false;

void setBurlaArmisHc(void)
{
	flagBurlaArmisHc = true;
}

void clrBurlaArmisHc(void)
{
	flagBurlaArmisHc = false;
	Outputs_Com_Cheat( _e_OutCom_Cheat_None );
}

bool getBurlaArmisHc(void)
{
	return flagBurlaArmisHc;
}

/* Private function prototypes -----------------------------------------------*/
static void _Api_Swing_RenewTimeoutCamera(void);

static void _api_swing_task_searchHome(void);
static void _Api_Swing_task_Home(void);

static void _Api_Swing_task_Trata_Packet(void);
static void _api_swing_task_mode_pc(void);
static void _api_swing_task_mode_na(void);
static void _api_swing_task_mode_nf(void);

static void _Api_SwingUrna_task(void);

uint8_t Api_Swing_get_sensor(void);
bool Api_Swing_alarm(void);

tApiSwUartFlag Protocolo_Flag_Get(void) ;

/* Private user code ---------------------------------------------------------*/

void Api_Swing_Rx_CallBack( uint8_t * p_pu8Buffer) {
	if(Package_Dismount(p_pu8Buffer, PACKET_SIZE, &sApiSwingCmdInfo.sInfo) == 0) {
		u8ReceiveRx = _e_ApiSwUartFlag_PackOk;
	}
	else
	{
		u8ReceiveRx = _e_ApiSwUartFlag_PackNok;
	}
}

tApiSwUartFlag Protocolo_Flag_Get(void) {
	tApiSwUartFlag u8Mem = u8ReceiveRx;
	u8ReceiveRx = _e_ApiSwUartFlag_None;
	return(u8Mem);
}


static void _Api_Swing_RenewTimeoutCamera(void)
{
	sApiSwingVar.m_u32TimeoutPolling = HAL_GetTick() + TIMEOUT_POOLING_CAMERA;
}


/*** Exported Functions ***/
typedef enum{
	_e_Api_Swing_Task_SerchHome_init,
	_e_Api_Swing_Task_SerchHome_calib,
	_e_Api_Swing_Task_SerchHome_resp_calib,
	_e_Api_Swing_Task_SerchHome_mode_operation,
	_e_Api_Swing_Task_SerchHome_calib_qty_error,
	_e_Api_Swing_Task_SerchHome_task_home,
	_e_Api_Swing_Task_SerchHome_error_calibration,
	_e_Api_Swing_Task_SerchHome_sensor_ativado,

}eApiSwingStep_t;

#if 0 //CONFIG_SWING_GPIO_HMI_BUS
swingConfig_t swingConfig;

#endif


static void _api_swing_task_searchHome(void)
{

	TaskManager_Set( API_SWING , _e_api_swuing_task_searchHome );

	if( TaskManager_First_Get( API_SWING ) == false )
	{
		TaskManager_First_Set( API_SWING );
		TaskManager_Step_Set( API_SWING, 0 );
		Timing_Set( _e_Timing_Swing_AlarmDeb , 500 );
		//Outputs_Buzz_Set( _e_OutBuzz_Off );

		_Api_Swing_RenewTimeoutCamera();

		SWING_ENABLE_CONFIG;

		sApiSwingVar.u8ErrorCalibration = 0;
	}

	if( Api_Swing_alarm() == false )
	{
#if(SW_PICTOGRAMA_MODE == SW_PICTOGRAMA_FITA_LED)
		Outputs_RGB_Set ( _e_OutPic_Both, _e_OutPicMode_Static, _e_OutPicColor_Yellow, _e_OutPicSpeed_Fast );
#else
		Outputs_RGB_Set(_e_OutPic_Both, _e_OutPicMode_Blink, _e_OutPicColor_Green, _e_OutPicSpeed_Slow);
#endif
	}

	switch( TaskManager_Step_Get( API_SWING ) )
	{

	case _e_Api_Swing_Task_SerchHome_init:
		Outputs_Buzz_Set( _e_OutBuzz_Siren );
		if( Api_Swing_alarm() != false ) {
			Outputs_RGB_Set ( _e_OutPic_Both, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
			TaskManager_Step_Set( API_SWING, _e_Api_Swing_Task_SerchHome_sensor_ativado );
		}
		else {

			TaskManager_Step_Inc( API_SWING );
		}
		break;

	case _e_Api_Swing_Task_SerchHome_calib:

		Timing_Set( _e_Timing_Swing_AlarmDeb , 500 );
		if( Api_Swing_alarm() != false )
		{
			Outputs_RGB_Set ( _e_OutPic_Both, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
			Outputs_Buzz_Set( _e_OutBuzz_Siren );
			TaskManager_Step_Set( API_SWING, _e_Api_Swing_Task_SerchHome_sensor_ativado );
		}
		else
		{

#if (SW_SWING_TESTE == SW_SWING_TESTE_DISABLE)

#if 0 //CONFIG_SWING_GPIO_HMI_BUS
			if(Input_PinConf_Get(_e_Motor_Qty_Bit1) == false) {
				if (Input_PinConf_Get(_e_Motor_Qty_Bit2) == false)
					Position_Config_Motor(_e_SwMtQty_Quad);
				else
					Position_Config_Motor(_e_SwMtQty_Jump);
			}
			else {
				if (Input_PinConf_Get(_e_Motor_Qty_Bit2) == false)
					Position_Config_Motor(_e_SwMtQty_Dual);
				else
					Position_Config_Motor(_e_SwMtQty_Single);
			}

			if(Input_PinConf_Get(_e_Barrier_Type) == _e_Barrier_Big) {

				__NOP();

				swingConfig.stpmt_mt_speed_max 					= 		14000							;
				swingConfig.stpmt_mt_acc_max 					= 		50								;
				swingConfig.stpmt_mt_speed_ini_min 				= 		100								;
				swingConfig.stpmt_mt_speed_ini_max 				= 		200								;

				swingConfig.stpmt_mt_speed_cal 					= 		10000							;
				swingConfig.stpmt_mt_acc_cal 					= 		25								;
				swingConfig.stpmt_mt_speed_ini_cal 				= 		50								;

				swingConfig.enc_move_param_speed_max[1] 		=  		swingConfig.stpmt_mt_speed_max	;
				swingConfig.enc_move_param_acc_max[1] 			=  		swingConfig.stpmt_mt_acc_max	;
				swingConfig.enc_move_param_ang_speed_max[1] 	=  		75								;
				swingConfig.enc_move_param_ramp_speed_min[1]	=  		0.5								;
				swingConfig.enc_move_param_ramp_acc_min[1] 		=  		0.5								;
				swingConfig.enc_move_param_correct_per[1] 		= 		01 								;

				swingConfig.enc_move_param_speed_max[2] 		= 		swingConfig.stpmt_mt_speed_max	;
				swingConfig.enc_move_param_acc_max[2] 			=  		swingConfig.stpmt_mt_acc_max	;
				swingConfig.enc_move_param_ang_speed_max[2] 	=  		75								;
				swingConfig.enc_move_param_ramp_speed_min[2] 	= 		0.5 							;
				swingConfig.enc_move_param_ramp_acc_min[2] 		=  		0.5								;
				swingConfig.enc_move_param_correct_per[2] 		=  		1.07							;

			}
			else {
				__NOP();
			}

			if(Input_PinConf_Get(_e_Sensor_Type) == false) {
				__NOP();
			}
			else {
				__NOP();
			}

			//SWING_DISABLE_CONFIG;

			TaskManager_Step_Inc( API_SWING );

#else  // CONFIG_SWING_GPIO_HMI_BUS


#if (SW_MOTOR_QTY == SW_MOTOR_UNIT)
			Position_Config_Motor(_e_SwMtQty_Single);
#elif (SW_MOTOR_QTY == SW_MOTOR_DUAL)
			Position_Config_Motor(_e_SwMtQty_Dual);
#elif (SW_MOTOR_QTY == SW_MOTOR_JMP)
			Position_Config_Motor(_e_SwMtQty_Jump);
#else
			Position_Config_Motor(_e_SwMtQty_Unknow);
#endif
			TaskManager_Step_Inc( API_SWING );

#endif  // CONFIG_SWING_GPIO_HMI_BUS


#else  // SW_SWING_TESTE == SW_SWING_TESTE_ENABLE
			Outputs_RGB_Set ( _e_OutPic_Both, _e_OutPicMode_Blink, _e_OutPicColor_TSIgr, _e_OutPicSpeed_Fast );
			Outputs_Buzz_Set( _e_OutBuzz_Off );
			TaskManager_Go( API_SWING , _Api_Swing_task_Home );
			return;
#endif


		}
		break;

	case _e_Api_Swing_Task_SerchHome_resp_calib:
		Timing_Set( _e_Timing_Swing_AlarmDeb , 500 );

		if ( Position_Status() == _e_PosSts_WaitCfgMode)
		{
			//TaskManager_Step_Set( API_SWING, _e_Api_Swing_Task_SerchHome_mode_operation );
			TaskManager_Step_Inc( API_SWING );
		}
		break;

	case _e_Api_Swing_Task_SerchHome_mode_operation:

#if (SW_OPERATION_MODE == SW_OPERATION_MODE_PC)
		if ( Position_Config_Mode(_e_PosModCfg_PC) == _e_PosRet_Ok) {
			TaskManager_Step_Set( API_SWING, _e_Api_Swing_Task_SerchHome_task_home );
		}
		else {
			sApiSwingVar.u8ErrorCalibration++;
			if ( sApiSwingVar.u8ErrorCalibration >= APISW_ERROR_CALIB_QTY ) {
				TaskManager_Step_Set( API_SWING, _e_Api_Swing_Task_SerchHome_calib_qty_error );
			}
			else {
				TaskManager_Step_Set( API_SWING, _e_Api_Swing_Task_SerchHome_calib );
			}
		}

#elif (SW_OPERATION_MODE == SW_OPERATION_MODE_NA)
		if ( Position_Config_Mode(_e_PosModCfg_NA) == _e_PosRet_Ok) {
			Outputs_Buzz_Set( _e_OutBuzz_Off );
			TaskManager_Go(API_SWING, _api_swing_task_mode_na);
		}
		else {
			sApiSwingVar.u8ErrorCalibration++;
			if ( sApiSwingVar.u8ErrorCalibration >= APISW_ERROR_CALIB_QTY ) {
				TaskManager_Step_Set( API_SWING, _e_Api_Swing_Task_SerchHome_calib_qty_error );
			}
			else {
				TaskManager_Step_Set( API_SWING, _e_Api_Swing_Task_SerchHome_calib );
			}
		}
#elif (SW_OPERATION_MODE == SW_OPERATION_MODE_NF)
		if ( Position_Config_Mode(_e_PosModCfg_NF) == _e_PosRet_Ok)
		{
			Outputs_Buzz_Set( _e_OutBuzz_Off );
			TaskManager_Go(API_SWING, _api_swing_task_mode_nf);
		}
		else
		{
			sApiSwingVar.u8ErrorCalibration++;
			if ( sApiSwingVar.u8ErrorCalibration >= APISW_ERROR_CALIB_QTY )
			{
				TaskManager_Step_Set( API_SWING, _e_Api_Swing_Task_SerchHome_calib_qty_error );
			}
			else
			{
				TaskManager_Step_Set( API_SWING, _e_Api_Swing_Task_SerchHome_calib );
			}
		}
#endif
		break;

	case _e_Api_Swing_Task_SerchHome_task_home:

		Outputs_RGB_Set ( _e_OutPic_Both, _e_OutPicMode_Blink, _e_OutPicColor_TSIgr, _e_OutPicSpeed_Fast );
		Outputs_Buzz_Set( _e_OutBuzz_Off );
		_Api_Swing_RenewTimeoutCamera();
		TaskManager_Go( API_SWING , _Api_Swing_task_Home );
		break;

	case _e_Api_Swing_Task_SerchHome_calib_qty_error:
		Outputs_Buzz_Set( _e_OutBuzz_Off );
		Timing_Set( _e_Timing_Swing_AlarmDeb , 500 );
		Timing_Set( _e_Timing_Motor_calibration, 50000 );
		Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Slow );
		TaskManager_Step_Set( API_SWING, _e_Api_Swing_Task_SerchHome_error_calibration );
		break;

	case _e_Api_Swing_Task_SerchHome_error_calibration:
		if( Timing_Get_Bool( _e_Timing_Motor_calibration ) == true )
		{
			sApiSwingVar.u8ErrorCalibration = 0;
			Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Blink, _e_OutPicColor_Yellow, _e_OutPicSpeed_Fast );
			TaskManager_Step_Set( API_SWING, _e_Api_Swing_Task_SerchHome_init );
		}
		break;

	case _e_Api_Swing_Task_SerchHome_sensor_ativado:

		if( Api_Swing_alarm() == false )
		{
			Outputs_RGB_Set ( _e_OutPic_Both, _e_OutPicMode_Blink, _e_OutPicColor_Yellow, _e_OutPicSpeed_Fast );
			Outputs_Buzz_Set( _e_OutBuzz_Off );
			TaskManager_Step_Set( API_SWING, _e_Api_Swing_Task_SerchHome_init );
		}
	break;

	default:
		break;
	}
}

static void _Api_Swing_task_Home(void)
{
	TaskManager_Set( API_SWING, _e_Api_Swuing_task_Home );

	if( TaskManager_First_Get( API_SWING ) == false )
	{
		TaskManager_First_Set ( API_SWING );
		TaskManager_Step_Set ( API_SWING, 0 );
	}

	switch( (uint8_t)TaskManager_Step_Get( API_SWING ) )
	{
		case 0:
		{
			if ( sApiSwingCmdInfo.bComandoRecebido == true ) // Se chegou msg do pc
			{
				_Api_Swing_RenewTimeoutCamera();
				sApiSwingCmdInfo.bComandoRecebido = false;
				TaskManager_Step_Set( API_SWING, 3 );
			}

			// Verificar timeout e mudar estado caso tempo tenha estourado
			if(HAL_GetTick() > sApiSwingVar.m_u32TimeoutPolling)
			{
				TaskManager_Step_Set( API_SWING, 1 );
			}
			break;
		}

		case 1:
		{

			if ( sApiSwingCmdInfo.bComandoRecebido == true )
			{
				_Api_Swing_RenewTimeoutCamera();
				if ( Position_Config_Mode(_e_PosModCfg_PC) == _e_PosRet_Ok) {
					TaskManager_Go(API_SWING, _api_swing_task_mode_pc);
					TaskManager_Step_Set( API_SWING, 0 );
				}
				else {
					TaskManager_Step_Set( API_SWING, 6 );
					return;
				}
			}
			else {
				TaskManager_Step_Set( API_SWING, 4 );
			}

			break;
		}

		case 2: // Idle
		{

			if( Api_Swing_alarm() == false ){
				if( Inputs_Urn_CardDet(_e_UrnCardDet_Get) == true ) {
					Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Blink, _e_OutPicColor_Blue, _e_OutPicSpeed_Slow );
				}
				else {
					if( Timing_Get_Bool( _e_Timing_Swing_Picto_RGB ) == true)
					{
						Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Static, _e_OutPicColor_TSIgr, _e_OutPicSpeed_Slow );
					}
				}
			}

			TaskManager_Step_Set( API_SWING, 1 );
			break;
		}

		case 3: //Comando recebido
		{
			TaskManager_Go( API_SWING , _api_swing_task_mode_pc );
			TaskManager_Step_Set( API_SWING, 0 );
			break;
		}

		case 4: //mode NA
		{
			if ( Position_Config_Mode(_e_PosModCfg_NA) == _e_PosRet_Ok) {
				TaskManager_Go(API_SWING, _api_swing_task_mode_na);
			}
			else {
				TaskManager_Step_Set( API_SWING, 5 );
			}
			break;
		}

		case 5: //mode NA
		{
#if (SW_SWING_TESTE == SW_SWING_TESTE_ENABLE )
			TaskManager_Go(API_SWING, _api_swing_task_mode_na);
			return;
#endif
			if ( Position_Status() == _e_PosSts_ModeNaRunning ) {
				TaskManager_Go(API_SWING, _api_swing_task_mode_na);
			}
			break;
		}

		case 6: //mode PC
		{
#if (SW_SWING_TESTE == SW_SWING_TESTE_ENABLE )
			TaskManager_Go(API_SWING, _api_swing_task_mode_pc);
			return;
#endif
			if ( Position_Status() == _e_PosSts_ModePcRunning ) {
				TaskManager_Go(API_SWING, _api_swing_task_mode_pc);
			}
			else {
				TaskManager_Step_Set( API_SWING, 0 );
			}
			break;
		}

		default:
			TaskManager_Step_Set( API_SWING, 0 );
			break;
	}

}

/*** ** * TASK PACKET PC * ** ***/

struct{
	uint32_t OK;
	uint32_t ERR;
}sApiSwPcdbg;

typedef enum
{
	_e_Api_Swuing_task_Trata_Packet,

}t_ApiSwing_Packet_PC_task;

static void _Api_Swing_task_Trata_Packet(void)
{
	//uint8_t u8Count;
	static eStatusAcesso_t eAcessoEntrada;
	static eStatusAcesso_t eAcessoSaida;

	tApiSwUartFlag packet;

	TaskManager_Set( TRATA_PACKET, _e_Api_Swuing_task_Trata_Packet );

	if( TaskManager_First_Get( TRATA_PACKET ) == false )
	{
		TaskManager_First_Set ( TRATA_PACKET );
		TaskManager_Step_Set ( TRATA_PACKET, 0 );

		sApiSwingCmdInfo.u16QtdePacket   = 0;
		sApiSwingCmdInfo.u32SomaPosicao1 = 0;
		sApiSwingCmdInfo.u32SomaPosicao2 = 0;
		eAcessoEntrada = _e_Sem_Acesso;
		eAcessoSaida = _e_Sem_Acesso;
	}
	switch( (uint8_t)TaskManager_Step_Get( TRATA_PACKET ) ) {

		case 0:
			switch((uint8_t)Inputs_Com_Get())
			{
				case _e_InCom_OpenIn:
					if(eAcessoEntrada == _e_Sem_Acesso)
						eAcessoEntrada = _e_Acesso_Valido_Pendente;
					break;
				case _e_InCom_OpenOut:
					if(eAcessoSaida == _e_Sem_Acesso)
						eAcessoSaida = _e_Acesso_Valido_Pendente;
					break;
				default:
					if ( eAcessoEntrada == _e_Acesso_Valido_Enviado ) {
						eAcessoEntrada = _e_Sem_Acesso;
					}
					if ( eAcessoSaida == _e_Acesso_Valido_Enviado ) {
						eAcessoSaida = _e_Sem_Acesso;
					}
					break;
			}
			TaskManager_Step_Inc(TRATA_PACKET);
			break;

		case 1:

//			if(USART_CheckPendinPacket())
			{
//				USART_GetPacket(sApiSwingCmdInfo.u8Buffer, (unsigned int*) &sApiSwingCmdInfo.u8Quantidade);

				//if(Package_Dismount(sApiSwingCmdInfo.u8Buffer, sApiSwingCmdInfo.u8Quantidade, &sApiSwingCmdInfo.sInfo) == 0)
				packet = Protocolo_Flag_Get() ;

				if(packet == _e_ApiSwUartFlag_PackOk )
				{
					sApiSwPcdbg.OK++;

					if(eAcessoEntrada == _e_Acesso_Valido_Pendente)
					{
						sApiSwingCmdInfo.sInfo.u8AcessoIn = 1;
						//eAcessoEntrada = _e_Acesso_Valido_Enviado;
					}

					if(eAcessoSaida == _e_Acesso_Valido_Pendente)
					{
						sApiSwingCmdInfo.sInfo.u8AcessoOut = 1;
						//eAcessoSaida = _e_Acesso_Valido_Enviado;
					}

					sApiSwingCmdInfo.u8Quantidade = Package_Mount(sApiSwingCmdInfo.u8Buffer, sizeof(sApiSwingCmdInfo.u8Buffer), sApiSwingCmdInfo.sInfo);



#if 0
					if(sApiSwingCmdInfo.u8Buffer[INDEX_POSICAO_1]<=100)
						u8BufferCircPosic[_e_SwMtNum_Pri][sApiSwingCmdInfo.u16QtdePacket] = sApiSwingCmdInfo.u8Buffer[INDEX_POSICAO_1];
					if(sApiSwingCmdInfo.u8Buffer[INDEX_POSICAO_2]<=100)
						u8BufferCircPosic[_e_SwMtNum_Sec][sApiSwingCmdInfo.u16QtdePacket] = sApiSwingCmdInfo.u8Buffer[INDEX_POSICAO_2];

					sApiSwingCmdInfo.u16QtdePacket++;

					if(sApiSwingCmdInfo.u16QtdePacket > APISW_BUFFER_CIRC_SIZE )
					{
						sApiSwingCmdInfo.u16QtdePacket = 0;
					}

					sApiSwingCmdInfo.u32SomaPosicao1 = 0;
					sApiSwingCmdInfo.u32SomaPosicao2 =0;

					for(u8Count = 0; u8Count <  APISW_BUFFER_CIRC_SIZE; u8Count++)
					{
						sApiSwingCmdInfo.u32SomaPosicao1 += u8BufferCircPosic[_e_SwMtNum_Pri][u8Count];
						sApiSwingCmdInfo.u32SomaPosicao2 += u8BufferCircPosic[_e_SwMtNum_Sec][u8Count];
					}

					sApiSwingCmdInfo.sInfo.u8Posicao_1 = sApiSwingCmdInfo.u32SomaPosicao1 / APISW_BUFFER_CIRC_SIZE;
					sApiSwingCmdInfo.sInfo.u8Posicao_2 = sApiSwingCmdInfo.u32SomaPosicao2 / APISW_BUFFER_CIRC_SIZE;

#endif

					sApiSwingCmdInfo.bComandoRecebido = true;

					if ( (eAcessoEntrada == _e_Sem_Acesso) && (eAcessoSaida == _e_Sem_Acesso) ) {
						sApiSwingCmdInfo.sInfo.u8AcessoIn  = 0;
						sApiSwingCmdInfo.sInfo.u8AcessoOut = 0;
						TaskManager_Step_Set ( TRATA_PACKET, 3 );
					}
					else {
						TaskManager_Step_Set ( TRATA_PACKET, 2 );
					}

					Usart_Set_Tx_Buff(sApiSwingCmdInfo.u8Buffer, sApiSwingCmdInfo.u8Quantidade);

				}
				else if(packet == _e_ApiSwUartFlag_PackNok )
				{
					sApiSwPcdbg.ERR++;
					sApiSwingCmdInfo.sInfo.u8Posicao_1 = sApiSwingCmdInfo.sInfo.u8Sentido_1 = sApiSwingCmdInfo.sInfo.u8Velocidade_1 = 0xFF;
					sApiSwingCmdInfo.sInfo.u8Posicao_2 = sApiSwingCmdInfo.sInfo.u8Sentido_2 = sApiSwingCmdInfo.sInfo.u8Velocidade_2 = 0xFF;
					sApiSwingCmdInfo.sInfo.u8AcessoIn = 0;
					sApiSwingCmdInfo.sInfo.u8AcessoOut = 0;
					sApiSwingCmdInfo.u8Quantidade = Package_Mount(sApiSwingCmdInfo.u8Buffer, sizeof(sApiSwingCmdInfo.u8Buffer), sApiSwingCmdInfo.sInfo);

					Usart_Set_Tx_Buff(sApiSwingCmdInfo.u8Buffer, sApiSwingCmdInfo.u8Quantidade);
				}

				//USART_EnviaBuffer(sApiSwingCmdInfo.u8Buffer, sApiSwingCmdInfo.u8Quantidade);
			}

			break;

		case 2:

//			if(USART_CheckPendinPacket())
			{
//				USART_GetPacket(sApiSwingCmdInfo.u8Buffer, (unsigned int*) &sApiSwingCmdInfo.u8Quantidade);

				//if(Package_Dismount(sApiSwingCmdInfo.u8Buffer, sApiSwingCmdInfo.u8Quantidade, &sApiSwingCmdInfo.sInfo) == 0)
				packet = Protocolo_Flag_Get() ;

				if(packet == _e_ApiSwUartFlag_PackOk )
				{
					sApiSwPcdbg.OK++;


					if(eAcessoEntrada == _e_Acesso_Valido_Pendente)
					{
						if(sApiSwingCmdInfo.u8Buffer[INDEX_ACESSO_IN] == 0) {
							sApiSwingCmdInfo.sInfo.u8AcessoIn = 1;
							eAcessoEntrada = _e_Acesso_Valido_Pendente;
						}
						else {
							sApiSwingCmdInfo.sInfo.u8AcessoIn = 0;
							eAcessoEntrada = _e_Acesso_Valido_Enviado;
							TaskManager_Step_Set ( TRATA_PACKET, 3 );
						}
					}
					else {
							sApiSwingCmdInfo.sInfo.u8AcessoIn = 0;
							eAcessoEntrada = _e_Acesso_Valido_Enviado;
							TaskManager_Step_Set ( TRATA_PACKET, 3 );
					}
					if(eAcessoSaida == _e_Acesso_Valido_Pendente)
					{

						if(sApiSwingCmdInfo.u8Buffer[INDEX_ACESSO_OUT] == 0) {
							sApiSwingCmdInfo.sInfo.u8AcessoOut = 1;
							eAcessoSaida = _e_Acesso_Valido_Pendente;
						}
						else {
							sApiSwingCmdInfo.sInfo.u8AcessoOut = 0;
							eAcessoSaida = _e_Acesso_Valido_Enviado;
							TaskManager_Step_Set ( TRATA_PACKET, 3 );
						}
					}
					else {
							sApiSwingCmdInfo.sInfo.u8AcessoOut = 0;
							eAcessoSaida = _e_Acesso_Valido_Enviado;
							TaskManager_Step_Set ( TRATA_PACKET, 3 );
					}

					sApiSwingCmdInfo.u8Quantidade = Package_Mount(sApiSwingCmdInfo.u8Buffer, sizeof(sApiSwingCmdInfo.u8Buffer), sApiSwingCmdInfo.sInfo);


#if 0
					if(sApiSwingCmdInfo.u8Buffer[INDEX_POSICAO_1]<=100)
						u8BufferCircPosic[_e_SwMtNum_Pri][sApiSwingCmdInfo.u16QtdePacket] = sApiSwingCmdInfo.u8Buffer[INDEX_POSICAO_1];
					if(sApiSwingCmdInfo.u8Buffer[INDEX_POSICAO_2]<=100)
						u8BufferCircPosic[_e_SwMtNum_Sec][sApiSwingCmdInfo.u16QtdePacket] = sApiSwingCmdInfo.u8Buffer[INDEX_POSICAO_2];

					sApiSwingCmdInfo.u16QtdePacket++;

					sApiSwingCmdInfo.u32SomaPosicao1 = 0;
					sApiSwingCmdInfo.u32SomaPosicao2 = 0;

					if(sApiSwingCmdInfo.u16QtdePacket > APISW_BUFFER_CIRC_SIZE )
					{
						sApiSwingCmdInfo.u16QtdePacket = 0;
					}

					for(u8Count = 0; u8Count < APISW_BUFFER_CIRC_SIZE; u8Count++)
					{
						sApiSwingCmdInfo.u32SomaPosicao1 += u8BufferCircPosic[_e_SwMtNum_Pri][u8Count];
						sApiSwingCmdInfo.u32SomaPosicao2 += u8BufferCircPosic[_e_SwMtNum_Sec][u8Count];
					}

					sApiSwingCmdInfo.sInfo.u8Posicao_1 = sApiSwingCmdInfo.u32SomaPosicao1 / APISW_BUFFER_CIRC_SIZE;
					sApiSwingCmdInfo.sInfo.u8Posicao_2 = sApiSwingCmdInfo.u32SomaPosicao2 / APISW_BUFFER_CIRC_SIZE;

#endif

					sApiSwingCmdInfo.bComandoRecebido = true;

					Usart_Set_Tx_Buff(sApiSwingCmdInfo.u8Buffer, sApiSwingCmdInfo.u8Quantidade);

					//TaskManager_Step_Set ( TRATA_PACKET, 2 );

				}
				else if(packet == _e_ApiSwUartFlag_PackNok )
				{
					sApiSwPcdbg.ERR++;
					sApiSwingCmdInfo.sInfo.u8Posicao_1 = sApiSwingCmdInfo.sInfo.u8Sentido_1 = sApiSwingCmdInfo.sInfo.u8Velocidade_1 = 0xFF;
					sApiSwingCmdInfo.sInfo.u8Posicao_2 = sApiSwingCmdInfo.sInfo.u8Sentido_2 = sApiSwingCmdInfo.sInfo.u8Velocidade_2 = 0xFF;
					sApiSwingCmdInfo.sInfo.u8AcessoIn = 0;
					sApiSwingCmdInfo.sInfo.u8AcessoOut = 0;
					sApiSwingCmdInfo.u8Quantidade = Package_Mount(sApiSwingCmdInfo.u8Buffer, sizeof(sApiSwingCmdInfo.u8Buffer), sApiSwingCmdInfo.sInfo);

					Usart_Set_Tx_Buff(sApiSwingCmdInfo.u8Buffer, sApiSwingCmdInfo.u8Quantidade);
				}

//				USART_EnviaBuffer(sApiSwingCmdInfo.u8Buffer, sApiSwingCmdInfo.u8Quantidade);
			}

			break;

		case 3:
			TaskManager_Step_Set ( TRATA_PACKET, 0 );
			break;

		default:

			break;
	}

}

/*** ** * TASK MODE PC * ** ***/
typedef enum{
	_e_api_swing_task_mode_pc_idle,
	_e_api_swing_task_mode_pc_close,
	_e_api_swing_task_mode_pc_entrance,
	_e_api_swing_task_mode_pc_waint_entrance,
	_e_api_swing_task_mode_pc_exit,
	_e_api_swing_task_mode_pc_waint_exit,
	_e_api_swing_task_mode_pc_access_ok,
	_e_api_swing_task_mode_pc_buzzer_beep,
	_e_api_swing_task_mode_pc_buzzer_sirene,
}eApiSwStepModoPc_t;

static void _api_swing_task_mode_pc(void)
{
	tSensorSts SensorSts = _e_SensorSts_None;
	tSensorSts SensorStsPass = _e_SensorSts_PassWait;

	TaskManager_Set( API_SWING, _e_api_swuing_task_mode_pc );

	if( TaskManager_First_Get( API_SWING ) == false )
	{
		TaskManager_First_Set( API_SWING );
		TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_pc_idle );

		Sensor_Set( _e_SensorMode_Alarm );
	}

	switch( (uint8_t)TaskManager_Step_Get( API_SWING ) )
	{
		case _e_api_swing_task_mode_pc_idle:

			Sensor_Set( _e_SensorMode_Alarm );
			SensorSts = Sensor_Get();

			if( Inputs_Com_Get() == _e_InCom_OpenIn  ) {
				TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_pc_entrance);
			}
			if( Inputs_Com_Get() == _e_InCom_OpenOut ) {
				TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_pc_exit );
			}

			switch(SensorSts)
			{
				case _e_SensorSts_AlarmEntrance:
					Outputs_RGB_Set( _e_OutPic_In, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_pc_close);

					break;
				case _e_SensorSts_AlarmExit:
					Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_pc_close);
					break;
				case _E_SensorSts_AlarmMid:
					Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
					Outputs_Buzz_Set( _e_OutBuzz_Siren );
					Timing_Set(_e_Timing_Swing_Beep, 500);

					sApiSwTask.stepBack = _e_api_swing_task_mode_pc_idle;
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_pc_buzzer_sirene );
					break;

				default:
					if (  sApiSwingCmdInfo.sInfo.u8Posicao_1 >= APISW_POSIC_ANG_BURLA ) {
						Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
					}
					if (sApiSwingCmdInfo.sInfo.u8Posicao_2 >= APISW_POSIC_ANG_BURLA ) {
						Outputs_RGB_Set( _e_OutPic_In, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
					}
					else if ( (sApiSwingCmdInfo.sInfo.u8Posicao_1 < APISW_POSIC_ANG_BURLA ) && (sApiSwingCmdInfo.sInfo.u8Posicao_2 < APISW_POSIC_ANG_BURLA )) {
						Outputs_Buzz_Set( _e_OutBuzz_Off);
						Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Static, _e_OutPicColor_TSIgr, _e_OutPicSpeed_Fast );

						if ( ( Inputs_Com_Get() != _e_InCom_OpenIn  ) && (Inputs_Com_Get() != _e_InCom_OpenOut ) ) {
							Outputs_Buzz_Set( _e_OutBuzz_Off );
							Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Static, _e_OutPicColor_TSIgr, _e_OutPicSpeed_Fast );
							TaskManager_Go( API_SWING , _Api_Swing_task_Home );
						}
					}
					break;
			}

			break;

		case _e_api_swing_task_mode_pc_close:
			Outputs_Com_Cheat( _e_OutCom_Cheat_Shit );
			Outputs_Buzz_Set( _e_OutBuzz_Off );
			TaskManager_Go( API_SWING , _Api_Swing_task_Home );

			break;

		case _e_api_swing_task_mode_pc_entrance:

			Sensor_Set( _e_SensorMode_Entrance );

			Outputs_RGB_Set( _e_OutPic_In, _e_OutPicMode_Blink, _e_OutPicColor_Green, _e_OutPicSpeed_Fast );
			Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
			Outputs_Com_Cheat( _e_OutCom_Cheat_None );
			Timing_Set( _e_Timing_Swing_CloseDelay, APISWING_TIMEOUT_WAIT_ACCESS );

			sApiSwTask.stepBack = _e_api_swing_task_mode_pc_waint_entrance;
			TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_pc_buzzer_beep );
			break;

		case _e_api_swing_task_mode_pc_waint_entrance:

			SensorStsPass = Sensor_Get();

			if( Inputs_Com_Get() != _e_InCom_OpenIn  	&&
				Inputs_Com_Get() != _e_InCom_OpenOut 	&&
				Timing_Get_Bool( _e_Timing_Swing_CloseDelay ) == true )
			{
				Outputs_Com_Pass( _e_OutCom_Pass_Nop );
				Outputs_Buzz_Set( _e_OutBuzz_Off );
				TaskManager_Go( API_SWING , _Api_Swing_task_Home );
				return;
			}

			switch( SensorStsPass )
			{
				case _e_SensorSts_PassOk:
					Outputs_RGB_Set( _e_OutPic_Both,  _e_OutPicMode_Blink, _e_OutPicColor_Pink, _e_OutPicSpeed_Fast );
					Timing_Set( _e_Timing_Swing_Picto_RGB , 1000 );
					Outputs_Com_Pass( _e_OutCom_Pass_OK );
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_pc_access_ok );

					break;
				case _e_SensorSts_AlarmExit:
					Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
					Outputs_Buzz_Set( _e_OutBuzz_Siren );
					break;
				case _E_SensorSts_AlarmMid:
					Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
					Outputs_Buzz_Set( _e_OutBuzz_Siren );
					break;

				default:

				break;
			}

			break;

		case _e_api_swing_task_mode_pc_exit:
			Sensor_Set( _e_SensorMode_Exit );

			Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Green, _e_OutPicSpeed_Fast );
			Outputs_RGB_Set( _e_OutPic_In, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
			Outputs_Com_Cheat( _e_OutCom_Cheat_None );
			Timing_Set( _e_Timing_Swing_CloseDelay, APISWING_TIMEOUT_WAIT_ACCESS );
			Outputs_Buzz_Set( _e_OutBuzz_Beep );
			Timing_Set(_e_Timing_Swing_Beep, 500);
			TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_pc_buzzer_beep );
			sApiSwTask.stepBack = _e_api_swing_task_mode_pc_waint_exit;

			break;

		case _e_api_swing_task_mode_pc_waint_exit:
			if( Inputs_Com_Get() != _e_InCom_OpenIn  	&&
				Inputs_Com_Get() != _e_InCom_OpenOut 	&&
				Timing_Get_Bool( _e_Timing_Swing_CloseDelay ) == true )
			{
				Outputs_Com_Pass( _e_OutCom_Pass_Nop );
				Outputs_Buzz_Set( _e_OutBuzz_Off );
				TaskManager_Go( API_SWING , _Api_Swing_task_Home );
				return;
			}

			SensorStsPass = Sensor_Get();
			switch( SensorStsPass )
			{
				case _e_SensorSts_PassOk:
					Outputs_RGB_Set( _e_OutPic_Both,  _e_OutPicMode_Blink, _e_OutPicColor_Pink, _e_OutPicSpeed_Fast );
					Timing_Set( _e_Timing_Swing_Picto_RGB , 1000 );
					Outputs_Com_Pass( _e_OutCom_Pass_OK );
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_pc_access_ok );

					break;
				case _e_SensorSts_AlarmEntrance:
					Outputs_Buzz_Set( _e_OutBuzz_Siren );
					Outputs_RGB_Set( _e_OutPic_In, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
					break;
				case _E_SensorSts_AlarmMid:
					Outputs_Buzz_Set( _e_OutBuzz_Siren );
					Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
					break;

				default:

				break;
			}

			break;

			case _e_api_swing_task_mode_pc_access_ok:
				if( Inputs_Com_Get() != _e_InCom_OpenIn &&
					Inputs_Com_Get() != _e_InCom_OpenOut ) {
					Outputs_Com_Pass( _e_OutCom_Pass_Nop );
					Outputs_Buzz_Set( _e_OutBuzz_Off );
					TaskManager_Go( API_SWING , _Api_Swing_task_Home );
				}
				break;


			case _e_api_swing_task_mode_pc_buzzer_beep:
				if( Timing_Get_Bool(_e_Timing_Swing_Beep) == true)
				{
					Outputs_Buzz_Set( _e_OutBuzz_Beep );
					Timing_Set(_e_Timing_Swing_Beep, 500);
				}
				TaskManager_Step_Set( API_SWING, sApiSwTask.stepBack );
				break;

			case _e_api_swing_task_mode_pc_buzzer_sirene:
				if( Timing_Get_Bool(_e_Timing_Swing_Beep) == true)
				{
					Outputs_Buzz_Set( _e_OutBuzz_Siren );
					Timing_Set(_e_Timing_Swing_Beep, 500);
				}
				TaskManager_Step_Set( API_SWING, sApiSwTask.stepBack );
				break;

		default:
			TaskManager_Go( API_SWING , _Api_Swing_task_Home );
			break;
	}

	if ( ( sApiSwingCmdInfo.sInfo.u8Posicao_1 == sApiSwingCmdInfo.u8Posicao_1_Mem ) &&
	     ( sApiSwingCmdInfo.sInfo.u8Posicao_2 == sApiSwingCmdInfo.u8Posicao_2_Mem ) ) {
		return;
	}

	// Controle das bandeiras para não fechar as duas ao mesmo tempo.
	if( ( sApiSwingCmdInfo.sInfo.u8Posicao_1 >=  SW_CLOSE_POSIC ) ||
		( sApiSwingCmdInfo.u8Posicao_1_Mem >= SW_CLOSE_POSIC ) ) {

		if(sApiSwingCmdInfo.u8Posicao_2_Mem >= SW_CLOSE_POSIC ) {
			sApiSwingCmdInfo.sInfo.u8Posicao_1 = sApiSwingCmdInfo.u8Posicao_1_Mem;
		}
		else {
			sApiSwingCmdInfo.sInfo.u8Posicao_2 = sApiSwingCmdInfo.u8Posicao_2_Mem;
		}
	}
	else if ( ( sApiSwingCmdInfo.sInfo.u8Posicao_2 >=  SW_CLOSE_POSIC ) ||
		( sApiSwingCmdInfo.u8Posicao_2_Mem >= SW_CLOSE_POSIC ) ) {

		if(sApiSwingCmdInfo.u8Posicao_1_Mem >= SW_CLOSE_POSIC ) {
			sApiSwingCmdInfo.sInfo.u8Posicao_2 = sApiSwingCmdInfo.u8Posicao_2_Mem;
		}
		else {
			sApiSwingCmdInfo.sInfo.u8Posicao_1 = sApiSwingCmdInfo.u8Posicao_1_Mem;
		}
	}

	if( ( sApiSwingCmdInfo.sInfo.u8Posicao_1 < sApiSwingCmdInfo.u8Posicao_1_Mem ) &&
		( ( Sensor_Get() == _e_SensorSts_AlarmEntrance ) || ( Sensor_Get() == _e_SensorSts_Both ) ) ) {
		__NOP();
		return;
	}

	if( ( sApiSwingCmdInfo.sInfo.u8Posicao_2 < sApiSwingCmdInfo.u8Posicao_2_Mem) &&
		( ( Sensor_Get() == _e_SensorSts_AlarmExit ) || ( Sensor_Get() == _e_SensorSts_Both ) ) ) {
		__NOP();
		return;
	}

	// Salva as posições na variavel Mem.
	sApiSwingCmdInfo.u8Posicao_1_Mem = sApiSwingCmdInfo.sInfo.u8Posicao_1;
	sApiSwingCmdInfo.u8Posicao_2_Mem = sApiSwingCmdInfo.sInfo.u8Posicao_2;

	// Envia o comando para os motores se moverem.
	Position_PC_Set(sApiSwingCmdInfo.sInfo.u8Posicao_1, sApiSwingCmdInfo.sInfo.u8Posicao_2);
}


/*** ** * TASK MODE NA (Normalmente Aberta * ** ***/
typedef enum{
	_e_api_swing_task_mode_na_idle,
	_e_api_swing_task_mode_na_close,
	_e_api_swing_task_mode_na_open,
	_e_api_swing_task_mode_na_entrance,
	_e_api_swing_task_mode_na_waint_entrance,
	_e_api_swing_task_mode_na_exit,
	_e_api_swing_task_mode_na_waint_exit,
	_e_api_swing_task_mode_na_access_ok,
	_e_api_swing_task_mode_na_alarm_sts_user,
	_e_api_swing_task_mode_na_Entrance_sts_User,
	_e_api_swing_task_mode_na_buzzer_sirene,
}eApiSwStepModoNa_t;


#define TIME_CLOSE     1000

static void _api_swing_task_mode_na(void)
{
	tSensorSts SensorSts = _e_SensorSts_None;
	tSensorSts SensorStsPass = _e_SensorSts_PassWait;

	TaskManager_Set( API_SWING, _e_api_swuing_task_mode_na );

	if( TaskManager_First_Get( API_SWING ) == false )
	{
		TaskManager_First_Set( API_SWING );
		TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_na_idle );

		Sensor_Set( _e_SensorMode_Alarm );
	}

	switch( (uint8_t)TaskManager_Step_Get( API_SWING ) )
	{
		case _e_api_swing_task_mode_na_idle:

			Sensor_Set( _e_SensorMode_Alarm );
			SensorSts = Sensor_Get();

			if( Inputs_Com_Get() == _e_InCom_OpenIn  ) {
				TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_na_entrance);
			}
			if( Inputs_Com_Get() == _e_InCom_OpenOut ) {
				TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_na_exit );
			}


			if ( SensorSts == _e_SensorSts_Both ) {
				Timing_Set( _e_Timing_Swing_CloseDelayEntrance, TIME_CLOSE );
				Timing_Set( _e_Timing_Swing_CloseDelayExit, TIME_CLOSE );
				sApiSwTask.stepBack = _e_api_swing_task_mode_na_close;
				TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_na_alarm_sts_user );
			}
			else
			{
				if ( SensorSts == _e_SensorSts_AlarmEntrance ) {
					Timing_Set( _e_Timing_Swing_CloseDelayEntrance, TIME_CLOSE );
					sApiSwTask.stepBack = _e_api_swing_task_mode_na_close;
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_na_alarm_sts_user );
				}

				if ( SensorSts == _e_SensorSts_AlarmExit ) {
					Timing_Set( _e_Timing_Swing_CloseDelayExit, TIME_CLOSE );
					sApiSwTask.stepBack = _e_api_swing_task_mode_na_close;
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_na_alarm_sts_user );
				}

				if ( SensorSts == _E_SensorSts_AlarmMid ) {
					sApiSwTask.stepBack = _e_api_swing_task_mode_na_idle;
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_na_alarm_sts_user );
				}

				if ( SensorSts == _e_SensorSts_None ) {
					if ( ( Inputs_Com_Get() != _e_InCom_OpenIn  ) && (  Inputs_Com_Get() != _e_InCom_OpenOut  ) ) {
						TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_na_open );
					}
				}
			}
			break;

		case _e_api_swing_task_mode_na_close:
			Outputs_Com_Cheat( _e_OutCom_Cheat_Shit );
			TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_na_idle );
			break;

		case _e_api_swing_task_mode_na_open:
			Outputs_Com_Cheat( _e_OutCom_Cheat_None );

			if (  Inputs_Com_Get() != _e_InCom_OpenIn ) {
				Outputs_Buzz_Set( _e_OutBuzz_Off );
				Outputs_RGB_Set( _e_OutPic_In, _e_OutPicMode_Static, _e_OutPicColor_TSIgr, _e_OutPicSpeed_Slow );
			}
			if (  Inputs_Com_Get() != _e_InCom_OpenOut ) {
				Outputs_Buzz_Set( _e_OutBuzz_Off );
				Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Static, _e_OutPicColor_TSIgr, _e_OutPicSpeed_Slow );
			}
			if ( ( Inputs_Com_Get() != _e_InCom_OpenIn  ) && (  Inputs_Com_Get() != _e_InCom_OpenOut  ) ) {
				TaskManager_Go( API_SWING , _Api_Swing_task_Home );
			}
			else {
				TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_na_idle );
			}

			break;
		case _e_api_swing_task_mode_na_entrance:

			Sensor_Set( _e_SensorMode_Entrance );

			Outputs_RGB_Set( _e_OutPic_In, _e_OutPicMode_Blink, _e_OutPicColor_Green, _e_OutPicSpeed_Slow );
			Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
			Outputs_Com_Cheat( _e_OutCom_Cheat_None );
			Outputs_Buzz_Set( _e_OutBuzz_Beep );
			Timing_Set( _e_Timing_Swing_CloseDelay, APISWING_TIMEOUT_WAIT_ACCESS );
			TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_na_waint_entrance );

			break;

		case _e_api_swing_task_mode_na_waint_entrance:

			SensorStsPass = Sensor_Get();

			if( Inputs_Com_Get() != _e_InCom_OpenIn  	&&
				Inputs_Com_Get() != _e_InCom_OpenOut 	&&
				Timing_Get_Bool( _e_Timing_Swing_CloseDelay ) == true )
			{
				Outputs_Com_Pass( _e_OutCom_Pass_Nop );
				Outputs_Buzz_Set( _e_OutBuzz_Off );
				TaskManager_Go( API_SWING , _Api_Swing_task_Home );
				return;
			}

			switch( SensorStsPass )
			{
				case _e_SensorSts_PassOk:
					Outputs_RGB_Set( _e_OutPic_Both,  _e_OutPicMode_Blink, _e_OutPicColor_Pink, _e_OutPicSpeed_Fast );
					Timing_Set( _e_Timing_Swing_Picto_RGB , 1000 );
					Outputs_Com_Pass( _e_OutCom_Pass_OK );
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_na_access_ok );

					break;
				case _e_SensorSts_AlarmExit:
					Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
					sApiSwTask.stepBack = _e_api_swing_task_mode_na_waint_entrance;
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_na_buzzer_sirene );
					break;
				case _E_SensorSts_AlarmMid:
					Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
					sApiSwTask.stepBack = _e_api_swing_task_mode_na_waint_entrance;
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_na_buzzer_sirene );
					break;

				default:
					break;
			}
			break;

		case _e_api_swing_task_mode_na_exit:
			Sensor_Set( _e_SensorMode_Exit );

			Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Green, _e_OutPicSpeed_Slow );
			Outputs_RGB_Set( _e_OutPic_In, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
			Outputs_Com_Cheat( _e_OutCom_Cheat_None );
			Timing_Set( _e_Timing_Swing_CloseDelay, APISWING_TIMEOUT_WAIT_ACCESS );
			Outputs_Buzz_Set( _e_OutBuzz_Beep );
			TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_na_waint_exit );
			break;

		case _e_api_swing_task_mode_na_waint_exit:
			if( Inputs_Com_Get() != _e_InCom_OpenIn  	&&
				Inputs_Com_Get() != _e_InCom_OpenOut 	&&
				Timing_Get_Bool( _e_Timing_Swing_CloseDelay ) == true )
			{
				Outputs_Com_Pass( _e_OutCom_Pass_Nop );
				Outputs_Buzz_Set( _e_OutBuzz_Off );
				TaskManager_Go( API_SWING , _Api_Swing_task_Home );
				return;
			}

			SensorStsPass = Sensor_Get();
			switch( SensorStsPass )
			{
				case _e_SensorSts_PassOk:
					Outputs_RGB_Set( _e_OutPic_Both,  _e_OutPicMode_Blink, _e_OutPicColor_Pink, _e_OutPicSpeed_Fast );
					Timing_Set( _e_Timing_Swing_Picto_RGB , 1000 );
					Outputs_Com_Pass( _e_OutCom_Pass_OK );
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_na_access_ok );

					break;
				case _e_SensorSts_AlarmEntrance:
					Outputs_RGB_Set( _e_OutPic_In, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
					sApiSwTask.stepBack = _e_api_swing_task_mode_na_waint_exit;
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_na_buzzer_sirene );
					break;
				case _E_SensorSts_AlarmMid:
					Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
					sApiSwTask.stepBack = _e_api_swing_task_mode_na_waint_exit;
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_na_buzzer_sirene );
					break;

				default:
					break;
			}

			break;

			case _e_api_swing_task_mode_na_access_ok:
				if( Inputs_Com_Get() != _e_InCom_OpenIn &&
					Inputs_Com_Get() != _e_InCom_OpenOut )
				{
					Outputs_Com_Pass( _e_OutCom_Pass_Nop );
					Outputs_Buzz_Set( _e_OutBuzz_Off );
					Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Static, _e_OutPicColor_TSIgr, _e_OutPicSpeed_Slow );
					TaskManager_Go( API_SWING , _Api_Swing_task_Home );
				}
				break;

			case _e_api_swing_task_mode_na_alarm_sts_user:

				SensorSts = Sensor_Get();

				if ( SensorSts == _e_SensorSts_Both ) {
					Outputs_RGB_Set( _e_OutPic_In, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Slow );
					Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
				}
				else {
					if ( SensorSts == _e_SensorSts_AlarmEntrance ) {
						Outputs_RGB_Set( _e_OutPic_In, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
					}
					else if ( SensorSts == _e_SensorSts_AlarmExit ) {
						Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
					}
					else if ( SensorSts == _E_SensorSts_AlarmMid ) {
						Outputs_Buzz_Set( _e_OutBuzz_Siren );
						Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
					}
				}

				Outputs_Buzz_Set( _e_OutBuzz_Siren );

				TaskManager_Step_Set( API_SWING, sApiSwTask.stepBack );
				break;

			case _e_api_swing_task_mode_na_buzzer_sirene:
				if( Timing_Get_Bool(_e_Timing_Swing_Beep) == true)
				{
					Timing_Set(_e_Timing_Swing_Beep, 1000);
					Outputs_Buzz_Set( _e_OutBuzz_Siren );
				}
				TaskManager_Step_Set( API_SWING, sApiSwTask.stepBack );
				break;

		default:
			TaskManager_Go( API_SWING , _Api_Swing_task_Home );
			break;
	}

	if ( Timing_Get_Bool( _e_Timing_Swing_CloseDelayExit ) == false )
	{
		Position_NA_Entrance_Set(_e_PosModeNfSts_Block);
	}
	else {
		Position_NA_Entrance_Set(_e_PosModeNfSts_Free);
	}

	if ( Timing_Get_Bool( _e_Timing_Swing_CloseDelayEntrance ) == false ) {
		Position_NA_Exit_Set(_e_PosModeNfSts_Block);
	}
	else{
		Position_NA_Exit_Set(_e_PosModeNfSts_Free);
	}

}

/*** ** * TASK MODE NF (Normalmente Fechado * ** ***/
typedef enum{
	_e_api_swing_task_mode_nf_idle,
	_e_api_swing_task_mode_nf_close,
	_e_api_swing_task_mode_nf_entrance,
	_e_api_swing_task_mode_nf_waint_entrance,
	_e_api_swing_task_mode_nf_exit,
	_e_api_swing_task_mode_nf_waint_exit,
	_e_api_swing_task_mode_nf_access_ok,
	_e_api_swing_task_mode_nf_invasion,
	_e_api_swing_task_mode_nf_alarm_sts_user,
	_e_api_swing_task_mode_nf_Entrance_sts_User,
	_e_api_swing_task_mode_nf_idle_sts_User,
	_e_api_swing_task_mode_nf_buzzer_sirene,

}eApiSwStepModoNf_t;

static void _api_swing_task_mode_nf(void)
{
	tSensorSts SensorSts = _e_SensorSts_None;
	tSensorSts SensorStsPass = _e_SensorSts_PassWait;

	TaskManager_Set( API_SWING, _e_api_swuing_task_mode_nf );

	if( TaskManager_First_Get( API_SWING ) == false )
	{
		TaskManager_First_Set( API_SWING );
		TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_idle );
		sApiSwTask.stepPrevious = _e_api_swing_task_mode_nf_idle;

		Position_NF_Set(_e_PosModeNfSts_Close);
		Sensor_Set( _e_SensorMode_Alarm );
		Timing_Set(_e_Timing_Swing_Picto_RGB, 1000);
#if(SW_PICTOGRAMA_MODE == SW_PICTOGRAMA_FITA_LED)
		Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Static, _e_OutPicColor_TSIgr, _e_OutPicSpeed_Slow );
#else
		Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Static, _e_OutPicColor_Green, _e_OutPicSpeed_Fast );
#endif
	}

	switch( (uint8_t)TaskManager_Step_Get( API_SWING ) )
	{
		case _e_api_swing_task_mode_nf_idle:

			Sensor_Set( _e_SensorMode_Alarm );
			SensorSts = Sensor_Get();

			if( Inputs_Com_Get() == _e_InCom_OpenIn  )
			{
				TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_exit );
			}
			if( Inputs_Com_Get() == _e_InCom_OpenOut )
			{
				TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_entrance);
			}

			if ( SensorSts == _e_SensorSts_Both )
			{
				clrBurlaArmisHc();
				sApiSwTask.stepBack = _e_api_swing_task_mode_nf_idle;
				TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_alarm_sts_user );
			}
			else
			{
				if ( (SensorSts == _e_SensorSts_AlarmEntrance ) && (Inputs_Com_Get() != _e_InCom_OpenOut))
				{
					clrBurlaArmisHc();
					sApiSwTask.stepBack = _e_api_swing_task_mode_nf_idle;
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_alarm_sts_user );
				}

				if ( (SensorSts == _e_SensorSts_AlarmExit ) && (Inputs_Com_Get() != _e_InCom_OpenIn))
				{
					clrBurlaArmisHc();
					sApiSwTask.stepBack = _e_api_swing_task_mode_nf_idle;
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_alarm_sts_user );
				}

				if ( SensorSts == _E_SensorSts_AlarmMid )
				{
					setBurlaArmisHc();
					sApiSwTask.stepBack = _e_api_swing_task_mode_nf_idle;
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_alarm_sts_user );
				}

				if ( SensorSts == _e_SensorSts_None )
				{
					clrBurlaArmisHc();
					if ( ( Inputs_Com_Get() != _e_InCom_OpenIn  )	&&
						 ( Inputs_Com_Get() != _e_InCom_OpenOut ) )
					{
						TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_close );
					}
				}
			}
		break;

		case _e_api_swing_task_mode_nf_close:
			if ( ( Inputs_Com_Get() != _e_InCom_OpenIn  ) && (  Inputs_Com_Get() != _e_InCom_OpenOut  ) )
			{
				if ( Timing_Get_Bool( _e_Timing_Swing_CloseDelay ) == true )
				{
					Timing_Set( _e_Timing_Swing_CloseDelay, APISWING_KEEP_CLOSE );
					Position_NF_Set(_e_PosModeNfSts_Close);
				}
				sApiSwTask.stepBack = _e_api_swing_task_mode_nf_idle;
				TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_idle_sts_User );
			}
			else
			{
				TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_idle );
			}
		break;

		case _e_api_swing_task_mode_nf_entrance:
			Sensor_Set( _e_SensorMode_Entrance );
			Position_NF_Set(_e_PosModeNfSts_Exit);

#if(SW_PICTOGRAMA_MODE == SW_PICTOGRAMA_FITA_LED)
			Outputs_RGB_Set( _e_OutPic_In, _e_OutPicMode_Blink, _e_OutPicColor_Green, _e_OutPicSpeed_Slow );
			Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
#else
			Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
#endif
			Outputs_Com_Cheat( _e_OutCom_Cheat_None );
			Outputs_Buzz_Set( _e_OutBuzz_Beep );
			Timing_Set( _e_Timing_Swing_CloseDelay, APISWING_TIMEOUT_WAIT_ACCESS );
			TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_waint_entrance );
		break;

		case _e_api_swing_task_mode_nf_waint_entrance:
			SensorStsPass = Sensor_Get();

			if( Inputs_Com_Get() != _e_InCom_OpenIn  	&&
				Inputs_Com_Get() != _e_InCom_OpenOut 	&&
				Timing_Get_Bool( _e_Timing_Swing_CloseDelay ) == true )
			{
				Outputs_Com_Pass( _e_OutCom_Pass_Nop );
				Outputs_Buzz_Set( _e_OutBuzz_Off );
				TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_idle );
				return;
			}

			switch( SensorStsPass )
			{
				case _e_SensorSts_PassOk:
#if(SW_PICTOGRAMA_MODE == SW_PICTOGRAMA_FITA_LED)
					Outputs_RGB_Set( _e_OutPic_Both,  _e_OutPicMode_Blink, _e_OutPicColor_Pink, _e_OutPicSpeed_Fast );
#else
					//Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
#endif
					Timing_Set( _e_Timing_Swing_CloseDelay, APISWING_TIMEOUT_WAIT_ACCESS ); // Renova o tempo para indicar passOKAY
					sApiSwTask.stepPrevious = _e_api_swing_task_mode_nf_waint_entrance;
					Outputs_Com_Pass( _e_OutCom_Pass_OK );
					Timing_Set( _e_Timing_PassOk, TIMEOUT_PASSOK );
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_access_ok );
				break;

				case _e_SensorSts_AlarmExit:
#if(SW_PICTOGRAMA_MODE == SW_PICTOGRAMA_FITA_LED)
					//Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
#else
					Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
#endif
					Sensor_Set( _e_SensorMode_Alarm );
					sApiSwTask.stepBack = _e_api_swing_task_mode_nf_waint_entrance;
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_invasion );
//#if (SW_CLOSE_INVASION_MODE == SW_CLOSE_INVASION_CLOSE)
//					Position_NF_Set(_e_PosModeNfSts_Close);
//#endif
//					Timing_Set( _e_Timing_Swing_CloseDelay, APISWING_DELAY_CLOSE );
				break;

				case _E_SensorSts_AlarmMid:
#if(SW_PICTOGRAMA_MODE == SW_PICTOGRAMA_FITA_LED)
					Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
#else
					Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
#endif
					sApiSwTask.stepBack = _e_api_swing_task_mode_nf_waint_entrance;
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_buzzer_sirene );
				break;

				default:
				break;
			}
		break;

		case _e_api_swing_task_mode_nf_invasion:  // DSB/LBM - 26/07/2024: Alguém burlando o acesso
			SensorSts = Sensor_Get();
			if ( SensorSts == _e_SensorSts_Both )
			{
				Outputs_RGB_Set( _e_OutPic_In,  _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
				Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
				Outputs_Buzz_Set( _e_OutBuzz_Siren );
			}
			else
			{
				if ( SensorSts == _e_SensorSts_AlarmEntrance )
				{
					Outputs_RGB_Set( _e_OutPic_In, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
					Outputs_Buzz_Set( _e_OutBuzz_Siren );
				}
				else if ( SensorSts == _e_SensorSts_AlarmExit )
				{
					Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
					Outputs_Buzz_Set( _e_OutBuzz_Siren );
				}
				else if ( SensorSts == _E_SensorSts_AlarmMid )
				{
					Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
				}
			}

			if ( SensorSts == _e_SensorSts_None )  // So volta para a lógica de acesso se não existir qualquer obstrução em sensor, senão fica aqui até o tempo de acesso acabar
			{
				if(sApiSwTask.stepBack == _e_api_swing_task_mode_nf_waint_entrance)
				{
					Sensor_Set(_e_SensorMode_Entrance);
					Outputs_RGB_Set(_e_OutPic_In , _e_OutPicMode_Blink, _e_OutPicColor_Green, _e_OutPicSpeed_Slow);
					Outputs_RGB_Set(_e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Red  , _e_OutPicSpeed_Fast);
					TaskManager_Step_Set(API_SWING, _e_api_swing_task_mode_nf_waint_entrance);
				}
				else if(sApiSwTask.stepBack == _e_api_swing_task_mode_nf_waint_exit)
				{
					Sensor_Set(_e_SensorMode_Exit);
					Outputs_RGB_Set(_e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Green, _e_OutPicSpeed_Slow);
					Outputs_RGB_Set(_e_OutPic_In , _e_OutPicMode_Blink, _e_OutPicColor_Red  , _e_OutPicSpeed_Fast);
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_waint_exit );

				}
				Outputs_Com_Cheat( _e_OutCom_Cheat_None );
				Outputs_Buzz_Set( _e_OutBuzz_Beep );
			}

			if(Timing_Get_Bool( _e_Timing_Swing_CloseDelay ) == true)
			{
				TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_idle ); // Se o tempo do acesso acabar durante a invasão, volta pra idle e finaliza o acesso
			}
		break;

		case _e_api_swing_task_mode_nf_exit:
			Sensor_Set( _e_SensorMode_Exit );
			Position_NF_Set(_e_PosModeNfSts_Entrance);
#if(SW_PICTOGRAMA_MODE == SW_PICTOGRAMA_FITA_LED)
			Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Green, _e_OutPicSpeed_Slow );
			Outputs_RGB_Set( _e_OutPic_In, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
#else
			Outputs_RGB_Set( _e_OutPic_In, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
#endif
			Outputs_Com_Cheat( _e_OutCom_Cheat_None );
			Timing_Set( _e_Timing_Swing_CloseDelay, APISWING_TIMEOUT_WAIT_ACCESS );
			Outputs_Buzz_Set( _e_OutBuzz_Beep );
			TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_waint_exit );
		break;

		case _e_api_swing_task_mode_nf_waint_exit:
			if( Inputs_Com_Get() != _e_InCom_OpenIn  	&&
				Inputs_Com_Get() != _e_InCom_OpenOut 	&&
				Timing_Get_Bool( _e_Timing_Swing_CloseDelay ) == true )
			{
				Outputs_Com_Pass( _e_OutCom_Pass_Nop );
				Outputs_Buzz_Set( _e_OutBuzz_Off );
				TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_idle );
				return;
			}

			SensorStsPass = Sensor_Get();
			switch( SensorStsPass )
			{
				case _e_SensorSts_PassOk:
#if(SW_PICTOGRAMA_MODE == SW_PICTOGRAMA_FITA_LED)
					Outputs_RGB_Set( _e_OutPic_Both,  _e_OutPicMode_Blink, _e_OutPicColor_Pink, _e_OutPicSpeed_Fast );
#else
					//Outputs_RGB_Set( _e_OutPic_Both,  _e_OutPicMode_Blink, _e_OutPicColor_Pink, _e_OutPicSpeed_Fast );
#endif
					Timing_Set( _e_Timing_Swing_CloseDelay, APISWING_TIMEOUT_WAIT_ACCESS ); // Renova o tempo para indicar passOKAY
					sApiSwTask.stepPrevious = _e_api_swing_task_mode_nf_waint_exit;
					Outputs_Com_Pass( _e_OutCom_Pass_OK );
					Timing_Set( _e_Timing_PassOk, TIMEOUT_PASSOK );
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_access_ok );
				break;

				case _e_SensorSts_AlarmEntrance:
					Sensor_Set( _e_SensorMode_Alarm );
#if(SW_PICTOGRAMA_MODE == SW_PICTOGRAMA_FITA_LED)
					//Outputs_RGB_Set( _e_OutPic_In, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
#else
					Outputs_RGB_Set( _e_OutPic_In, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
#endif
					sApiSwTask.stepBack = _e_api_swing_task_mode_nf_waint_exit;
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_invasion );
//#if (SW_CLOSE_INVASION_MODE == SW_CLOSE_INVASION_CLOSE)
//					Position_NF_Set(_e_PosModeNfSts_Close);
//#endif
//					Timing_Set( _e_Timing_Swing_CloseDelay, APISWING_DELAY_CLOSE );
				break;

				case _E_SensorSts_AlarmMid:
#if(SW_PICTOGRAMA_MODE == SW_PICTOGRAMA_FITA_LED)
					Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
#else
					Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
#endif
					sApiSwTask.stepBack = _e_api_swing_task_mode_nf_waint_exit;
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_buzzer_sirene );
				break;

				default:

				break;
			}
		break;


		case _e_api_swing_task_mode_nf_access_ok:
			if ( (sApiSwTask.stepPrevious == _e_api_swing_task_mode_nf_waint_entrance)||
				 (sApiSwTask.stepPrevious == _e_api_swing_task_mode_nf_waint_exit    ) )
			{
				if(Timing_Get_Bool( _e_Timing_Swing_CloseDelay ) == true)
				{
					//Position_NF_Set(_e_PosModeNfSts_Close);
					TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_idle ); // Se o tempo carregado na passaOK estourar, fecha a bandeira e volta pra IDLE
				}

				if( Inputs_Com_Get() != _e_InCom_OpenIn &&
					Inputs_Com_Get() != _e_InCom_OpenOut )
				{
					if( Timing_Get_Bool(_e_Timing_PassOk) == true)
					{
						//Position_NF_Set(_e_PosModeNfSts_Close);
						Outputs_Com_Pass( _e_OutCom_Pass_Nop );
						Outputs_Buzz_Set( _e_OutBuzz_Off );
#if(SW_PICTOGRAMA_MODE == SW_PICTOGRAMA_FITA_LED)
						Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Static, _e_OutPicColor_TSIgr, _e_OutPicSpeed_Slow );
#endif
						Timing_Set( _e_Timing_Swing_CloseDelay, APISWING_DELAY_CLOSE );
						TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_idle );
					}
				}
			}
		break;

		case _e_api_swing_task_mode_nf_alarm_sts_user:
			SensorSts = Sensor_Get();
			if ( SensorSts == _e_SensorSts_Both )
			{
#if(SW_PICTOGRAMA_MODE == SW_PICTOGRAMA_FITA_LED)
				Outputs_RGB_Set( _e_OutPic_In,  _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
				Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
#else
				Outputs_RGB_Set( _e_OutPic_In , _e_OutPicMode_Blink, _e_OutPicColor_Green, _e_OutPicSpeed_Fast );      // LBM - 20/09/2023: Foi mudado o firmware nessa versão, para fazer o pictograma da PD300 funcionar. Já funcionava na POC na primeira versão, porém não foi feito diagrama de ligação.
				Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Green, _e_OutPicSpeed_Fast );
#endif
				Outputs_Buzz_Set( _e_OutBuzz_Siren );
			}
			else
			{
				if ( SensorSts == _e_SensorSts_AlarmEntrance )
				{
					Outputs_RGB_Set( _e_OutPic_In, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
					Outputs_Buzz_Set( _e_OutBuzz_Siren );
				}
				else if ( SensorSts == _e_SensorSts_AlarmExit )
				{
					Outputs_RGB_Set( _e_OutPic_Out, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
					Outputs_Buzz_Set( _e_OutBuzz_Siren );
				}
				else if ( SensorSts == _E_SensorSts_AlarmMid )
				{
					Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Blink, _e_OutPicColor_Red, _e_OutPicSpeed_Fast );
				}
			}
			TaskManager_Step_Set( API_SWING, sApiSwTask.stepBack );
		break;

		case _e_api_swing_task_mode_nf_idle_sts_User:

			if( Timing_Get_Bool(_e_Timing_Swing_Picto_RGB) == true)
			{
#if(SW_PICTOGRAMA_MODE == SW_PICTOGRAMA_FITA_LED)
				Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Static, _e_OutPicColor_TSIgr, _e_OutPicSpeed_Slow );
#else
				Outputs_RGB_Set( _e_OutPic_Both, _e_OutPicMode_Static, _e_OutPicColor_Green, _e_OutPicSpeed_Fast );
#endif
				Timing_Set(_e_Timing_Swing_Picto_RGB, 1000);
			}
			Outputs_Buzz_Set( _e_OutBuzz_Off );
			TaskManager_Step_Set( API_SWING, sApiSwTask.stepBack );
		break;

		case _e_api_swing_task_mode_nf_buzzer_sirene:
			if( Timing_Get_Bool(_e_Timing_Swing_Beep) == true)
			{
				Timing_Set(_e_Timing_Swing_Beep, 500);
				Outputs_Buzz_Set( _e_OutBuzz_Siren );
			}
			TaskManager_Step_Set( API_SWING, sApiSwTask.stepBack );
		break;

		default: TaskManager_Step_Set( API_SWING, _e_api_swing_task_mode_nf_idle );	break;
	}

}

void Api_Swing_Init(void)
{
	memset((void *)&sApiSwingVar, 0, sizeof(sApiSwingVar_t));
	sApiSwingCmdInfo.configMode = 0;

	TaskManager_Clear( API_SWING );
	TaskManager_Go( API_SWING , _api_swing_task_searchHome );

	TaskManager_Clear( API_URNA );
	TaskManager_Go( API_URNA , _Api_SwingUrna_task );

	TaskManager_Clear( TRATA_PACKET );
	TaskManager_Go( TRATA_PACKET, _Api_Swing_task_Trata_Packet );
	_Api_Swing_RenewTimeoutCamera();

	// inicializa o modulo de position.
	Position_Init();

	// inicializa o modulo de sensor.
	Sensor_Init();
}

//URNA
/*** ** * TASK API_SWING ALARME * ** ***/
bool Api_Swing_alarm(void)
{
	if(Api_Swing_get_sensor() != 0 ){
		Timing_Set( _e_Timing_Swing_AlarmDeb , 500 );
		return true;
	}
	else{
		return false;
	}
}

//--------------------------------------
void Api_Swing_alarm_task(void)
{
	if( Timing_Get_Bool( _e_Timing_Swing_Beep ) == false )// Beep
	{
		Outputs_Buzz_Set( _e_OutBuzz_Beep );
	}

	if(getBurlaArmisHc() == true)
	{
		if ( Timing_Get_Bool( _e_Timing_Burla ) == true )
		{
			Timing_Set( _e_Timing_Burla, TIMEOUT_BURLA );
			if(Outputs_Com_Cheat_Get() == _e_OutCom_Cheat_None)
			{
				Outputs_Com_Cheat( _e_OutCom_Cheat_Shit );
			}
			else
			{
				Outputs_Com_Cheat( _e_OutCom_Cheat_None );
			}
		}
	}
}

//--------------------------------------
void Api_Swing_Task_Alarme(void)
{
	Api_Swing_alarm_task();
}

/*** ** * TASK SWING URNA * ** ***/
static void _Api_SwingUrna_task(void)
{
	TaskManager_Set( API_URNA, _e_ApiUrna_task_Urna );

	if( TaskManager_First_Get( API_URNA ) == false )
	{
		TaskManager_First_Set ( API_URNA );
		TaskManager_Step_Set ( API_URNA , 0 );
	}
#if 0
	switch( (uint8_t)Inputs_Com_Get() )
	{
		case _e_InCom_UrnPicto:
			Outputs_Urn_Pic_Set( _e_OutUrnPic_Blink );
			break;
		default:
			Outputs_Urn_Pic_Set( _e_OutUrnPic_Off );
			break;
	}
#endif

	switch( (uint8_t)TaskManager_Step_Get( API_URNA ) )
	{
		case 0:

			switch( (uint8_t)Inputs_Com_Get() )
			{
				case _e_InCom_UrnPicto:
					Outputs_Urn_Pic_Set( _e_OutUrnPic_Blink );
					Timing_Set( _e_Timing_Swing_PictoUrn , 3000 );
					break;
				default:
					if( Timing_Get_Bool( _e_Timing_Swing_PictoUrn ) ){
						Outputs_Urn_Pic_Set( _e_OutUrnPic_Off );
						Timing_Set( _e_Timing_Swing_PictoUrn , 200 );
					}
					break;
			}

			if( Inputs_Urn_CardDet(_e_UrnCardDet_Get) == true ){
				Timing_Set( _e_Timing_Swing_UrnTimeOut , SWING_URN_TIMEOUT );
				TaskManager_Step_Set ( API_URNA , 1 );
			}
			else{
				//Inputs_Urn_CardDet( _e_UrnCardDet_Clr );
			}
			break;

		case 1:
			if( Timing_Get_Bool( _e_Timing_Swing_UrnTimeOut ) ){
				Timing_Set( _e_Timing_Swing_UrnTimeOut , SWING_URN_SOLTIME );
				Outputs_Urn_Sol_Set( _e_OutUrn_Open );
				TaskManager_Step_Set	( API_URNA , 2 );
			}
			break;

		case 2:
			if( Timing_Get_Bool( _e_Timing_Swing_UrnTimeOut ) ){
				Inputs_Urn_CardDet(_e_UrnCardDet_Clr);
				Outputs_Urn_Sol_Set( _e_OutUrn_Close );
				TaskManager_Step_Set	( API_URNA , 0 );
			}
			break;

		default:
			TaskManager_Step_Set	( API_URNA , 0 );
			break;
	}
}



//--------------------------------------
// LEITURA DOS SENSROES

uint8_t Api_Swing_get_sensor(void)
{
	uint8_t sensorReturn = _e_Sensors_None;

	bool senIn    = (bool)!Inputs_Get( _e_Inputs_SenIn );
	bool senOut   = (bool)!Inputs_Get( _e_Inputs_SenOut );
	bool senMidUp = (bool)!Inputs_Get( _e_Inputs_SenMidU );
	bool senMidDw = (bool)!Inputs_Get( _e_Inputs_SenMidD );
	sensorReturn = (uint8_t)( senIn + (senMidUp<<1) + (senMidDw<<2) + (senOut<<3) );

	return sensorReturn;
}
