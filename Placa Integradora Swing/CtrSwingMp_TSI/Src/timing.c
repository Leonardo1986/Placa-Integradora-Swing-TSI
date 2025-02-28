/**
  ******************************************************************************
  * @file    timing.c
  * @author  Arthur Murata
  * @version V0.1
  * @date    08/02/2019
  * @brief   Timing Source
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Hive</center></h2>
  *
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/

#include "timing.h"


/* Local Variables -----------------------------------------------------------*/

uint16_t Timing[_e_Timing_Qty];

/* IT Functions --------------------------------------------------------------*/

void Timing_IT(void)//   +/- 10ms
{
  uint8_t i;
  for( i = 0 ; i < _e_Timing_Qty ; i++ ){
    if( Timing[i] > 0 )Timing[i]--;
  }
}

/* Functions -----------------------------------------------------------------*/

void Timing_Set( t_Timing TimeType , uint16_t Time )
{
  Timing[TimeType] = Time;
}

uint16_t Timing_Get( t_Timing TimeType )
{
  return Timing[TimeType];
}

bool Timing_Get_Bool( t_Timing TimeType )
{
	if( Timing[TimeType] == 0 ){
		return true;
	}
	else{
		return false;
	}
}

/************************** (C) COPYRIGHT Hive ************** END OF FILE *****/
