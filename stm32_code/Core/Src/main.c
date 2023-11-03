/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd1602.h"
#include "key.h"
#include "pid.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

//测转速
_Bool Capture_Flag = 0;
_Bool Capture_Start_Flag = 0;
uint32_t cnt = 0;
uint32_t update_cnt = 0;
char Uart_Str[8];
float all_cnt = 0;

//LED显示屏
char Lcd_Str1[30];
char Lcd_Str2[30];


//按键变量定义区
uint8_t ucKey_Val = 0, ucKey_Down, ucKey_Up, ucKey_Old;

//时间减速器
__IO uint32_t Lcd_Proc_Systick = 0;
__IO uint32_t Usart_Proc_Systick = 0;
__IO uint32_t Key_Proc_Systick = 0;
__IO uint32_t Motor_Proc_Systick = 0;
__IO uint32_t Led_Proc_Systick = 0;

//电机变量
uint16_t aim_h = 300; // 单位为cm
uint16_t now_h = 0;
float  count = 0;
float n_velocity = 0;
_Bool floor_start = 0;
_Bool motor_start = 0;
uint8_t aim_floor = 0;



//uint16_t  new_count = 0;


//PID中变量
float control_val;


uint16_t i = 0;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Lcd_Proc(void);
void Key_Proc(void);
void Motor_Proc(void);
void Led_Proc(void);
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
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	PID_param_init();
	LCD_Init();
  
	
	//HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	
	//HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		Key_Proc();
		Lcd_Proc();
		Led_Proc();
		Motor_Proc();
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void Led_Proc()
{
	if((uwTick -  Led_Proc_Systick)<50)	return;//减速函数
	Led_Proc_Systick = uwTick;

	if((now_h - pid_location.target_val)<2)
	{
		switch(aim_floor)
		{
			case 1:
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
			case 2:
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
			case 3:
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
			case 4:
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);			
		}
	}
}


void Key_Proc()
{
	if((uwTick -  Key_Proc_Systick)<50)	return;//减速函数
	Key_Proc_Systick = uwTick;
	if(floor_start == 0)
	{
		ucKey_Val = Velocity_Key_Scan();
		ucKey_Down = ucKey_Val & (ucKey_Old ^ ucKey_Val); 
		ucKey_Up = ~ucKey_Val & (ucKey_Old ^ ucKey_Val);	
		ucKey_Old = ucKey_Val;
		
		//根据按键设定速度
		if(ucKey_Down == 1)
		{
			pid_speed.target_val = 1.000;
			floor_start = 1;
		}
		else if (ucKey_Down == 2)
		{
			pid_speed.target_val = 1.6666;
			floor_start = 1;
		}
		else if (ucKey_Down == 3)
		{
			pid_speed.target_val = 2;
			floor_start = 1;
		}
	}
	else 	//根据按键设定目标楼层
	{
		ucKey_Val = Floor_Key_Scan();
		ucKey_Down = ucKey_Val & (ucKey_Old ^ ucKey_Val); 
		ucKey_Up = ~ucKey_Val & (ucKey_Old ^ ucKey_Val);	
		ucKey_Old = ucKey_Val;		
		
		if(ucKey_Down != 0)
		{
			aim_floor ^= 1<<ucKey_Down;
			motor_start = 1;
		}
		
	}
}


void Motor_Proc()
{
	if((uwTick -  Motor_Proc_Systick)<10)	return;//减速函数
	Motor_Proc_Systick = uwTick;
	if(motor_start != 0)
	{
		control_val = location_pid_realize(&pid_location, now_h); 
		speed_val_protect(&pid_speed, &control_val);  
		count = speed_pid_realize(&pid_speed, n_velocity);
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, ((__HAL_TIM_GetAutoreload(&htim2)+1)*(count/100) - 1));
	}
}




void Lcd_Proc()
{
	if((uwTick - Lcd_Proc_Systick) <10) return;
	Lcd_Proc_Systick = uwTick;
	//sprintf((char *)Lcd_Str1, "H:%d, T:%.3f    N:%.3f,T:%.2f",now_h, all_cnt/1000, n_velocity, pid_speed.target_val);
	if(n_velocity < 0.1)
		n_velocity = 0;
	sprintf((char *)Lcd_Str1, "%d:%.3f:%.3f_",now_h, all_cnt/1000, n_velocity);
	LCD_Puts(0,0, (char *)Lcd_Str1);	
	
	//sprintf((char *)Uart_Str, "%.2f_%d_%.2f", pid_location.err ,now_h, pid_speed.err);
	HAL_UART_Transmit(&huart1, (char *)Lcd_Str1, strlen((char *)Lcd_Str1), 50);	
	
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
		if(htim->Instance == TIM3)
		{
			update_cnt++;
		}
		//else if(htim->Instance == TIM2)
		//{
		//	sprintf((char *)Lcd_Str, "%d_", cnt);
		//	HAL_UART_Transmit(&huart1, (char *)Lcd_Str, strlen((char *)Lcd_Str), 5);		
		//}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if( htim->Instance == TIM3 )
	{
		if(Capture_Flag == 0)
		{
			HAL_TIM_IC_Stop(&htim3, TIM_CHANNEL_1);
		  //__HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			Capture_Flag = 1;
			update_cnt = 0;
			__HAL_TIM_SetCounter(&htim3, 0);
			HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
		}
		else if(Capture_Flag == 1)
		{
			cnt = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1) + update_cnt * 65535; //- cnt + update_cnt * 65535;
			all_cnt += cnt;
			HAL_TIM_IC_Stop(&htim3, TIM_CHANNEL_1);
		  //__HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);	
			//HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);	
			now_h += pid_speed.direction * 1; //经过一个分辨率电梯缆绳所经过的路程
			n_velocity = (50/(3*(float)cnt));
			Capture_Flag = 0;
			update_cnt = 0;		
			HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
		}
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

