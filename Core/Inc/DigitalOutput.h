/*
 * DigitalOutput.h
 *
 *  Created on: 15 paź 2025
 *      Author: mateo
 */

#ifndef INC_DIGITALOUTPUT_H_
#define INC_DIGITALOUTPUT_H_

#pragma once

#include "softwareTimer_ms.h"

extern const uint16_t flashingDurations[];

extern const uint16_t impulseDurations[];


typedef struct
{
	GPIO_TypeDef *port;
	uint16_t pin;
	SoftwareTimerHandler *timer;
}DigitalOutput;

void digitalOutputsInitialize(void);
void processFunctionData(DigitalOutput *digitalOutput, const uint8_t functionData);
void toggleOutputPin(void * param);

extern DigitalOutput digitaloutputs[16];


#endif /* INC_DIGITALOUTPUT_H_ */
