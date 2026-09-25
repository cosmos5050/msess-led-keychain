/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <stdbool.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// Struct for
typedef struct
{
	uint16_t duty;
	bool enabled;
} LED_t;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */
volatile uint16_t tickCounter = 0;
volatile uint8_t pattern = 0;
uint16_t stepRes = 10;
uint16_t defaultDuty = 1;
uint8_t numLeds = 12;
volatile uint8_t patternStep = 0;
volatile bool gpioNotReset = true;

volatile LED_t ledArr[12];


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */
void writeGPIO(uint8_t ledNum, bool enabled);
void alternating();
void ledChaser();
void sparkling();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK)
  {
      Error_Handler();
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // Set all bits high

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	// Check if pattern's changed

	switch(pattern)
	{
		case 0:
			alternating();
			break;
		case 1:
			ledChaser();
			break;
		case 2:
			sparkling();
			break;
	}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_0);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV4;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 11;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9|GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB9 PB0 PB2 PB3
                           PB4 PB5 PB6 PB7
                           PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA5 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA6 PA8 PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

// Set bit corresponding to switch if switched on
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_5)
	{
		pattern |= 1;
	}

	if (GPIO_Pin == GPIO_PIN_7)
	{
		pattern |= 2;
	}

	if (GPIO_Pin == GPIO_PIN_1)
	{
		pattern |= 4;
	}

	// Reset pattern step
	patternStep = 0;
	gpioNotReset = true;
}

// Reset bit corresponding to switch if switched off
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_5)
	{
		pattern &= ~(1 << 0);
	}

	if (GPIO_Pin == GPIO_PIN_7)
	{
		pattern &= ~(1 << 1);
	}

	if (GPIO_Pin == GPIO_PIN_1)
	{
		pattern &= ~(1 << 2);
	}

	// Reset pattern step
	patternStep = 0;
	gpioNotReset = true;
}

// ISR TIM3 for LED PWM
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	// Check instance member to see if tim3 has an IRQ
	if (htim->Instance == TIM3)
	{
		//Reset tick counter if reached 100%
		if (tickCounter >= stepRes)
		{
			tickCounter = 0;
		}

		// Cycle through LEDs
		for (uint8_t i = 0; i < numLeds; i++)
		{
			// If less than duty cycle set high else low
			if ((tickCounter < ledArr[i].duty) && ledArr[i].enabled)
			{
				writeGPIO(i + 1, true);
			}
			else
			{
				writeGPIO(i + 1, false);
			}
		}

		// Increment tick
		tickCounter++;
	}
}

// Lookup table to write GPIOs for PWM IRQ for cleanliness
void writeGPIO(uint8_t ledNum, bool enabled)
{
	switch (ledNum)
	{
		case 1:
			if (enabled)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
			}
			break;
		case 2:
			if (enabled)
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
			}
			break;
		case 3:
			if (enabled)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
			}
			break;
		case 4:
			if (enabled)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
			}
			break;
		case 5:
			if (enabled)
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
			}
			break;
		case 6:
			if (enabled)
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
			}
			break;
		case 7:
			if (enabled)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
			}
			break;
		case 8:
			if (enabled)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
			}
			break;
		case 9:
			if (enabled)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
			}
			break;
		case 10:
			if (enabled)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
			}
			break;
		case 11:
			if (enabled)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
			}
			break;
		case 12:
			if (enabled)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
			}
			break;
	}
}

// Helper function for sparkling to pick a new random index for one LED slot
static void reassignLedIdx(uint8_t randomLedIdx[3], uint8_t slot)
{
	bool notFarEnoughApart = true;
	while (notFarEnoughApart)
	{
		randomLedIdx[slot] = rand() % numLeds;
		notFarEnoughApart = false;

		for (uint8_t j = 0; j < 3; j++)
		{
			if (j == slot)
				continue;

			uint8_t wrapDiff = numLeds - abs(randomLedIdx[slot] - randomLedIdx[j]);

			if (wrapDiff > numLeds / 2)
				wrapDiff = numLeds - wrapDiff;

			if (wrapDiff <= 1)
			{
				notFarEnoughApart = true;
				break;
			}
		}
	}
}

