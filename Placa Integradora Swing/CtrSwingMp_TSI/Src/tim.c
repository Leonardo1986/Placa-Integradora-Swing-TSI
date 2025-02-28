/**
  ******************************************************************************
  * File Name          : tim.c
  * Description        : timers
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"
//#include "../Drv/motor.h"
#include "../Drv/outputs.h"


/*----------------------------------------------------------------------------*/
/* Configure TIM                                                             */
/*----------------------------------------------------------------------------*/

#define MAX_PERIOD	100

#define MOTOR_TASK_IT_BY_OUTPUT_CAPTURE 1
/* Private variables ---------------------------------------------------------*/

TIM_HandleTypeDef htim1;	//Pwm Motors
TIM_HandleTypeDef htim3;	//Pwm Picto RGB
TIM_HandleTypeDef htim6;	//Timing
TIM_HandleTypeDef htim14;	//Pwm Urna Solenoide
TIM_HandleTypeDef htim15;	//Pwm Picto RGB
TIM_HandleTypeDef htim16;	//Pwm Led Display
TIM_HandleTypeDef htim17;	//Pwm Buzzer

static uint8_t m_u8FlagMotor_1_Start = 0, m_u8FlagMotor_2_Start = 0;


/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
void TIM_1_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 2500;//1000;//250;//64;//48;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 100;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;//TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 50;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }

	if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
	if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
	if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
	if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim1);
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
void TIM_3_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 48;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 250;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;//TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }

	if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
	if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
	if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
	if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim3);
}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
void TIM_6_Init(void)
{
	HAL_NVIC_SetPriority(TIM6_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ(TIM6_IRQn);

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 480; // 100;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 100;//
  htim6.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK){
    Error_Handler();
  }

  if (HAL_TIM_Base_Start_IT(&htim6) != HAL_OK){
    Error_Handler();
  }

}

/**
  * @brief TIM14 Initialization Function
  * @param None
  * @retval None
  */
void TIM_14_Init(void)
{
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 48;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 100;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;//TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

	if (HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }


  HAL_TIM_MspPostInit(&htim14);
}

/**
  * @brief TIM15 Initialization Function
  * @param None
  * @retval None
  */
void TIM_15_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  htim15.Instance = TIM15;
  htim15.Init.Prescaler = TIM_MOTOR_PRESCALER;
  htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim15.Init.Period = TIM_MOTOR_PERIOD;
  htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim15.Init.RepetitionCounter = 0;
  htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim15) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim15, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = TIM_MOTOR_PULSE;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim15, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
//  if (HAL_TIM_PWM_ConfigChannel(&htim15, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
//  {
//    Error_Handler();
//  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim15, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim15);

//  if (HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//
//  if (HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2) != HAL_OK)
//  {
//    Error_Handler();
//  }

}

/**
  * @brief TIM16 Initialization Function
  * @param None
  * @retval None
  */
void TIM_16_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  htim16.Instance = TIM16;
  htim16.Init.Prescaler = TIM_MOTOR_PRESCALER;//48;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = TIM_MOTOR_PERIOD;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;//TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim16) != HAL_OK)
  {
	Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim16, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = TIM_MOTOR_PULSE;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim16, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim16, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim16);

//  if (HAL_TIM_PWM_Start_IT(&htim16, TIM_CHANNEL_1) != HAL_OK)
//  {
//    Error_Handler();
//  }
}

/**
  * @brief TIM17 Initialization Function
  * @param None
  * @retval None
  */
void TIM_17_Init(void)
{
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  htim17.Instance = TIM17;
  htim17.Init.Prescaler = 24;//48;//48;
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = 100;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;//TIM_CLOCKDIVISION_DIV1;
  htim17.Init.RepetitionCounter = 0;
  htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim17) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim17) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim17, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim17, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim17);
}


void TIM_Init_All(void)
{
	TIM_1_Init();
	TIM_3_Init();
	TIM_6_Init();
	TIM_14_Init();
	TIM_15_Init();
	TIM_16_Init();
	//#warning "BUZZER DESABILITADO"
	TIM_17_Init();
}



void TIM_MOTOR_STOP(uint8_t p_u8Motor)
{
	if(p_u8Motor == TIM_MOTOR_1)
	{
#if MOTOR_TASK_IT_BY_OUTPUT_CAPTURE
		HAL_TIM_PWM_Stop_IT(&htim15, TIM_CHANNEL_1);
#else
		HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_1);
#endif
		m_u8FlagMotor_1_Start = 0;
		GPIO_MT1_DIR_RIGHT;
	}
	else
	{
#if MOTOR_TASK_IT_BY_OUTPUT_CAPTURE
		HAL_TIM_PWM_Stop_IT(&htim16, TIM_CHANNEL_1);
#else
		HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
#endif
		m_u8FlagMotor_2_Start = 0;
		GPIO_MT2_DIR_RIGHT;
	}
}

