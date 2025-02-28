/**
  ******************************************************************************
  * @file    sw_config.h
  * @author  José Faustino
  * @version V0.1
  * @date    19/11/2021
  * @brief   sw_config Header
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) TSI.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __sw_config_H
#define __sw_config_H
#ifdef __cplusplus
 extern "C" {
#endif

/*** ** * Includes * ** ***/
#include "main.h"

/*** ** * Defines * ** ***/
 #define SWCFG_BUZZER	                1

 #define SW_SWING_TESTE_DISABLE         0
 #define SW_SWING_TESTE_ENABLE          1
 #define SW_SWING_TESTE                 SW_SWING_TESTE_DISABLE

 #define SW_MOTOR_UNIT                  1
 #define SW_MOTOR_DUAL                  2
 #define SW_MOTOR_JMP                   3
 #define SW_MOTOR_QUAD                  4
 #define SW_MOTOR_QTY                   SW_MOTOR_DUAL

 #define SW_SENSOR_MODE_NORMAL          0
 #define SW_SENSOR_MODE_GIGA            1
 #define SW_SENSOR_MODE_COMPLETO        2
 #define SW_SENSOR_MODE                 SW_SENSOR_MODE_NORMAL

 #define SW_OPERATION_MODE_PC           0
 #define SW_OPERATION_MODE_NA           1
 #define SW_OPERATION_MODE_NF           2
 #define SW_OPERATION_MODE              SW_OPERATION_MODE_NF

 #define SW_OPERATION_SMALL_BARRIER     0
 #define SW_OPERATION_MEDIUM_BARRIER    1
 #define SW_OPERATION_BIG_BARRIER       2
 #define SW_OPERATION_METALIC_BARRIER   3                             // LBM - 10/02/2024: Utilizada para selfcheckout (código: 37.707.096.0)
 #define SW_OPERATION_MEDIUM_L_BARRIER  4                             // LBM - 16/01/2025: Bandeira em formato de 'L' usada uma unica vez
 #define SW_OPERATION_BARRIER           SW_OPERATION_MEDIUM_BARRIER // SW_OPERATION_METALIC_BARRIER // SW_OPERATION_MEDIUM_BARRIER

 // DSB/LBM - 24/07/2024: Determina se a bandeira irá fechar imediatamente a intrusão no sentido oposto ao acesso permitido.
 #define SW_CLOSE_INVASION_CLOSE                   0 // Fecha "imediatamante"
 #define SW_CLOSE_INVASION_AFTER_REALESE_SENSOR    1 // Fica alarmando enquanto estiver no sensor, saiu fecha
 #define SW_CLOSE_INVASION_MODE                    SW_CLOSE_INVASION_AFTER_REALESE_SENSOR //SW_CLOSE_INVASION_CLOSE //SW_CLOSE_INVASION_AFTER_REALESE_SENSOR

 // LBM - 16/01/2025: Configura se a indicação do pictograma da catraca será feita pela fita de LED ou pela PLACA.
 #define SW_PICTOGRAMA_FITA_LED  0
 #define SW_PICTOGRAMA_PLACA     1
 #define SW_PICTOGRAMA_MODE      SW_PICTOGRAMA_FITA_LED //SW_PICTOGRAMA_PLACA

//CONFIG. SYS MOVIMENTO MOTOR

 //stepmotor
#if (SW_OPERATION_MODE == SW_OPERATION_MODE_PC)

#define STPMT_PARAM_MEC_REDDUCTION	4

#else

// DSB/LBM - 24/07/2024: Definições de modelos diferentes de redutor
#define MOTOR_RED_KALATEC	16
#define MOTOR_RED_NEOMOTION	15

#define STPMT_PARAM_MEC_REDDUCTION	(1 + MOTOR_RED_NEOMOTION)

#endif

