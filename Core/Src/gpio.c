/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, OUT6_Pin|OUT5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, OUT13_Pin|CAN_OK_Pin|OUT8_Pin|OUT7_Pin
                          |OUT12_Pin|OUT11_Pin|OUT10_Pin|OUT9_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, OUT14_Pin|OUT15_Pin|OUT16_Pin|CAN_FAULT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : OUT6_Pin OUT5_Pin */
  GPIO_InitStruct.Pin = OUT6_Pin|OUT5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : IN7_Pin IN8_Pin IN9_Pin IN3_Pin
                           IN5_Pin IN6_Pin IN4_Pin IN1_Pin */
  GPIO_InitStruct.Pin = IN7_Pin|IN8_Pin|IN9_Pin|IN3_Pin
                          |IN5_Pin|IN6_Pin|IN4_Pin|IN1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : IN2_Pin IN10_Pin IN11_Pin IN12_Pin
                           IN13_Pin IN14_Pin IN16_Pin IN15_Pin */
  GPIO_InitStruct.Pin = IN2_Pin|IN10_Pin|IN11_Pin|IN12_Pin
                          |IN13_Pin|IN14_Pin|IN16_Pin|IN15_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : OUT13_Pin CAN_OK_Pin OUT8_Pin OUT7_Pin
                           OUT12_Pin OUT11_Pin OUT10_Pin OUT9_Pin */
  GPIO_InitStruct.Pin = OUT13_Pin|CAN_OK_Pin|OUT8_Pin|OUT7_Pin
                          |OUT12_Pin|OUT11_Pin|OUT10_Pin|OUT9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : OUT14_Pin OUT15_Pin OUT16_Pin CAN_FAULT_Pin */
  GPIO_InitStruct.Pin = OUT14_Pin|OUT15_Pin|OUT16_Pin|CAN_FAULT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
