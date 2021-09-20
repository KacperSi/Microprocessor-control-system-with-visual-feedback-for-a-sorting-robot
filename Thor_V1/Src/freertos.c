/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "my_header.h"
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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for TCP_connection */
osThreadId_t TCP_connectionHandle;
const osThreadAttr_t TCP_connection_attributes = {
  .name = "TCP_connection",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 4
};
/* Definitions for reszta */
osThreadId_t resztaHandle;
const osThreadAttr_t reszta_attributes = {
  .name = "reszta",
  .priority = (osPriority_t) osPriorityBelowNormal,
  .stack_size = 1024 * 4
};
/* Definitions for UART_communicat */
osThreadId_t UART_communicatHandle;
const osThreadAttr_t UART_communicat_attributes = {
  .name = "UART_communicat",
  .priority = (osPriority_t) osPriorityBelowNormal,
  .stack_size = 1024 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartTCPcommunication(void *argument);
void my_main(void *argument);
void UART_listening(void *argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of TCP_connection */
  TCP_connectionHandle = osThreadNew(StartTCPcommunication, NULL, &TCP_connection_attributes);

  /* creation of reszta */
  resztaHandle = osThreadNew(my_main, NULL, &reszta_attributes);

  /* creation of UART_communicat */
  UART_communicatHandle = osThreadNew(UART_listening, NULL, &UART_communicat_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartTCPcommunication */
/**
  * @brief  Function implementing the TCP_connection thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartTCPcommunication */
void StartTCPcommunication(void *argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN StartTCPcommunication */
  server_TCP();
  /* Infinite loop */
/*  for(;;)
  {
    osDelay(1);
  }*/
  /* USER CODE END StartTCPcommunication */
}

/* USER CODE BEGIN Header_my_main */
/**
* @brief Function implementing the reszta thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_my_main */
void my_main(void *argument)
{
  /* USER CODE BEGIN my_main */

  /* Infinite loop */
  for(;;)
  {

	    for (int i = 1; i <= 5; i++) {
	 	  		  int o=main_axis[i-1].steps_counter;
	 	  		  int f=0;
	 	  	  }
	 	  	  int o=main_axis[0].steps_counter;
	 	  	  		  int f=0;


	 	  		if (activation_table[11] == 1) {


	 	  				regulator();

	 	  		}
	 	  	  if(activation_table[13]==1 && activation_table[10] == 1)
	 	  	  {
	 	  		  home_position();

	 	  	  }
	 	  	  //one xies movement
	 	  	  if(activation_table[9]==0)
	 	  	  {
	 	  	  HAL_GPIO_WritePin(in_out_table[0].port,in_out_table[0].pin, GPIO_PIN_RESET);
	 	  		   for(int i=1;i<=6;i++)
	 	  		   {
	 	  			   if(activation_table[i]==1)
	 	  			   {
	 	  			   	axis_enable(i);
	 	  			   }
	 	  		   }


	 	  	  }
	 	  		if (activation_table[12] == 1) {
	 	  			if (activation_table[7] == 1 && TIM3->CCR1 < 1000) {
	 	  				TIM3->CCR1=TIM3->CCR1+5;
	 	  			}
	 	  			if (activation_table[8] == 1 && TIM3->CCR1 > 0) {
	 	  				TIM3->CCR1=TIM3->CCR1-5;
	 	  			}
	 	  			HAL_Delay(25);
	 	  		}
	 	  		for (int i = 1; i <= 5; i++) {
	 	  			 	  	 			if (activation_table[i] == 1 && ((activation_table[7] == 1 || activation_table[8] == 1)&&activation_table[10] == 1
	 	  			 	  	 					||activation_table[11] == 1||activation_table[13]==1)) { //potworek, ogarnij to
	 	  			 	  	 				for (int j = 0; j <= 1; j++) {
	 	  			 	  	 					if(main_axis[i - 1].count_of_motors>j ){
	 	  			 	  	 					HAL_GPIO_TogglePin(in_out_table[main_axis[i - 1].morots[j] + 8].port,in_out_table[main_axis[i - 1].morots[j] + 8].pin);
	 	  			 	  	 					}
	 	  			 	  	 				}

	 	  			 	  	 				if (main_axis[i-1].full_step == 0) {
	 	  			 	  	 					if (main_axis[i-1].direction == 1) {
	 	  			 	  	 						main_axis[i-1].steps_counter--;
	 	  			 	  	 					}
	 	  			 	  	 					if (main_axis[i-1].direction == 0) {
	 	  			 	  	 						main_axis[i-1].steps_counter++;;
	 	  			 	  	 					}
	 	  			 	  	 					main_axis[i-1].full_step = 1;
	 	  			 	  	 				} else {
	 	  			 	  	 					main_axis[i-1].full_step = 0;
	 	  			 	  	 				}

	 	  			 	  	 			}
	 	  			 	  	 		}

    osDelay(10);
  }
  /* USER CODE END my_main */
}

/* USER CODE BEGIN Header_UART_listening */
/**
* @brief Function implementing the UART_communicat thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_UART_listening */
void UART_listening(void *argument)
{
  /* USER CODE BEGIN UART_listening */
  /* Infinite loop */
  for(;;)
  {
	  if (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_RXNE) == SET)
	  {
	      HAL_UART_Receive(&huart3, &received, 1, 1);
	      activation(received);
	  }
    osDelay(1);
  }
  /* USER CODE END UART_listening */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