#define STPMT_PARAM_DRV_RESOLUTION	16
#define STPMT_PARAM_FULL_STEP_ANG	1.8
#define STPMT_PARAM_FULL_STEP_REV	(360 / STPMT_PARAM_FULL_STEP_ANG) //200
#define STPMT_PARAM_STEPS_PER_REV	((STPMT_PARAM_FULL_STEP_REV * STPMT_PARAM_DRV_RESOLUTION * STPMT_PARAM_MEC_REDDUCTION)*2) // 51200

//encoder
#define ENC_TYPE_DISC 	1
#define ENC_TYPE_INC	0

#if ( SW_OPERATION_MODE == SW_OPERATION_MODE_PC )
#define ENC_TYPE		ENC_TYPE_INC
#else
#define ENC_TYPE		ENC_TYPE_DISC // Disc "END-STOP", release 25/05/2022
#endif

#if(ENC_TYPE == ENC_TYPE_INC)
#define ENC_BREAK_COUNT						5000//2500//750//(STPMT_MT_SPEED_MAX/2*1.35)//5000
#else
	#if( (SW_OPERATION_BARRIER == SW_OPERATION_BIG_BARRIER)||(SW_OPERATION_BARRIER == SW_OPERATION_METALIC_BARRIER)||(SW_OPERATION_BARRIER == SW_OPERATION_MEDIUM_L_BARRIER) )
		#define ENC_BREAK_COUNT						3500//(STPMT_MT_SPEED_MAX/2*1.35)//5000
	#elif(SW_OPERATION_BARRIER == SW_OPERATION_MEDIUM_BARRIER)
		#define ENC_BREAK_COUNT						2500//(STPMT_MT_SPEED_MAX/2*1.35)//5000
	#else
		#define ENC_BREAK_COUNT						4000//(STPMT_MT_SPEED_MAX/2*1.35)//5000
	#endif

#endif

#if(STPMT_PARAM_MEC_REDDUCTION==4)

#define STPMT_MT_SPEED_MAX					16000//7000///5000/8250//8250
#define STPMT_MT_ACC_MAX					25//40//4//5//16
#define STPMT_MT_SPEED_INI_MIN				300
#define STPMT_MT_SPEED_INI_MAX				500//1000

#define STPMT_MT_SPEED_CAL					10000//8000//5250
#define STPMT_MT_ACC_CAL					1  //4
#define STPMT_MT_SPEED_INI_CAL				0

 //FEHCA
#define ENC_MOVE_PARAM_L_SPEED_MAX			STPMT_MT_SPEED_MAX
#define ENC_MOVE_PARAM_L_ACC_MAX			STPMT_MT_ACC_MAX
#define ENC_MOVE_PARAM_L_ANG_SPEED_MAX		50		//30//25
#define ENC_MOVE_PARAM_L_RAMP_SPEED_MIN		0.5	//0.25//0.25
#define ENC_MOVE_PARAM_L_RAMP_ACC_MIN		0.5	//0.50
#define ENC_MOVE_PARAM_L_CORRECT_PER		1 //02//1.07
 //ABRE
#define ENC_MOVE_PARAM_R_SPEED_MAX			(STPMT_MT_SPEED_MAX * 0.9)
#define ENC_MOVE_PARAM_R_ACC_MAX			(STPMT_MT_ACC_MAX * 0.8)
#define ENC_MOVE_PARAM_R_ANG_SPEED_MAX		100//75		//30//25
#define ENC_MOVE_PARAM_R_RAMP_SPEED_MIN		0.25	//0.25//0.25
#define ENC_MOVE_PARAM_R_RAMP_ACC_MIN		0.25	//0.5	//0.50
#define ENC_MOVE_PARAM_R_CORRECT_PER		0.85 //1	//1.02//1.07

