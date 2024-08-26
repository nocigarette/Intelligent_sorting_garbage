/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bj_motor.h"
#include "Usart_Task.h"
#include "Invert_Garbage_Task.h"
#include "Conveyor_Bj_Task.h"
#include "Drp_Task.h"

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
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
osThreadId_t Conveyor_TaskHandle;
const osThreadAttr_t Conveyor_task_attributes = {
  .name = "Conveyor_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal3,
};

osThreadId_t Invert_taskHandle;
const osThreadAttr_t Invert_task_attributes = {
  .name = "Invert_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal4,
};

osThreadId_t Panel_Dj_TaskHandle;
const osThreadAttr_t Panel_Dj_Task_attributes = {
  .name = "Panel_Dj_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal5,
};

osThreadId_t Drp_TaskHandle;
const osThreadAttr_t Drp_Task_attributes = {
  .name = "Drp_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal6,
};

osThreadId_t tr_Full_TaskHandle;
const osThreadAttr_t tr_Full_Task_attributes = {
  .name = "tr_Full_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal6,
};
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	Conveyor_TaskHandle= osThreadNew(Conveyor_Task,NULL,&Conveyor_task_attributes);
	Invert_taskHandle= osThreadNew(Invert_task,NULL,&Invert_task_attributes);
	Panel_Dj_TaskHandle= osThreadNew(Panel_Dj_Task,NULL,&Panel_Dj_Task_attributes);
	Drp_TaskHandle= osThreadNew(Drp_Task,NULL,&Drp_Task_attributes);
	tr_Full_TaskHandle= osThreadNew(tr_Full_Task,NULL,&tr_Full_Task_attributes);
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

