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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//DAY LA CODE CHO LED 7 DOAN COMMON-ANODE (DUONG CHUNG)
uint8_t seg[10] =
{
	0xC0, //0
	0xF9, //1
	0xA4, //2
	0xB0, //3
	0x99, //4
	0x92, //5
	0x82, //6
	0xF8, //7
	0x80, //8
	0x90, //9
};

typedef struct
{
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
} led7;

typedef struct
{
	led7 led[7]; // 7 chan chung a b c d e f g
	led7 COM1;  // nguon cho led 1 (led hang` chuc)
	led7 COM2;  // nguon cho led 2 (led hang` don vi)
}LED7_;

void Displayyy(LED7_ *hled, uint8_t num) //ham display xai chung cho 2 den`
{
	uint8_t data= seg[num];

	for (int i = 0; i<7 ; i++)
	{
		uint8_t bit= (data >> i) & 0x01;
		HAL_GPIO_WritePin(
				hled->led[i].GPIOx,
				hled->led[i].GPIO_Pin,
				bit);
	}
}

void Displayled1(LED7_ *hled, uint8_t num) //Hien thi len led hang chuc
{
	HAL_GPIO_WritePin(
	            hled->COM2.GPIOx,
	            hled->COM2.GPIO_Pin,
	            GPIO_PIN_RESET); // tat led hang chuc
	Displayyy(hled, num);
	HAL_GPIO_WritePin(
	            hled->COM1.GPIOx,
	            hled->COM1.GPIO_Pin,
	            GPIO_PIN_SET); //bat led hang don vi
}

void Displayled2(LED7_ *hled, uint8_t num) //Hien thi len led hang don vi
{
	HAL_GPIO_WritePin(
	            hled->COM1.GPIOx,
	            hled->COM1.GPIO_Pin,
	            GPIO_PIN_RESET);
	Displayyy(hled, num);
	HAL_GPIO_WritePin(
	            hled->COM2.GPIOx,
	            hled->COM2.GPIO_Pin,
	            GPIO_PIN_SET);
}

void Display2led(LED7_ *hled, uint8_t num) //Ket hop 2 led
{
	uint8_t chuc = num / 10;
	uint8_t donvi = num % 10;

	Displayled1(hled, chuc);
	HAL_Delay(1);

	Displayled2(hled, donvi);
	HAL_Delay(1);
}

// CAC BUTTON CHUC NANG
uint8_t dem = 0;
uint8_t running = 0;
uint32_t now = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	// CHONG DEBOUNCE
	static uint32_t lastTick = 0;

	if(HAL_GetTick() - lastTick < 200) return;
	lastTick = HAL_GetTick();

	if (GPIO_Pin == startstop_Pin)
	{
		running = !running;
	}

	if (GPIO_Pin == reset_Pin)
	{
		dem = 0;
		running = 0;
		now = HAL_GetTick();

		HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, 0);
	}
}


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
  	LED7_ initled;

	//DOAN A
	initled.led[0].GPIOx = GPIOA;
	initled.led[0].GPIO_Pin = GPIO_PIN_0;
	// DOAN B
	initled.led[1].GPIOx = GPIOA;
	initled.led[1].GPIO_Pin = GPIO_PIN_1;
	//DOAN C
	initled.led[2].GPIOx = GPIOA;
	initled.led[2].GPIO_Pin = GPIO_PIN_2;
	//DOAN D
	initled.led[3].GPIOx = GPIOA;
	initled.led[3].GPIO_Pin = GPIO_PIN_3;
	//DOAN E
	initled.led[4].GPIOx = GPIOA;
	initled.led[4].GPIO_Pin = GPIO_PIN_4;
	//DOAN F
	initled.led[5].GPIOx = GPIOA;
	initled.led[5].GPIO_Pin = GPIO_PIN_5;
	//DOAN G
	initled.led[6].GPIOx = GPIOA;
	initled.led[6].GPIO_Pin = GPIO_PIN_6;
	//COM 1
	initled.COM1.GPIOx = GPIOB;
	initled.COM1.GPIO_Pin = GPIO_PIN_0;
	//COM 2
	initled.COM2.GPIOx = GPIOB;
	initled.COM2.GPIO_Pin = GPIO_PIN_1;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  Display2led(&initled, dem);

	  if (running)
	  {
		  if(HAL_GetTick() - now >= 500)
		  {
			  now = HAL_GetTick();

			  if (dem < 99)
			  {
				  dem++;
			  }
			  else
			  {
				  running = 0;
				  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, 1);
			  }
		  }
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|buzzer_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 PA5 PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 buzzer_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|buzzer_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : startstop_Pin reset_Pin */
  GPIO_InitStruct.Pin = startstop_Pin|reset_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