#elif(STPMT_PARAM_MEC_REDDUCTION == STPMT_PARAM_MEC_REDDUCTION)
	#if(SW_OPERATION_BARRIER == SW_OPERATION_METALIC_BARRIER)

		//Não ultrapassar 18000 - perde passo
 	 	// LBM - 29/07/2024: As configurações abaixo apesar de ficarem excelentes, perde passo e o fw cai na condições de obstrução de bandeira a todo o momento
//		#define STPMT_MT_SPEED_MAX				    17500 //16500
//		#define STPMT_MT_ACC_MAX					5     //50
//		#define STPMT_MT_SPEED_INI_MIN			    3000  //600
//		#define STPMT_MT_SPEED_INI_MAX			    5000  //1000


 	 	// LBM - 29/07/2024: Parametros originais, não perde passo
		#define STPMT_MT_SPEED_MAX					14000 //16000       //18500
		#define STPMT_MT_ACC_MAX					50                  //4//5//16
		#define STPMT_MT_SPEED_INI_MIN				25
		#define STPMT_MT_SPEED_INI_MAX				100                 //1500//teste

 	 	// Atua para desprender o mecanismo de travamento
		#define STPMT_MT_SPEED_TREME                1000
		#define STPMT_MT_ACC_TEME					  40
		#define STPMT_MT_SPEED_INI_CAL				  25

		#define BALANCE_COUNT    				    2   // 5
		#define BALANCE_CONF_TIME					250 // 350 // 150 // 50
		#define BALANCE_CONF_ANG					5
		#define BALANCE_CONF_SPEED					STPMT_MT_SPEED_TREME*1    // 1.1
		#define BALANCE_CONF_ACC					STPMT_MT_ACC_TEME*1       // 1.1
		#define BALANCE_CONF_SPDI					STPMT_MT_SPEED_INI_CAL*1  // 1.1

		#define STPMT_MT_SPEED_CAL					10000               // 17000 //8000
		#define STPMT_MT_ACC_CAL					40                  // 4
		#define STPMT_MT_SPEED_INI_CAL				25                  // 50 //100

		 //FEHCA
		#define ENC_MOVE_PARAM_L_SPEED_MAX			STPMT_MT_SPEED_MAX
		#define ENC_MOVE_PARAM_L_ACC_MAX			STPMT_MT_ACC_MAX
		#define ENC_MOVE_PARAM_L_ANG_SPEED_MAX		75	                //30//25
		#define ENC_MOVE_PARAM_L_RAMP_SPEED_MIN		0.5	                //0.25
		#define ENC_MOVE_PARAM_L_RAMP_ACC_MIN		0.5	                //0.5
		#define ENC_MOVE_PARAM_L_CORRECT_PER		1	                //1.02//1.07
		 //ABRE
		#define ENC_MOVE_PARAM_R_SPEED_MAX			STPMT_MT_SPEED_MAX
		#define ENC_MOVE_PARAM_R_ACC_MAX			STPMT_MT_ACC_MAX
		#define ENC_MOVE_PARAM_R_ANG_SPEED_MAX		75	                //30//25
		#define ENC_MOVE_PARAM_R_RAMP_SPEED_MIN		0.5	                //0.25
		#define ENC_MOVE_PARAM_R_RAMP_ACC_MIN		0.5	                //0.5
		#define ENC_MOVE_PARAM_R_CORRECT_PER		1.07	            //1.02//1.07

	#elif(SW_OPERATION_BARRIER == SW_OPERATION_BIG_BARRIER)
		#define STPMT_MT_SPEED_MAX					14000               //18500
		#define STPMT_MT_ACC_MAX					50                  //4//5//16
		#define STPMT_MT_SPEED_INI_MIN				25
		#define STPMT_MT_SPEED_INI_MAX				100                 //1500//teste

		#define STPMT_MT_SPEED_CAL					10000               //17000//8000
		#define STPMT_MT_ACC_CAL					40                  //4
		#define STPMT_MT_SPEED_INI_CAL				25                  //50//100

		 //FEHCA
		#define ENC_MOVE_PARAM_L_SPEED_MAX			STPMT_MT_SPEED_MAX
		#define ENC_MOVE_PARAM_L_ACC_MAX			STPMT_MT_ACC_MAX
		#define ENC_MOVE_PARAM_L_ANG_SPEED_MAX		75	                //30//25
		#define ENC_MOVE_PARAM_L_RAMP_SPEED_MIN		0.5	                //0.25
		#define ENC_MOVE_PARAM_L_RAMP_ACC_MIN		0.5	                //0.5
		#define ENC_MOVE_PARAM_L_CORRECT_PER		1	                //1.02//1.07
		 //ABRE
		#define ENC_MOVE_PARAM_R_SPEED_MAX			STPMT_MT_SPEED_MAX
		#define ENC_MOVE_PARAM_R_ACC_MAX			STPMT_MT_ACC_MAX
		#define ENC_MOVE_PARAM_R_ANG_SPEED_MAX		75	                //30//25
		#define ENC_MOVE_PARAM_R_RAMP_SPEED_MIN		0.5	                //0.25
		#define ENC_MOVE_PARAM_R_RAMP_ACC_MIN		0.5	                //0.5
		#define ENC_MOVE_PARAM_R_CORRECT_PER		1.07	            //1.02//1.07
	#elif(SW_OPERATION_BARRIER == SW_OPERATION_MEDIUM_L_BARRIER)
	//Não ultrapassar 18000 - perde passo
		#define STPMT_MT_SPEED_MAX				    17500 //16500
		#define STPMT_MT_ACC_MAX					5     //50
		#define STPMT_MT_SPEED_INI_MIN			    3000  //600
		#define STPMT_MT_SPEED_INI_MAX			    5000  //1000

