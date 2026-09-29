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
// Struct for storing LED info
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
// Timer counter for interrupt PWM
volatile uint16_t g_tickCounter = 0;
volatile uint8_t g_pattern = 0;
// 10%, 20%, ..., 100%
uint16_t g_stepRes = 10;
// In ms, instead of duty % for less compute in interrupt cycles
uint16_t g_defaultDuty = 1;
uint8_t g_numLeds = 12;
volatile uint8_t g_patternStep = 0;
volatile bool g_gpioNotReset = true;

volatile LED_t ledArr[12];


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */
void writeGPIO(uint8_t ledNum, bool enabled);
void pollButtons();
void alternating();
void ledChaser();
void sparkling();
void groupedLedChaser();
void breathing();
void comet();
void meteorShower();
void patternCycle();

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
	// Poll if patterns changed
	pollButtons();

	// Pick non-blocking pattern based of button combination
	switch(g_pattern)
	{
		case 0:
			comet();
			break;
		case 1:
			alternating();
			break;
		case 2:
			sparkling();
			break;
		case 3:
			groupedLedChaser();
			break;
		case 4:
			breathing();
			break;
		case 5:
			ledChaser();
			break;
		case 6:
			meteorShower();
			break;
		case 7:
			patternCycle();
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
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
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
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

// ISR TIM3 for LED PWM
// Timer: Interrupt rate 1kHz, 1ms
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	// Check instance member to see if tim3 has an IRQ
	if (htim->Instance == TIM3)
	{
		//Reset tick counter if reached 100%
		if (g_tickCounter >= g_stepRes)
		{
			g_tickCounter = 0;
		}

		// Cycle through LEDs
		for (uint8_t i = 0; i < g_numLeds; i++)
		{
			// If less than duty cycle set high else low
			if ((g_tickCounter < ledArr[i].duty) && ledArr[i].enabled)
			{
				writeGPIO(i + 1, true);
			}
			else
			{
				writeGPIO(i + 1, false);
			}
		}

		// Increment tick for each timer interrupt
		g_tickCounter++;
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

// Helper function for sparkling pattern to pick a new random index for LEDs
static void reassignLedIdx(uint8_t randomLedIdx[3], uint8_t slot)
{
	bool notFarEnoughApart = true;
	// Loop for checking new idx until led idxs are spaced out
	while (notFarEnoughApart)
	{
		// get random num from 0-11
		randomLedIdx[slot] = rand() % g_numLeds;
		notFarEnoughApart = false;

		for (uint8_t j = 0; j < 3; j++)
		{
			// Don't check idx difference for same idx
			if (j == slot)
			{
				continue;
			}

			// Difference between new led idx and others idxs
			uint8_t wrapDiff = g_numLeds - abs(randomLedIdx[slot] - randomLedIdx[j]);

			if (wrapDiff > g_numLeds / 2)
			{
				wrapDiff = g_numLeds - wrapDiff;
			}

			// Check if difference is more than 1
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
	if (g_gpioNotReset)
	{
		for (uint8_t i = 0; i < g_numLeds; i++)
		{
			ledArr[i].enabled = false;
			ledArr[i].duty = g_defaultDuty;
		}

		// Initial seed for all three slots (same spacing rule as before,
		// checking only against already-assigned slots since this only
		// runs once at reset)
		for (uint8_t i = 0; i < 3; i++)
		{
			randomLedIdx[i] = rand() % g_numLeds;

			for (uint8_t j = 0; j < i; j++)
			{
				bool notFarEnoughApart = true;
				while (notFarEnoughApart)
				{
					uint8_t wrapDiff = g_numLeds - abs(randomLedIdx[i] - randomLedIdx[j]);

					if (wrapDiff > g_numLeds / 2)
						wrapDiff = g_numLeds - wrapDiff;

					if (wrapDiff <= 1)
					{
						randomLedIdx[i] = rand() % g_numLeds;
					}
					else
					{
						notFarEnoughApart = false;
					}
				}
			}
		}

		g_gpioNotReset = false;
		firstCycle = true;
	}

	// Delay for each brightness increase
	uint16_t brightnessDelay = 300;

	switch(g_patternStep)
	{
		case 0:
			if (HAL_GetTick() - startTime >= brightnessDelay)
			{

				ledArr[randomLedIdx[0]].enabled = true;
				ledArr[randomLedIdx[0]].duty = g_defaultDuty;

				startTime = HAL_GetTick();
				if (firstCycle)
				{
					g_patternStep += 3;
				}
				else
				{
					g_patternStep++;
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
				g_patternStep++;
			}
			break;
		case 2:
			if (HAL_GetTick() - startTime >= 100)
			{

				ledArr[randomLedIdx[2]].duty = g_defaultDuty;

				startTime = HAL_GetTick();
				g_patternStep++;
			}
			break;
		case 3:
			if (HAL_GetTick() - startTime >= brightnessDelay)
			{
				ledArr[randomLedIdx[0]].duty = g_defaultDuty * 2;

				startTime = HAL_GetTick();
				g_patternStep++;
			}
			break;
		case 4:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[1]].duty = g_defaultDuty;
				ledArr[randomLedIdx[1]].enabled = g_defaultDuty;

				startTime = HAL_GetTick();
				if (firstCycle)
				{
					g_patternStep += 2;
					firstCycle = false;
				}
				else
				{
					g_patternStep++;
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
				g_patternStep++;
			}
			break;
		case 6:
			if (HAL_GetTick() - startTime >= brightnessDelay)
			{
				ledArr[randomLedIdx[0]].duty = g_defaultDuty * 3;

				startTime = HAL_GetTick();
				g_patternStep++;
			}
			break;
		case 7:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[1]].duty = g_defaultDuty * 2;

				startTime = HAL_GetTick();
				g_patternStep++;
			}
			break;
		case 8:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[2]].duty = g_defaultDuty;
				ledArr[randomLedIdx[2]].enabled = true;

				startTime = HAL_GetTick();
				g_patternStep++;
			}
			break;
		case 9:
			if (HAL_GetTick() - startTime >= brightnessDelay)
			{
				ledArr[randomLedIdx[0]].duty = g_defaultDuty * 2;

				startTime = HAL_GetTick();
				g_patternStep++;
			}
			break;
		case 10:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[1]].duty = g_defaultDuty * 3;

				startTime = HAL_GetTick();
				g_patternStep++;
			}
			break;
		case 11:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[2]].duty = g_defaultDuty * 2;

				startTime = HAL_GetTick();
				g_patternStep++;
			}
			break;
		case 12:
			if (HAL_GetTick() - startTime >= brightnessDelay)
			{
				ledArr[randomLedIdx[0]].duty = g_defaultDuty;

				startTime = HAL_GetTick();
				g_patternStep++;
			}
			break;
		case 13:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[1]].duty = g_defaultDuty * 2;

				startTime = HAL_GetTick();
				g_patternStep++;
			}
			break;
		case 14:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[2]].duty = g_defaultDuty * 3;

				startTime = HAL_GetTick();
				g_patternStep++;
			}
			break;
		case 15:
			if (HAL_GetTick() - startTime >= brightnessDelay)
			{
				ledArr[randomLedIdx[0]].duty = g_defaultDuty;

				startTime = HAL_GetTick();
				g_patternStep++;
			}
			break;
		case 16:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[1]].duty = g_defaultDuty * 2;

				startTime = HAL_GetTick();
				g_patternStep++;
			}
			break;
		case 17:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[2]].duty = g_defaultDuty * 3;

				startTime = HAL_GetTick();
				g_patternStep++;
			}
			break;
		case 18:
			if (HAL_GetTick() - startTime >= brightnessDelay)
			{

				ledArr[randomLedIdx[0]].enabled = false;
				// idx[0] just turned off -> pick its next target now
				reassignLedIdx(randomLedIdx, 0);

				startTime = HAL_GetTick();
				g_patternStep++;
			}
			break;
		case 19:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[1]].duty = g_defaultDuty;

				startTime = HAL_GetTick();
				g_patternStep++;
			}
			break;
		case 20:
			if (HAL_GetTick() - startTime >= 100)
			{
				ledArr[randomLedIdx[2]].duty = g_defaultDuty * 2;

				startTime = HAL_GetTick();
				g_patternStep = 0;
			}
			break;
	}
}

