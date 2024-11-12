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
osThreadId ChassisPIDCalHandle;
osThreadId ScrewPIDCalHandle;
osThreadId StartHandle;
osThreadId ConnectHandle;
osThreadId DebugHandle;
osMessageQId PositionHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void ChassisPIDCalculate(void const * argument);
void ScrewPIDCalTask(void const * argument);
void StartTask(void const * argument);
void ConnectTask(void const * argument);
void DebugTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

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

  /* Create the queue(s) */
  /* definition and creation of Position */
  osMessageQDef(Position, 64, uint16_t);
  PositionHandle = osMessageCreate(osMessageQ(Position), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of ChassisPIDCal */
  osThreadDef(ChassisPIDCal, ChassisPIDCalculate, osPriorityRealtime, 0, 128);
  ChassisPIDCalHandle = osThreadCreate(osThread(ChassisPIDCal), NULL);

  /* definition and creation of ScrewPIDCal */
  osThreadDef(ScrewPIDCal, ScrewPIDCalTask, osPriorityRealtime, 0, 128);
  ScrewPIDCalHandle = osThreadCreate(osThread(ScrewPIDCal), NULL);

  /* definition and creation of Start */
  osThreadDef(Start, StartTask, osPriorityRealtime, 0, 128);
  StartHandle = osThreadCreate(osThread(Start), NULL);

  /* definition and creation of Connect */
  osThreadDef(Connect, ConnectTask, osPriorityHigh, 0, 128);
  ConnectHandle = osThreadCreate(osThread(Connect), NULL);

  /* definition and creation of Debug */
  osThreadDef(Debug, DebugTask, osPriorityNormal, 0, 128);
  DebugHandle = osThreadCreate(osThread(Debug), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_ChassisPIDCalculate */
/**
  * @brief  Function implementing the ChassisPIDCal thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_ChassisPIDCalculate */
__weak void ChassisPIDCalculate(void const * argument)
{
  /* USER CODE BEGIN ChassisPIDCalculate */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ChassisPIDCalculate */
}

/* USER CODE BEGIN Header_ScrewPIDCalTask */
/**
* @brief Function implementing the ScrewPIDCal thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ScrewPIDCalTask */
__weak void ScrewPIDCalTask(void const * argument)
{
  /* USER CODE BEGIN ScrewPIDCalTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ScrewPIDCalTask */
}

/* USER CODE BEGIN Header_StartTask */
/**
* @brief Function implementing the Start thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask */
__weak void StartTask(void const * argument)
{
  /* USER CODE BEGIN StartTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTask */
}

/* USER CODE BEGIN Header_ConnectTask */
/**
* @brief Function implementing the Connect thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ConnectTask */
__weak void ConnectTask(void const * argument)
{
  /* USER CODE BEGIN ConnectTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ConnectTask */
}

/* USER CODE BEGIN Header_DebugTask */
/**
* @brief Function implementing the Debug thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DebugTask */
__weak void DebugTask(void const * argument)
{
  /* USER CODE BEGIN DebugTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END DebugTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