//		// Parametros originais
//		#define STPMT_MT_SPEED_MAX					14000 //16000       //18500
//		#define STPMT_MT_ACC_MAX					50                  //4//5//16
//		#define STPMT_MT_SPEED_INI_MIN				25
//		#define STPMT_MT_SPEED_INI_MAX				100                 //1500//teste

		// Atua para desprender o mecanismo de travamento
		#define STPMT_MT_SPEED_TREME                1000 // Valor baixo para aumentar o torque no balanço
		#define STPMT_MT_ACC_TEME					  40
		#define STPMT_MT_SPEED_INI_CAL				  25

		#define BALANCE_COUNT    				    2                         // 5
		#define BALANCE_CONF_TIME					350 				      // 150 // 50
		#define BALANCE_CONF_ANG					5
		#define BALANCE_CONF_SPEED					STPMT_MT_SPEED_TREME*1    // 1.1
		#define BALANCE_CONF_ACC					STPMT_MT_ACC_TEME*1       // 1.1
		#define BALANCE_CONF_SPDI					STPMT_MT_SPEED_INI_CAL*1  // 1.1

		#define STPMT_MT_SPEED_CAL					10000                     // 17000 //8000
		#define STPMT_MT_ACC_CAL					40                        // 4
		#define STPMT_MT_SPEED_INI_CAL				25                        // 50 //100

		 //FEHCA
		#define ENC_MOVE_PARAM_L_SPEED_MAX			STPMT_MT_SPEED_MAX
		#define ENC_MOVE_PARAM_L_ACC_MAX			STPMT_MT_ACC_MAX
		#define ENC_MOVE_PARAM_L_ANG_SPEED_MAX		75	                //30//25
		#define ENC_MOVE_PARAM_L_RAMP_SPEED_MIN		0.5	                //0.25
		#define ENC_MOVE_PARAM_L_RAMP_ACC_MIN		0.5	                //0.5
		#define ENC_MOVE_PARAM_L_CORRECT_PER		1	                //1.02//1.07
		 //ABRE
		#define ENC_MOVE_PARAM_R_SPEED_MAX			STPMT_MT_SPEED_MAX
		#define ENC_MOVE_PARAM_R_ACC_MAX			STPMT_MT_ACC_MAX
		#define ENC_MOVE_PARAM_R_ANG_SPEED_MAX		75	                //30//25
		#define ENC_MOVE_PARAM_R_RAMP_SPEED_MIN		0.5	                //0.25
		#define ENC_MOVE_PARAM_R_RAMP_ACC_MIN		0.5	                //0.5
		#define ENC_MOVE_PARAM_R_CORRECT_PER		1.07	            //1.02//1.07
	#elif(SW_OPERATION_BARRIER == SW_OPERATION_MEDIUM_BARRIER)