void ledChaser()
{
	static uint32_t startTime = 0;

	if (g_gpioNotReset)
	{
		for (uint8_t i = 0; i < g_numLeds; i++)
		{
			ledArr[i].enabled = false;
			ledArr[i].duty = g_defaultDuty;

			g_gpioNotReset = false;
		}
	}

	uint16_t chaseDelay = 100;

	if (HAL_GetTick() - startTime >= chaseDelay)
	{
		// Reset led idx once done one full cycle
		if (g_patternStep >= g_numLeds)
		{
			g_patternStep = 0;
		}

		// Turn off last LED from last idx
		ledArr[g_patternStep].enabled = false;
		// Turn on middle led and leds either side of it
		ledArr[(g_patternStep + 1) % g_numLeds].enabled = true;
		ledArr[(g_patternStep + 2) % g_numLeds].enabled = true;
		ledArr[(g_patternStep + 3) % g_numLeds].enabled = true;

		// Make middle LED brightest and leds on either side dimmer
		// for a fading effect as they cycle
		ledArr[(g_patternStep + 1) % g_numLeds].duty = g_defaultDuty;
		ledArr[(g_patternStep + 2) % g_numLeds].duty = g_defaultDuty * 5;
		ledArr[(g_patternStep + 3) % g_numLeds].duty = g_defaultDuty;

		startTime = HAL_GetTick();
		g_patternStep++;
	}
}

