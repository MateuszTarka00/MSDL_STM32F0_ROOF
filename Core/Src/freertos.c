/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "CO_app_STM32.h"
#include "tim.h"
#include "can.h"
#include "NMT_functions.h"
#include "OD.h"
#include "IO_MappingFunctions.h"
#include "softwareTimer_ms.h"
#include "DigitalInputs.h"
#include "DigitalOutput.h"
#include "iwdg.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define CANOPEN_TASK_DELAY_MS 			1
#define TPDO_REQUESTER_TASK_DELAY_MS 	1
#define INPUT_CHECK_TASK_DELAY_MS		20

#define CANOPEN_ID						11
#define COB_ID							0x480 + CANOPEN_ID
#define TPDO_INDEX						0x1800
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
/* Definitions for inputCheckT */
osThreadId_t inputCheckTHandle;
const osThreadAttr_t inputCheckT_attributes = {
  .name = "inputCheckT",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for canOpenManagerT */
osThreadId_t canOpenManagerTHandle;
const osThreadAttr_t canOpenManagerT_attributes = {
  .name = "canOpenManagerT",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for tpdoRequesterT */
osThreadId_t tpdoRequesterTHandle;
const osThreadAttr_t tpdoRequesterT_attributes = {
  .name = "tpdoRequesterT",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void inputCheck(void *argument);
void canOpenManager(void *argument);
void tpdoRequester(void *argument);

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
  /* creation of inputCheckT */
  inputCheckTHandle = osThreadNew(inputCheck, NULL, &inputCheckT_attributes);

  /* creation of canOpenManagerT */
  canOpenManagerTHandle = osThreadNew(canOpenManager, NULL, &canOpenManagerT_attributes);

  /* creation of tpdoRequesterT */
  tpdoRequesterTHandle = osThreadNew(tpdoRequester, NULL, &tpdoRequesterT_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_inputCheck */
/**
  * @brief  Function implementing the inputCheckT thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_inputCheck */
void inputCheck(void *argument)
{
  /* USER CODE BEGIN inputCheck */
	  /* Infinite loop */
  const OD_entry_t *entry = OD_find(OD, 0x6100);

  for(;;)
  {
	HAL_IWDG_Refresh(&hiwdg);
	for(uint8_t subIndex = 1; subIndex <= OD_CNT_ARR_6100; ++subIndex)
	{
		OD_IO_t io;
		{
			ODR_t result = OD_getSub(entry, subIndex, &io, 0);
		}

		//check if input is enabled
		uint8_t enabledInput = 0;
		uint8_t *identifier = (uint8_t *)io.stream.dataOrig;
		uint8_t state = 0;

		for(uint8_t i = 0; i < 5; ++i)
		{
			if(identifier[i])
			{
				enabledInput = 1;
				break;
			}
		}

		if(enabledInput)
		{
			state = HAL_GPIO_ReadPin(digitalInput[subIndex - 1].port, digitalInput[subIndex - 1].pin);
			identifier[5] = state;
		}

		// check if input has changed
		uint8_t inputChanged = memcmp(virtualInputMapping[subIndex - 1].InputFunctionID, identifier, sizeof(virtualInputMapping[subIndex - 1].InputFunctionID)) == 0;

		if(inputChanged)
		{
			continue;
		}

		memcpy(virtualInputMapping[subIndex - 1].InputFunctionID, identifier, sizeof(virtualInputMapping[subIndex - 1].InputFunctionID));

		if(virtualInputMapping[subIndex - 1].pending) //check if already pending
		{
			continue;
		}

		virtualInputMapping[subIndex - 1].pending = 1;
		++pendingVirtualInputMappings;

	}

	osDelay(pdMS_TO_TICKS(INPUT_CHECK_TASK_DELAY_MS));
  }
  /* USER CODE END inputCheck */
}

/* USER CODE BEGIN Header_canOpenManager */
/**
* @brief Function implementing the canOpenManagerT thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_canOpenManager */
void canOpenManager(void *argument)
{
  /* USER CODE BEGIN canOpenManager */
  OD_extension_t virtualInputMappingExtension = {0, virtualInputMappingRead, OD_writeOriginal, 0};
  ODR_t result = OD_extension_init(OD_find(OD, 0x6010), &virtualInputMappingExtension);

  OD_extension_t virtualOutputMappingExtension = {0, OD_readOriginal, virtualOutputMappingWrite, 0};
  result = OD_extension_init(OD_find(OD, 0x6011), &virtualOutputMappingExtension);

  OD_extension_t outputGroupExtension = {0, OD_readOriginal, outputGroupWrite, 0};
  result  = OD_extension_init(OD_find(OD, 0x6200), &outputGroupExtension);

  OD_extension_t saveParametersExtension = {0, OD_readOriginal, saveParametersWrite, 0};
  result  = OD_extension_init(OD_find(OD, 0x1010), &saveParametersExtension);

  canOpenNodeSTM32.CANHandle = &hcan;
  canOpenNodeSTM32.HWInitFunction = MX_CAN_Init;
  canOpenNodeSTM32.timerHandle = &htim2;
  canOpenNodeSTM32.desiredNodeID = CANOPEN_ID;
  canOpenNodeSTM32.baudrate = 250;

  uint32_t correctTpdo1CobId = COB_ID;

  //Set COB-ID for TPDO
  OD_entry_t *tpdoCommEntry = OD_find(OD, TPDO_INDEX);
  if (tpdoCommEntry != NULL) {
	  OD_IO_t io;

	  // Get subindex 1 — COB-ID
	  if (OD_getSub(tpdoCommEntry, 1, &io, 0) == ODR_OK) {
		  uint32_t *cobIdPtr = (uint32_t *)io.stream.dataOrig;

		  // Disable PDO temporarily (set bit 31)
		  *cobIdPtr |= 0x80000000;

		  // Update COB-ID
		  *cobIdPtr = correctTpdo1CobId;

		  // Re-enable PDO (clear bit 31)
		  *cobIdPtr &= ~0x80000000;
	  }
  }

  canopen_app_init(&canOpenNodeSTM32);
  CO_NMT_initCallbackChanged(canOpenNodeSTM32.canOpenStack->NMT, nmtStateChangedCallback);
  /* Infinite loop */
  for(;;)
  {
	HAL_IWDG_Refresh(&hiwdg);
	HAL_GPIO_WritePin(CAN_OK_GPIO_Port, CAN_OK_Pin , canOpenNodeSTM32.outStatusLEDGreen);
	HAL_GPIO_WritePin(CAN_FAULT_GPIO_Port, CAN_FAULT_Pin, canOpenNodeSTM32.outStatusLEDRed);

	canopen_app_interrupt();
	canopen_app_process();

	ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(CANOPEN_TASK_DELAY_MS));
  }
  /* USER CODE END canOpenManager */
}

/* USER CODE BEGIN Header_tpdoRequester */
/**
* @brief Function implementing the tpdoRequesterT thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_tpdoRequester */
void tpdoRequester(void *argument)
{
  /* USER CODE BEGIN tpdoRequester */
	uint8_t *flags6010 = OD_getFlagsPDO(OD_find(OD,0x6010));
  /* Infinite loop */
  for(;;)
  {
	  HAL_IWDG_Refresh(&hiwdg);
	  osDelay(pdMS_TO_TICKS(TPDO_REQUESTER_TASK_DELAY_MS));

	  if(pendingVirtualInputMappings == 0) //check if there is any virtual input mapping pending
	  {
		  continue;
	  }

	  if(OD_TPDOtransmitted(flags6010, 0) == false) //check if there is any 6010 Od tpdo transmited
	  {
		  continue;
	  }

	  if(canOpenNodeSTM32.canOpenStack->TPDO[0].CANtxBuff->bufferFull == true)
	  {
		  continue;
	  }

	  OD_requestTPDO(flags6010, 0);
  }
  /* USER CODE END tpdoRequester */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