void sparkling()
{
	static uint32_t startTime = 0;
	static bool firstCycle = false;
	static uint8_t randomLedIdx[3];

	// Reset LEDs on first pattern run, and seed the initial 3 indices
	if (gpioNotReset)
	{
		for (uint8_t i = 0; i < numLeds; i++)
		{
			ledArr[i].enabled = false;
			ledArr[i].duty = defaultDuty;
		}

		// Initial seed for all three slots (same spacing rule as before,
		// checking only against already-assigned slots since this only
		// runs once at reset)
		for (uint8_t i = 0; i < 3; i++)
		{
			randomLedIdx[i] = rand() % numLeds;

			for (uint8_t j = 0; j < i; j++)
			{
				bool notFarEnoughApart = true;
				while (notFarEnoughApart)
				{
					uint8_t wrapDiff = numLeds - abs(randomLedIdx[i] - randomLedIdx[j]);

					if (wrapDiff > numLeds / 2)
						wrapDiff = numLeds - wrapDiff;

					if (wrapDiff <= 1)
					{
						randomLedIdx[i] = rand() % numLeds;
					}
					else
					{
						notFarEnoughApart = false;
					}
				}
			}
		}

		gpioNotReset = false;
		firstCycle = true;
	}

	// Delay for each brightness increase
	uint16_t brightnessDelay = 300;

	switch(patternStep)
	{
		case 0:
			if (HAL_GetTick() - startTime >= brightnessDelay)
			{

				ledArr[randomLedIdx[0]].enabled = true;
				ledArr[randomLedIdx[0]].duty = defaultDuty;

				startTime = HAL_GetTick();
				if (firstCycle)
				{
					patternStep += 3;
				}
				else
				{
					patternStep++;
				}
			}
			break;
		case 1:
			if (HAL_GetTick() - startTime >= 100)
			{

				ledArr[randomLedIdx[1]].enabled = false;
				// idx[1] just turned off -> pick its next target now
				reassignLedIdx(randomLedIdx, 1);

				startTime = HAL_GetTick();
				patternStep++;
			}
			break;
		case 2:
			if (HAL_GetTick() - startTime >= 100)
			{

				ledArr[randomLedIdx[2]].duty = defaultDuty;

				startTime = HAL_GetTick();
				patternStep++;
			}
			break;
		case 3:
			if (HAL_GetTick() - startTime >= brightnessDelay)
			{
				ledArr[randomLedIdx[0]].duty = defaultDuty * 2;

				startTime = HAL_GetTick();
				patternStep++;
			}
			break;
		case 4:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[1]].duty = defaultDuty;
				ledArr[randomLedIdx[1]].enabled = defaultDuty;

				startTime = HAL_GetTick();
				if (firstCycle)
				{
					patternStep += 2;
					firstCycle = false;
				}
				else
				{
					patternStep++;
				}
			}
			break;
		case 5:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[2]].enabled = false;
				// idx[2] just turned off -> pick its next target now
				reassignLedIdx(randomLedIdx, 2);

				startTime = HAL_GetTick();
				patternStep++;
			}
			break;
		case 6:
			if (HAL_GetTick() - startTime >= brightnessDelay)
			{
				ledArr[randomLedIdx[0]].duty = defaultDuty * 3;

				startTime = HAL_GetTick();
				patternStep++;
			}
			break;
		case 7:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[1]].duty = defaultDuty * 2;

				startTime = HAL_GetTick();
				patternStep++;
			}
			break;
		case 8:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[2]].duty = defaultDuty;
				ledArr[randomLedIdx[2]].enabled = true;

				startTime = HAL_GetTick();
				patternStep++;
			}
			break;
		case 9:
			if (HAL_GetTick() - startTime >= brightnessDelay)
			{
				ledArr[randomLedIdx[0]].duty = defaultDuty * 2;

				startTime = HAL_GetTick();
				patternStep++;
			}
			break;
		case 10:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[1]].duty = defaultDuty * 3;

				startTime = HAL_GetTick();
				patternStep++;
			}
			break;
		case 11:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[2]].duty = defaultDuty * 2;

				startTime = HAL_GetTick();
				patternStep++;
			}
			break;
		case 12:
			if (HAL_GetTick() - startTime >= brightnessDelay)
			{
				ledArr[randomLedIdx[0]].duty = defaultDuty;

				startTime = HAL_GetTick();
				patternStep++;
			}
			break;
		case 13:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[1]].duty = defaultDuty * 2;

				startTime = HAL_GetTick();
				patternStep++;
			}
			break;
		case 14:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[2]].duty = defaultDuty * 3;

				startTime = HAL_GetTick();
				patternStep++;
			}
			break;
		case 15:
			if (HAL_GetTick() - startTime >= brightnessDelay)
			{
				ledArr[randomLedIdx[0]].duty = defaultDuty;

				startTime = HAL_GetTick();
				patternStep++;
			}
			break;
		case 16:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[1]].duty = defaultDuty * 2;

				startTime = HAL_GetTick();
				patternStep++;
			}
			break;
		case 17:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[2]].duty = defaultDuty * 3;

				startTime = HAL_GetTick();
				patternStep++;
			}
			break;
		case 18:
			if (HAL_GetTick() - startTime >= brightnessDelay)
			{

				ledArr[randomLedIdx[0]].enabled = false;
				// idx[0] just turned off -> pick its next target now
				reassignLedIdx(randomLedIdx, 0);

				startTime = HAL_GetTick();
				patternStep++;
			}
			break;
		case 19:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[1]].duty = defaultDuty;

				startTime = HAL_GetTick();
				patternStep++;
			}
			break;
		case 20:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[2]].duty = defaultDuty * 2;

				startTime = HAL_GetTick();
				patternStep = 0;
			}
			break;
	}
}