void alternating()
{
	static uint32_t startTime = 0;

	if (g_gpioNotReset)
	{
		for (uint8_t i = 0; i < g_numLeds; i++)
		{
			ledArr[i].enabled = false;
			ledArr[i].duty = g_defaultDuty;

			g_gpioNotReset = false;
		}
	}

	switch(g_patternStep)
	{
		case 0:
			if (HAL_GetTick() - startTime >= 500)
			{
				for (uint8_t i = 0; i < g_numLeds; i += 2)
				{
					ledArr[i].enabled = true;
					ledArr[i].duty = g_defaultDuty;
					ledArr[i+1].enabled = false;
					ledArr[i+1].duty = g_defaultDuty;
				}
				startTime = HAL_GetTick();
				g_patternStep++;
			}
			break;
		case 1:
			if (HAL_GetTick() - startTime >= 500)
			{
				for (uint8_t i = 0; i < g_numLeds; i += 2)
				{
					ledArr[i].enabled = false;
					ledArr[i+1].enabled = true;
				}
				startTime = HAL_GetTick();
				g_patternStep = 0;
			}
			break;
	}
}

void groupedLedChaser()
{
	static uint32_t startTime = 0;

	if (g_gpioNotReset)
	{
		for (uint8_t i = 0; i < g_numLeds; i++)
		{
			ledArr[i].enabled = false;
			ledArr[i].duty = g_defaultDuty;
		}

		g_gpioNotReset = false;
	}

	uint16_t groupDelay = 500;

	if (HAL_GetTick() - startTime >= groupDelay)
	{
		// Turn all LEDs off first
		for (uint8_t i = 0; i < g_numLeds; i++)
		{
			ledArr[i].enabled = false;
		}

		// Turn on the current group of 3
		uint8_t startLed = g_patternStep * 3;

		for (uint8_t i = 0; i < 3; i++)
		{
			ledArr[startLed + i].enabled = true;
			ledArr[startLed + i].duty = g_defaultDuty;
		}

		startTime = HAL_GetTick();

		// Move to next group
		g_patternStep++;

		// 4 groups: 1-3, 4-6, 7-9, 10-12
		if (g_patternStep >= g_numLeds / 3)
		{
			g_patternStep = 0;
		}
	}
}

void breathing()
{
	static uint32_t startTime = 0;

	if (g_gpioNotReset)
	{
		for (uint8_t i = 0; i < g_numLeds; i++)
		{
			ledArr[i].enabled = true;
			ledArr[i].duty = 0;
		}

		g_gpioNotReset = false;
		g_patternStep = 0;
	}

	uint16_t breathingDelay = 50;

	if (HAL_GetTick() - startTime >= breathingDelay)
	{
		switch (g_patternStep)
		{
			// Increasing brightness
			case 0:
				for (uint8_t i = 0; i < g_numLeds; i++)
				{
					ledArr[i].duty++;

					if (ledArr[i].duty >= g_stepRes)
					{
						ledArr[i].duty = g_stepRes;
						g_patternStep = 1;
					}
				}
				break;

			// Decreasing brightness
			case 1:
				for (uint8_t i = 0; i < g_numLeds; i++)
				{
					if (ledArr[i].duty > 0)
					{
						ledArr[i].duty--;
					}
					else
					{
						g_patternStep = 0;
					}
				}
				break;
		}

		startTime = HAL_GetTick();
	}
}

void comet()
{
	static uint32_t startTime = 0;

	if (g_gpioNotReset)
	{
		for (uint8_t i = 0; i < g_numLeds; i++)
		{
			ledArr[i].enabled = false;
			ledArr[i].duty = g_defaultDuty;
		}

		g_gpioNotReset = false;
		g_patternStep = 0;
	}

	uint16_t cometDelay = 100;

	if (HAL_GetTick() - startTime >= cometDelay)
	{
		// Turn all LEDs off
		for (uint8_t i = 0; i < g_numLeds; i++)
		{
			ledArr[i].enabled = false;
			ledArr[i].duty = g_defaultDuty;
		}

		// Current LED
		uint8_t head = g_patternStep;

		// Head - brightest
		ledArr[head].enabled = true;
		ledArr[head].duty = g_defaultDuty * 5;

		// Tail - medium brightness
		ledArr[(head + g_numLeds - 1) % g_numLeds].enabled = true;
		ledArr[(head + g_numLeds - 1) % g_numLeds].duty = g_defaultDuty * 3;

		// Tail - dimmer
		ledArr[(head + g_numLeds - 2) % g_numLeds].enabled = true;
		ledArr[(head + g_numLeds - 2) % g_numLeds].duty = g_defaultDuty * 2;

		// Tail - dimmest
		ledArr[(head + g_numLeds - 3) % g_numLeds].enabled = true;
		ledArr[(head + g_numLeds - 3) % g_numLeds].duty = g_defaultDuty;

		// Move comet
		g_patternStep++;

		if (g_patternStep >= g_numLeds)
		{
			g_patternStep = 0;
		}

		startTime = HAL_GetTick();
	}
}

