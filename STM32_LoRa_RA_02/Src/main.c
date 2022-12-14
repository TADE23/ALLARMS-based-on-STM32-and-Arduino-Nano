/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define TIME_BEF_START_ACTION 400
#define USER_BTN_PIN BUTTON_Pin
#define USER_PORT_BTN_PIN GPIOB

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "LoRa.h"

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
LoRa myLoRa;
uint8_t LoRa_state = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//-------------------------------------------------
int _write(int file, uint8_t *ptr, int len) {

	for (int DataIdx = 0; DataIdx < len; DataIdx++) {
		ITM_SendChar(*ptr++);
	}
	return len;
}

//-------------------------------------------------

uint8_t click = 0; //to store num of click
bool click_flug = true;
bool flug_time_press = true; //for don't set new press btn time if it's already set

uint64_t time_press = 0; //time when btn was pressed
bool flug_for_timer = false; //flug for store time when btn will be pressed

uint8_t led_on_command[10] = "Led_On_On";
uint8_t led_off_command[7] = "Led_Off";

uint8_t send_GET_DATA[128] =
		"GET_DATA_FROM_PIDR_2_433_444_FREQUENCY_TEST_WITH_LONG_CONFIRM_DATA_12378978896564132_564654_6545646664%:?%;?:%;(*?:(*?%(12345678";



uint8_t RxBuffer[128] = { };

uint8_t packet_size = 0;

uint8_t led_blink_command[] = "Led_Blink";

void one_btn() {
	//------------------------------------------------------------
	if (!HAL_GPIO_ReadPin(USER_PORT_BTN_PIN, USER_BTN_PIN) && click_flug) {
		if (flug_time_press) {
			flug_for_timer = true;
			//flug_time_press = false;
		}

		click_flug = false;
		click++;
	}
	if (HAL_GPIO_ReadPin(USER_PORT_BTN_PIN, USER_BTN_PIN)) { //only when but is unpress click can be incremented
		click_flug = true;
	}
	if (flug_for_timer) {
		time_press = HAL_GetTick();
		flug_for_timer = false;
	}
	if ((HAL_GetTick() - time_press >= TIME_BEF_START_ACTION)
			&& HAL_GPIO_ReadPin(USER_PORT_BTN_PIN, USER_BTN_PIN)) {
		//-------------------------------
		//action:
		switch (click) {
		case 1:
			LoRa_transmit(&myLoRa, led_on_command, sizeof(led_on_command), 500);
			break;

		case 2:
			LoRa_transmit(&myLoRa, led_off_command, sizeof(led_off_command),
					500);
			break;

		case 3:
			LoRa_transmit(&myLoRa, led_blink_command,
					(sizeof(led_blink_command) - 1), 500);
			break;

		case 4:

			break;

		case 5:

			break;
		}
		//-------------------------------

		//-------------------------------
		//reset:
		flug_for_timer = false;				//initial value
		flug_time_press = true;  			//initial value
		click_flug = true;					//initial value
		click = 0;							//initial value
		//-------------------------------
	}

	//------------------------------------------------------------
}

bool LoRa_intertupt = false;
bool freq = false;

int flag = 0;
int count = 0;
void print_RxBuffer() {
	count++;

	for (int a = 0; a < 40; a++) {
		printf("%c", RxBuffer[a]);
	}
	printf("\n"); //printf("|  myLoRa.frequency = %d\n", myLoRa.frequency);

	//if(count == 10) {
	//	count = 0;
	//flag = 1111;

//		if(freq == false) {
//			freq = true;
//
//			LoRa_gotoMode(&myLoRa, SLEEP_MODE);
//			LoRa_setFrequency(&myLoRa, 433);
//			LoRa_startReceiving(&myLoRa);
//		} else {
//			freq = false;
//
//			LoRa_gotoMode(&myLoRa, SLEEP_MODE);
//			LoRa_setFrequency(&myLoRa, 444);
//			LoRa_startReceiving(&myLoRa);
//		}

	// WORK --------------------------------
//		LoRa_gotoMode(&myLoRa, SLEEP_MODE);
//		LoRa_setFrequency(&myLoRa, 433);
//		LoRa_startReceiving(&myLoRa);
	//--------------------------------------

	// WORK --------------------------------
//		myLoRa.frequency 		 = 433;
//		LoRa_init(&myLoRa);
//		LoRa_startReceiving(&myLoRa);
	//--------------------------------------
	//}
}

void motion_cam_callback() {
	// CHANGE LORA FREQUENCY----------------
	myLoRa.frequency = 444;
	LoRa_init(&myLoRa);
	HAL_Delay(80);
	//--------------------------------------

	print_RxBuffer();

	HAL_Delay(500);

	for (int a = 0; a < 6; a++) {
		LoRa_transmit(&myLoRa, send_GET_DATA, 128, 100);
		HAL_GPIO_TogglePin(GPIOC, LED_PC13_Pin);
		HAL_Delay(250);
	}

	// CHANGE LORA FREQUENCY----------------
	myLoRa.frequency = 433;
	LoRa_init(&myLoRa);
	LoRa_startReceiving(&myLoRa);
	HAL_Delay(80);
	//--------------------------------------

	//HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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
	MX_SPI1_Init();
	/* USER CODE BEGIN 2 */

	HAL_Delay(100);

	myLoRa = newLoRa();

	myLoRa.CS_port = NSS_GPIO_Port;
	myLoRa.CS_pin = NSS_Pin;
	myLoRa.reset_port = RST_GPIO_Port;
	myLoRa.reset_pin = RST_Pin;
	myLoRa.DIO0_port = DIO0_GPIO_Port;
	myLoRa.DIO0_pin = DIO0_Pin;
	myLoRa.hSPIx = &hspi1;

	myLoRa.frequency = 433;

	if (LoRa_init(&myLoRa) == LORA_OK) {
		LoRa_state = 1;
	}

	LoRa_startReceiving(&myLoRa);

	HAL_Delay(100);

	//LoRa_transmit(&myLoRa, led_on_command, sizeof(led_on_command), 500);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

//	  LoRa_transmit(&myLoRa, led_on_command, sizeof(led_on_command), 500);
//	  HAL_GPIO_TogglePin(GPIOC, LED_PC13_Pin);
//	  HAL_Delay(500);
		//bool motion_cam_callback_flug = false;
		if (LoRa_intertupt == true) {
			LoRa_intertupt = false;

			if (memcmp(RxBuffer, "ALLARM", 6) == 0) {
				//motion_cam_callback_flug = true;
				HAL_GPIO_TogglePin(GPIOC, LED_PC13_Pin);

				motion_cam_callback();
			}

			//print_RxBuffer();
		}

	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == DIO0_Pin) {
		//HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);

		LoRa_intertupt = true;

		LoRa_receive(&myLoRa, RxBuffer, 128);
	}
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
