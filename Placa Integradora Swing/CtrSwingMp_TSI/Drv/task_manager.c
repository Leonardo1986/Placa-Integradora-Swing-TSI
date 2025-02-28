/**
  ******************************************************************************
  * @file    task_manager.c
  * @author  Arthur Murata
  * @version V0.1
  * @date    26/08/2020
  * @brief   task_manager Source
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) TSI.
  *
  ******************************************************************************
  */

/*** ** * Includes * ** ***/

#include "task_manager.h"

/*** ** * Defines * ** ***/

#define TASK_MAX	20

/*** ** * Types * ** ***/

typedef struct{
	bool		flag;
	uint8_t	step;
}tTaskCtrl;

/*** ** * Variables * ** ***/

struct{
	tTaskCtrl 		sTaskCtrl[TASK_MAX];
	uint8_t			taskNum;
	_TaskManager_t	taskToRun;
}sTask[_e_TaskManager_Qty];

/*** ** * Funcs Int * ** ***/

/*** ** * Funcs Ext * ** ***/

//Roda as funcoes carregadas, nao esquecer de carregar antes de executar
void TaskManager(void)
{
	for( uint8_t i = 0 ; i < _e_TaskManager_Qty ; i++ ){
		if( sTask[i].taskToRun != NULL ){
			sTask[i].taskToRun();
		}
	}
}

//Carrega ponteiro de funcao a ser executada
void TaskManager_Go( tTaskManager taskMod , _TaskManager_t taskToGo )
{
	sTask[taskMod].taskToRun = taskToGo;
}

//Apaga flags de primeira entrada das tasks e seta 0 pra task que esta rodando
void TaskManager_Clear(tTaskManager taskMod )
{
	for( uint8_t i = 0 ; i < TASK_MAX ; i++ ){
		sTask[taskMod].sTaskCtrl[i].flag = false;
	}
	sTask[taskMod].taskNum = 0;
}

//Seta task atual - Colocar na primeira linha da funcao
void TaskManager_Set( tTaskManager taskMod , uint8_t taskRun )
{
	sTask[taskMod].taskNum = taskRun;
}

//Pega task atual
uint8_t TaskManager_Get( tTaskManager taskMod )
{
	return sTask[taskMod].taskNum;
}

//Seta primeira entrada na task
void TaskManager_First_Set( tTaskManager taskMod )
{
  for( uint8_t i = 0 ; i < TASK_MAX ; i++ ){
    if( i == sTask[taskMod].taskNum ) sTask[taskMod].sTaskCtrl[i].flag = true;
    else sTask[taskMod].sTaskCtrl[i].flag = false;
  }
}

// Limpa a primeira entrada na task
void TaskManager_First_Clear( tTaskManager taskMod )
{
   sTask[taskMod].sTaskCtrl[sTask[taskMod].taskNum].flag = false;
}

//Pega primeira entrada na task
bool TaskManager_First_Get( tTaskManager taskMod )
{
  return sTask[taskMod].sTaskCtrl[sTask[taskMod].taskNum].flag;
}

//Seta step da task
void TaskManager_Step_Set( tTaskManager taskMod, uint8_t taskStep )
{
	sTask[taskMod].sTaskCtrl[sTask[taskMod].taskNum].step = taskStep;
}

//Pega step da task
uint8_t TaskManager_Step_Get( tTaskManager taskMod )
{
	return sTask[taskMod].sTaskCtrl[sTask[taskMod].taskNum].step;
}

//Incrementa step da task
void TaskManager_Step_Inc( tTaskManager taskMod )
{
	sTask[taskMod].sTaskCtrl[sTask[taskMod].taskNum].step++;
}

//Decrementa step da task
void TaskManager_Step_Dec( tTaskManager taskMod )
{
	sTask[taskMod].sTaskCtrl[sTask[taskMod].taskNum].step--;
}
/*****************************END OF FILE****/