void meteorShower()
{
	static uint32_t startTime = 0;
	static uint8_t head1 = 0;
	static uint8_t head2 = 5;
	static uint8_t head3 = 9;

	uint16_t meteorDelay = 120;

	if (g_gpioNotReset)
	{
		for (uint8_t i = 0; i < g_numLeds; i++)
		{
			ledArr[i].enabled = false;
			ledArr[i].duty = g_defaultDuty;
		}

		head1 = 0;
		head2 = 5;
		head3 = 9;

		g_patternStep = 0;
		g_gpioNotReset = false;
	}

	if (HAL_GetTick() - startTime >= meteorDelay)
	{
		// Turn everything off
		for (uint8_t i = 0; i < g_numLeds; i++)
		{
			ledArr[i].enabled = false;
			ledArr[i].duty = g_defaultDuty;
		}

		// Meteor 1
		ledArr[head1].enabled = true;
		ledArr[head1].duty = g_stepRes;

		ledArr[(head1 + g_numLeds - 1) % g_numLeds].enabled = true;
		ledArr[(head1 + g_numLeds - 1) % g_numLeds].duty = g_defaultDuty * 3;

		ledArr[(head1 + g_numLeds - 2) % g_numLeds].enabled = true;
		ledArr[(head1 + g_numLeds - 2) % g_numLeds].duty = g_defaultDuty;

		// Meteor 2
		ledArr[head2].enabled = true;
		ledArr[head2].duty = g_stepRes;

		ledArr[(head2 + g_numLeds - 1) % g_numLeds].enabled = true;
		ledArr[(head2 + g_numLeds - 1) % g_numLeds].duty = g_defaultDuty * 3;

		ledArr[(head2 + g_numLeds - 2) % g_numLeds].enabled = true;
		ledArr[(head2 + g_numLeds - 2) % g_numLeds].duty = g_defaultDuty;

		// Meteor 3
		ledArr[head3].enabled = true;
		ledArr[head3].duty = g_stepRes;

		ledArr[(head3 + g_numLeds - 1) % g_numLeds].enabled = true;
		ledArr[(head3 + g_numLeds - 1) % g_numLeds].duty = g_defaultDuty * 3;

		ledArr[(head3 + g_numLeds - 2) % g_numLeds].enabled = true;
		ledArr[(head3 + g_numLeds - 2) % g_numLeds].duty = g_defaultDuty;

		// Move meteors
		head1++;
		head2++;
		head3++;

		// Wrap around
		if (head1 >= g_numLeds)
			head1 = 0;

		if (head2 >= g_numLeds)
			head2 = 0;

		if (head3 >= g_numLeds)
			head3 = 0;

		startTime = HAL_GetTick();
	}
}

void patternCycle()
{
	static uint32_t startTime = 0;
	static uint8_t currentPattern = 0;

	uint16_t patternDelay = 5000;

	if (HAL_GetTick() - startTime >= patternDelay)
	{
		// Move to next g_pattern
		currentPattern++;

		if (currentPattern >= 7)
		{
			currentPattern = 0;
		}

		// Reset g_pattern state
		g_patternStep = 0;
		g_gpioNotReset = true;

		// Prevent the individual g_pattern from being selected
		// through the global g_pattern variable
		g_pattern = 7;

		startTime = HAL_GetTick();
	}

	// Run current g_pattern
	switch (currentPattern)
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

		case 3:
			groupedLedChaser();
			break;

		case 4:
			breathing();
			break;

		case 5:
			comet();
			break;

		case 6:
			meteorShower();
			break;
	}
}

void pollButtons()
{
    uint8_t newPattern = 0;

    // PA5 = bit 0
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_RESET)
    {
        newPattern |= (1 << 0);
    }

    // PA7 = bit 1
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_RESET)
    {
        newPattern |= (1 << 1);
    }

    // PB1 = bit 2
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET)
    {
        newPattern |= (1 << 2);
    }

    // Only reset the g_pattern when the selected pattern changes
    if (newPattern != g_pattern)
    {
        g_pattern = newPattern;
        g_patternStep = 0;
        g_gpioNotReset = true;
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