#if 0 //CONFIG_SWING_GPIO_HMI_BUS
 typedef struct swingConfig{
	 uint16_t stpmt_mt_speed_max;
	 uint16_t stpmt_mt_acc_max;
	 uint16_t stpmt_mt_speed_ini_min;
	 uint16_t stpmt_mt_speed_ini_max;
	 uint16_t stpmt_mt_speed_cal;
	 uint16_t stpmt_mt_acc_cal;
	 uint16_t stpmt_mt_speed_ini_cal;
	 uint16_t enc_move_param_speed_max[2];
	 uint16_t enc_move_param_acc_max[2];
	 uint16_t enc_move_param_ang_speed_max[2];
	 float enc_move_param_ramp_speed_min[2];
	 float enc_move_param_ramp_acc_min[2];
	 float enc_move_param_correct_per[2];
 }swingConfig_t;

#endif

#if(SW_OPERATION_BARRIER == SW_OPERATION_MEDIUM_BARRIER)
		// Não ultrapassar 18000 - perde passo
		#define STPMT_MT_SPEED_MAX					17500
		#define STPMT_MT_ACC_MAX					50
		#define STPMT_MT_SPEED_INI_MIN				400//600
		#define STPMT_MT_SPEED_INI_MAX				800//1000

		// Não ultrapassar 18000 - perde passo
		#define STPMT_MT_SPEED_CAL					14000//17000
		#define STPMT_MT_ACC_CAL					20
		#define STPMT_MT_SPEED_INI_CAL				100
#else
 	 // Não ultrapassar 18000 - perde passo
		#define STPMT_MT_SPEED_MAX					18000
		#define STPMT_MT_ACC_MAX					50
		#define STPMT_MT_SPEED_INI_MIN				600//400//600
		#define STPMT_MT_SPEED_INI_MAX				1000//800//1000


		// Não ultrapassar 18000 - perde passo
		#define STPMT_MT_SPEED_CAL					16000
		#define STPMT_MT_ACC_CAL					50
		#define STPMT_MT_SPEED_INI_CAL				100
#endif

		//FECHA
		#define ENC_MOVE_PARAM_L_SPEED_MAX			STPMT_MT_SPEED_MAX
		#define ENC_MOVE_PARAM_L_ACC_MAX			STPMT_MT_ACC_MAX
		#define ENC_MOVE_PARAM_L_ANG_SPEED_MAX		50
		#define ENC_MOVE_PARAM_L_RAMP_SPEED_MIN		0.5
		#define ENC_MOVE_PARAM_L_RAMP_ACC_MIN		0.5//(STPMT_MT_ACC_MAX*0.75)
#if(SW_OPERATION_BARRIER == SW_OPERATION_SMALL_BARRIER)
		#define ENC_MOVE_PARAM_L_CORRECT_PER		1.01//0.95 //1.0
#else
		#define ENC_MOVE_PARAM_L_CORRECT_PER		1//1.01//0.95 //1.0
#endif
		//ABRE
		#define ENC_MOVE_PARAM_R_SPEED_MAX			STPMT_MT_SPEED_MAX
		#define ENC_MOVE_PARAM_R_ACC_MAX			STPMT_MT_ACC_MAX
		#define ENC_MOVE_PARAM_R_ANG_SPEED_MAX		50
		#define ENC_MOVE_PARAM_R_RAMP_SPEED_MIN		0.5
		#define ENC_MOVE_PARAM_R_RAMP_ACC_MIN		0.5//(STPMT_MT_ACC_MAX*0.75)