void TIM_MOTOR_RUN(uint8_t p_u8Motor, uint16_t p_u16Volocidade, uint8_t p_u8Dir)
{

	ENABLE_MOTORS();

	p_u16Volocidade = MAX_PERIOD - p_u16Volocidade;

	if(p_u8Motor == TIM_MOTOR_1)
	{
		p_u8Dir == TIM_DIR_LEFT ? GPIO_MT1_DIR_LEFT : GPIO_MT1_DIR_RIGHT;

		if(m_u8FlagMotor_1_Start == 0)
		{
#if MOTOR_TASK_IT_BY_OUTPUT_CAPTURE
			HAL_TIM_PWM_Start_IT(&htim15, TIM_CHANNEL_1);
			__HAL_TIM_ENABLE_IT(&htim15, TIM_IT_UPDATE);
#else
			HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
#endif
			m_u8FlagMotor_1_Start = 1;
		}

		OUTPUTS_TIMER_MOTOR_1->ARR = p_u16Volocidade;
		OUTPUTS_TIMER_MOTOR_1->CCR1 = p_u16Volocidade / 2;
	}
	else
	{
		p_u8Dir == TIM_DIR_LEFT ? GPIO_MT2_DIR_LEFT : GPIO_MT2_DIR_RIGHT;

		if(m_u8FlagMotor_2_Start == 0)
		{
#if MOTOR_TASK_IT_BY_OUTPUT_CAPTURE
			HAL_TIM_PWM_Start_IT(&htim16, TIM_CHANNEL_1);
			__HAL_TIM_ENABLE_IT(&htim16, TIM_IT_UPDATE);
#else
			HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
#endif
			m_u8FlagMotor_2_Start = 1;
		}

		OUTPUTS_TIMER_MOTOR_2->ARR = p_u16Volocidade;
		OUTPUTS_TIMER_MOTOR_2->CCR1 = p_u16Volocidade / 2;
	}
}

void TIM_DisableInterruptMotor(uint8_t p_u8Motor)
{
	if(p_u8Motor == TIM_MOTOR_1)
	{
		__HAL_TIM_DISABLE_IT(&htim15, TIM_IT_UPDATE);
	}
	else
	{
		__HAL_TIM_DISABLE_IT(&htim16, TIM_IT_UPDATE);
	}
}

void TIM_EnableInterruptMotor(uint8_t p_u8Motor)
{
	if(p_u8Motor == TIM_MOTOR_1)
	{
		__HAL_TIM_ENABLE_IT(&htim15, TIM_IT_UPDATE);
	}
	else
	{
		__HAL_TIM_ENABLE_IT(&htim16, TIM_IT_UPDATE);
	}
}


void TIM_STPMT_STOP(uint8_t p_u8Motor)
{
	if(p_u8Motor == TIM_MOTOR_1)
	{
		HAL_TIM_PWM_Stop_IT(&htim15, TIM_CHANNEL_1);
		__HAL_TIM_DISABLE_IT(&htim15, TIM_IT_UPDATE);
		GPIO_MT1_DIR_RIGHT;
	}
	else
	{
		HAL_TIM_PWM_Stop_IT(&htim16, TIM_CHANNEL_1);
		__HAL_TIM_DISABLE_IT(&htim16, TIM_IT_UPDATE);
		GPIO_MT2_DIR_RIGHT;
	}
}

void TIM_STPMT_START(uint8_t p_u8Motor , uint8_t p_u8Dir )
{
	if(p_u8Motor == TIM_MOTOR_1)
	{
		p_u8Dir == TIM_DIR_LEFT ? GPIO_MT1_DIR_LEFT : GPIO_MT1_DIR_RIGHT;
		HAL_TIM_PWM_Start_IT(&htim15, TIM_CHANNEL_1);
		__HAL_TIM_ENABLE_IT(&htim15, TIM_IT_UPDATE);
	}
	else
	{
		p_u8Dir == TIM_DIR_LEFT ? GPIO_MT2_DIR_LEFT : GPIO_MT2_DIR_RIGHT;
		HAL_TIM_PWM_Start_IT(&htim16, TIM_CHANNEL_1);
		__HAL_TIM_ENABLE_IT(&htim16, TIM_IT_UPDATE);
	}
}

void TIM_STPMT_SET(uint8_t p_u8Motor , uint16_t p_period )
{
	if(p_u8Motor == TIM_MOTOR_1)
	{
		OUTPUTS_TIMER_MOTOR_1->ARR = p_period;
		OUTPUTS_TIMER_MOTOR_1->CCR1 = p_period / 2;
	}
	else
	{
		OUTPUTS_TIMER_MOTOR_2->ARR = p_period;
		OUTPUTS_TIMER_MOTOR_2->CCR1 = p_period / 2;
	}
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
