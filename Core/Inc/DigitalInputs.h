/*
 * DigitalInput.h
 *
 *  Created on: 17 paź 2025
 *      Author: mateo
 */

#ifndef INC_DIGITALINPUTS_H_
#define INC_DIGITALINPUTS_H_

#include "main.h"

typedef struct
{
	GPIO_TypeDef *port;
	uint16_t pin;
}DigitalInput;

extern DigitalInput digitalInput[16];

#endif /* INC_DIGITALINPUTS_H_ */