#if(SW_OPERATION_BARRIER == SW_OPERATION_SMALL_BARRIER)
		#define ENC_MOVE_PARAM_R_CORRECT_PER		1.02//0.95 //1.0
#else
		#define ENC_MOVE_PARAM_R_CORRECT_PER		1//0.95 //1.0
#endif

	#else
		#define STPMT_MT_SPEED_MAX					18000
		#define STPMT_MT_ACC_MAX					50
		#define STPMT_MT_SPEED_INI_MIN				600//400//600
		#define STPMT_MT_SPEED_INI_MAX				1000//800//1000

		#define STPMT_MT_SPEED_CAL					16000//8000//5250
		#define STPMT_MT_ACC_CAL					50  //4
		#define STPMT_MT_SPEED_INI_CAL				100

		// FEHCA
		#define ENC_MOVE_PARAM_L_SPEED_MAX			STPMT_MT_SPEED_MAX
		#define ENC_MOVE_PARAM_L_ACC_MAX			STPMT_MT_ACC_MAX
		#define ENC_MOVE_PARAM_L_ANG_SPEED_MAX		75//50		//30//25
		#define ENC_MOVE_PARAM_L_RAMP_SPEED_MIN		0.5	//0.25//0.25
		#define ENC_MOVE_PARAM_L_RAMP_ACC_MIN		0.5	//0.5	//0.50
		#define ENC_MOVE_PARAM_L_CORRECT_PER		1	//1.02//1.07

		// ABRE
		#define ENC_MOVE_PARAM_R_SPEED_MAX			STPMT_MT_SPEED_MAX
		#define ENC_MOVE_PARAM_R_ACC_MAX			STPMT_MT_ACC_MAX
		#define ENC_MOVE_PARAM_R_ANG_SPEED_MAX		75		//30//25
		#define ENC_MOVE_PARAM_R_RAMP_SPEED_MIN		0.5	//0.25//0.25
		#define ENC_MOVE_PARAM_R_RAMP_ACC_MIN		0.5	//0.5	//0.50
		#define ENC_MOVE_PARAM_R_CORRECT_PER		1	//1.02//1.07
	#endif

#else  // STPMT_PARAM_MEC_REDDUCTION == 16

// Não ultrapassar 18000 - perde passo
//#define STPMT_MT_SPEED_MAX				17500 //16500
//#define STPMT_MT_ACC_MAX					5     //50
//#define STPMT_MT_SPEED_INI_MIN			3000  //600
//#define STPMT_MT_SPEED_INI_MAX			5000  //1000

#define STPMT_MT_SPEED_MAX					16000               //18500
#define STPMT_MT_ACC_MAX					50                  //4//5//16
#define STPMT_MT_SPEED_INI_MIN				25
#define STPMT_MT_SPEED_INI_MAX				100                 //1500//teste

// Não ultrapassar 18000 - perde passo
#define STPMT_MT_SPEED_CAL					16000
#define STPMT_MT_ACC_CAL					50
#define STPMT_MT_SPEED_INI_CAL				100

 //FECHA
#define ENC_MOVE_PARAM_L_SPEED_MAX			STPMT_MT_SPEED_MAX
#define ENC_MOVE_PARAM_L_ACC_MAX			STPMT_MT_ACC_MAX
#define ENC_MOVE_PARAM_L_ANG_SPEED_MAX		50
#define ENC_MOVE_PARAM_L_RAMP_SPEED_MIN		0.75
#define ENC_MOVE_PARAM_L_RAMP_ACC_MIN		0.75//(STPMT_MT_ACC_MAX*0.75)
#define ENC_MOVE_PARAM_L_CORRECT_PER		1.00
 //ABRE
