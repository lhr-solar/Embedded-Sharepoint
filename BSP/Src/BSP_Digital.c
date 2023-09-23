/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"
#include "BSP_DigitalIn.h"

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA4 PA5
                           PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PC10 PC11 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

//TODO: Combine these definitions together and explicitly define the struct with idx value. Example 
//can be found in FaultState.c

// Just some lookup for the values
static const uint32_t LIGHT_VALS[LIGHTS_MAX] = {
	GPIO_Pin_4,	// RUN, 
	GPIO_Pin_7, // UVOLT, 
	GPIO_Pin_6,	// OVOLT, 
	GPIO_Pin_5,	// OTEMP, 
	GPIO_Pin_4,	// OCURR, 
	GPIO_Pin_2,	// WDOG, 
	GPIO_Pin_1,	// CAN, 
	GPIO_Pin_0,	// EXTRA, 
	GPIO_Pin_3,	// WIRE,
  GPIO_Pin_12 // STROBE
};

static GPIO_TypeDef * const LIGHT_PORTS[LIGHTS_MAX] = {
  GPIOC,  // RUN, 
  GPIOA,  // UVOLT, 
  GPIOA,  // OVOLT, 
  GPIOA,  // OTEMP, 
  GPIOA,  // OCURR, 
  GPIOC,  // WDOG, 
  GPIOC,  // CAN, 
  GPIOC,  // EXTRA, 
  GPIOC,  // WIRE,
  GPIOB,  // STROBE
};

static GPIO_TypeDef* getPort(port_t port){
 	const GPIO_TypeDef* gpio_mapping[5] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOH};

	return (GPIO_TypeDef *) gpio_mapping[port];

}

/**
 * @brief   Initializes a GPIO port
 * @param   port - port to initialize
 * @param	mask - pins
 * @param	direction - input or output 
 * @return  None
 */ 
void BSP_GPIO_Init(port_t port, uint16_t mask, direction_t direction){
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    switch getPort(port):
      case PORTA:
        __HAL_RCC_GPIOA_CLK_ENABLE();

        GPIO_InitStruct.Pin = mask;
        GPIO_InitStruct.Mode = direction ? GPIO_MODE_OUTPUT_PP : GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        break;
      case PORTB:
        __HAL_RCC_GPIOB_CLK_ENABLE();

        GPIO_InitStruct.Pin = mask;
        GPIO_InitStruct.Mode = direction ? GPIO_MODE_OUTPUT_PP : GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        break;
      case PORTC:
        __HAL_RCC_GPIOC_CLK_ENABLE();

        GPIO_InitStruct.Pin = mask;
        GPIO_InitStruct.Mode = direction ? GPIO_MODE_OUTPUT_PP : GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
        break;
      case PORTD:
        __HAL_RCC_GPIOD_CLK_ENABLE();

        GPIO_InitStruct.Pin = mask;
        GPIO_InitStruct.Mode = direction ? GPIO_MODE_OUTPUT_PP : GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
        break;
      case PORTH:
        __HAL_RCC_GPIOH_CLK_ENABLE();

        GPIO_InitStruct.Pin = mask;
        GPIO_InitStruct.Mode = direction ? GPIO_MODE_OUTPUT_PP : GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

        HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
        break;
}

/**
 * @brief   Reads value of the specified port
 * @param   port to read
 * @return  data of the port
 */ 
uint16_t BSP_GPIO_Read(port_t port){
    GPIO_TypeDef *gpio_port = getPort(port);
    return gpio_port->IDR;
}

/**
 * @brief   Writes data to a specified port 
 * @param   port port to write to
 * @param   data data to write 
 * @return  None
 */ 

void BSP_GPIO_Write(port_t port, uint16_t data){
	GPIO_TypeDef *gpio_port = GPIO_GetPort(port);
  gpio_port->ODR = data;
}

/**
 * @brief   Reads data from a specified input pin (not applicable to output pins)
 * @param   port The port to read from
 * @param   pin The pin to read from 
 * @return  State of the pin
 */ 

uint8_t BSP_GPIO_Read_Pin(port_t port, uint16_t pinmask){
	GPIO_TypeDef *gpio_port = GPIO_GetPort(port);

  return HAL_GPIO_ReadPin(gpio_port, pinmask);
}

/**
 * @brief   Writes data to a specified pin
 * @param   port The port to write to
 * @param   pin The pin to write to 
 * @param   state true=ON or false=OFF
 * @return  None
 */ 

void BSP_GPIO_Write_Pin(port_t port, uint16_t pinmask, bool state){
	GPIO_TypeDef *gpio_port = GPIO_GetPort(port);

  HAL_GPIO_WritePin(gpio_port, pinmask, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/**
 * @brief   Returns state of output pin (not applicable to input pins)
 * @param   port The port to get state from
 * @param   pin The pin to get state from
 * @return  1 if pin is high, 0 if low
 */ 
uint8_t BSP_GPIO_Get_State(port_t port, uint16_t pin){
	GPIO_TypeDef *gpio_port = GPIO_GetPort(port);

	return gpio_port->IDR & pin ? 1 : 0;
}

/**
 * @brief   Initialize all the GPIO pins connected to each LED/Light
 * @param   None
 * @return  None
 */
void BSP_Lights_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7;

  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
  GPIO_Init(GPIOB, &GPIO_InitStruct);


  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_Pin_12;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
 * @brief   Toggles a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_Toggle(Light signal) {
    LIGHT_PORTS[signal]->ODR ^= LIGHT_VALS[signal];
}

/**
 * @brief   Turns on a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_On(Light signal) {
    LIGHT_PORTS[signal]->ODR |= LIGHT_VALS[signal];
}

/**
 * @brief   Turns off a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_Off(Light signal) {
    LIGHT_PORTS[signal]->ODR &= ~LIGHT_VALS[signal];
}

/**
 * @brief   Gets the state of the LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  State of the LED/Light (ON/OFF)
 */
State BSP_Light_GetState(Light signal) {
    return (LIGHT_PORTS[signal]->ODR & LIGHT_VALS[signal]) ? ON : OFF;
}

/* USER CODE END 2 */