void ledChaser()
{
	static uint32_t startTime = 0;

	if (gpioNotReset)
	{
		for (uint8_t i = 0; i < numLeds; i++)
		{
			ledArr[i].enabled = false;
			ledArr[i].duty = defaultDuty;

			gpioNotReset = false;
		}
	}

	uint16_t chaseDelay = 100;

	if (HAL_GetTick() - startTime >= chaseDelay)
	{
		// Reset led idx once done one full cycle
		if (patternStep >= numLeds + 2)
		{
			patternStep = 0;
		}

		// Turn off last LED from last idx
		ledArr[patternStep].enabled = false;
		// Turn on middle led and leds either side of it
		ledArr[(patternStep + 1) % numLeds].enabled = true;
		ledArr[(patternStep + 2) % numLeds].enabled = true;
		ledArr[(patternStep + 3) % numLeds].enabled = true;

		// Make middle LED brightest and leds on either side dimmer
		// for a fading effect as they cycle
		ledArr[(patternStep + 1) % numLeds].duty = defaultDuty;
		ledArr[(patternStep + 2) % numLeds].duty = defaultDuty * 5;
		ledArr[(patternStep + 3) % numLeds].duty = defaultDuty;

		startTime = HAL_GetTick();
		patternStep++;
	}
}

void alternating()
{
	static uint32_t startTime = 0;

	if (gpioNotReset)
	{
		for (uint8_t i = 0; i < numLeds; i++)
		{
			ledArr[i].enabled = false;
			ledArr[i].duty = defaultDuty;

			gpioNotReset = false;
		}
	}

	switch(patternStep)
	{
		case 0:
			if (HAL_GetTick() - startTime >= 500)
			{
				for (uint8_t i = 0; i < numLeds; i += 2)
				{
					ledArr[i].enabled = true;
					ledArr[i].duty = defaultDuty;
					ledArr[i+1].enabled = false;
					ledArr[i+1].duty = defaultDuty;
				}
				startTime = HAL_GetTick();
				patternStep++;
			}
			break;
		case 1:
			if (HAL_GetTick() - startTime >= 500)
			{
				for (uint8_t i = 0; i < numLeds; i += 2)
				{
					ledArr[i].enabled = false;
					ledArr[i+1].enabled = true;
				}
				startTime = HAL_GetTick();
				patternStep = 0;
			}
			break;
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