#define ENC_MOVE_PARAM_R_SPEED_MAX			STPMT_MT_SPEED_MAX
#define ENC_MOVE_PARAM_R_ACC_MAX			STPMT_MT_ACC_MAX
#define ENC_MOVE_PARAM_R_ANG_SPEED_MAX		50
#define ENC_MOVE_PARAM_R_RAMP_SPEED_MIN		0.75
#define ENC_MOVE_PARAM_R_RAMP_ACC_MIN		0.75//(STPMT_MT_ACC_MAX*0.75)
#define ENC_MOVE_PARAM_R_CORRECT_PER		1.00

#endif

typedef enum{
	_e_EncMoveParam_SpeedMax,		// velocidade máxima
	_e_EncMoveParam_AccMax,			// aceleração máxima
	_e_EncMoveParam_AngSpeedMax,	// a partir deste angula atribui velocidade máxia
	_e_EncMoveParam_RampSpeedMin,	// velocidade inicial
	_e_EncMoveParam_RampAccMin,		// aceleração inicial
	_e_EncMoveParam_CorrectPer,		// variação do deslocamento
	_e_EncMoveParam_Qty,
}tEncMoveParam;

 	 //position

#define ENC_MOVE_ANG_TOL		3 //1//0.5//1.5

#if(SW_OPERATION_BARRIER == SW_OPERATION_SMALL_BARRIER)
#define POSITION_ANGPER_TOL		2//1.5//2 // 1 - Swing gabinete grande
#elif( (SW_OPERATION_BARRIER == SW_OPERATION_MEDIUM_BARRIER)||(SW_OPERATION_BARRIER == SW_OPERATION_BIG_BARRIER)||(SW_OPERATION_BARRIER == SW_OPERATION_METALIC_BARRIER)||(SW_OPERATION_BARRIER == SW_OPERATION_MEDIUM_L_BARRIER) )
#define POSITION_ANGPER_TOL		ENC_MOVE_ANG_TOL	//0.5//2 // 1 - Swing gabinete grande
#endif

#define SOLENOID_TIME 			2500 //1000 //2500
#define POSITION_CORRECT_TIME	2500
//#define POSITION_CORRECT_TIME_M	500 //250 //1000 //500

#define POS_CALIB_ATTEMPTS		5

#define TEMPO_BASE_CALIB       108000000 // VALOR BASE DE CALIBRAÇÃO ( STPMT_MT_SPEED_MAX * TIMEOUT_CALIB === 18000 * 6000)

#if (STPMT_MT_SPEED_MAX > STPMT_MT_SPEED_CAL)  // Usar sempre o menor valor para calibrar entre STPMT_MT_SPEED_MAX e STPMT_MT_SPEED_CAL

#define TIMEOUT_CALIB_DINAM    (TEMPO_BASE_CALIB / STPMT_MT_SPEED_CAL)

#else

#define TIMEOUT_CALIB_DINAM    (TEMPO_BASE_CALIB / STPMT_MT_SPEED_MAX)

#endif\
 /*** ** * Access Control * ** ***/

#define SW_CLOSE_POSIC         75

 /*** ** * Types * ** ***/

 typedef enum{
 	_e_SwMtNum_Pri,
 	_e_SwMtNum_Sec,
 	_e_SwMtNum_Qty,
 }eSwMtNum_t;

 typedef enum{
 	_e_SwMtQty_Single,
 	_e_SwMtQty_Dual,
 	_e_SwMtQty_Jump,
 	_e_SwMtQty_Quad,
 	_e_SwMtQty_Unknow,
 }eSwMtQty_t;

 typedef enum{
 	_e_Barrier_Medium = 0,
 	_e_Barrier_Big,
	_e_Barrier_Qty,
 }eSwBarrier_t;
	 
#ifdef __cplusplus
}
#endif
#endif /*__sw_config_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
