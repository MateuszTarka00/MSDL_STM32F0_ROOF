/*
 * DigitalOutputs.c
 *
 *  Created on: 15 paź 2025
 *      Author: mateo
 */


#include "DigitalOutput.h"

#define DIGITAL_OUTPUT_COUNT 	16
#define IMPULS_SIGNAL			0b010
#define FLASHING_SIGNAL			0b011

static SoftwareTimerHandler outputsTimers[12];

const uint16_t flashingDurations[] =
{
		50, 	//50 ms
		67, 	//67 ms
		100,	//100 ms
		250,	//250 ms
		333,	//333 ms
		500,	//0.5 s
		1000,	//1 s
		2000,	//2 s
};

const uint16_t impulseDurations[] =
{
		500, 	//500 ms
		1000,	//1 s
		1500,	//1.5 s
		2000,	//2 s
		3000,	//3 s
		5000,	//5 s
		10000,	//10 s
		15000,	//15 s
};


DigitalOutput digitaloutputs[12] =
		{
				{OUT5_GPIO_Port, OUT5_Pin, &outputsTimers[4]},
				{OUT6_GPIO_Port, OUT6_Pin, &outputsTimers[5]},
				{OUT7_GPIO_Port, OUT7_Pin, &outputsTimers[6]},
				{OUT8_GPIO_Port, OUT8_Pin, &outputsTimers[7]},
				{OUT9_GPIO_Port, OUT9_Pin, &outputsTimers[8]},
				{OUT10_GPIO_Port, OUT10_Pin, &outputsTimers[9]},
				{OUT11_GPIO_Port, OUT11_Pin, &outputsTimers[10]},
				{OUT12_GPIO_Port, OUT12_Pin, &outputsTimers[11]},
				{OUT13_GPIO_Port, OUT13_Pin, &outputsTimers[12]},
				{OUT14_GPIO_Port, OUT14_Pin, &outputsTimers[13]},
				{OUT15_GPIO_Port, OUT15_Pin, &outputsTimers[14]},
				{OUT16_GPIO_Port, OUT16_Pin, &outputsTimers[15]},
		};

void toggleOutputPin(void * param)
{
	DigitalOutput *digitalOutput = (DigitalOutput *)param;
	HAL_GPIO_TogglePin(digitalOutput->port, digitalOutput->pin);
}


void processFunctionData(DigitalOutput *digitalOutput, const uint8_t functionData)
{
	//enter critical section

	deInitSoftwareTimer(digitalOutput->timer);
	uint8_t property = (functionData >> 1) & 0b111;
	uint8_t parameter = (functionData >> 4) & 0b111;
	uint8_t state = (functionData & 1) != 0;
	uint8_t repeat;
	uint16_t duration = 0;

	HAL_GPIO_WritePin(digitalOutput->port, digitalOutput->pin, state);

	if(property == IMPULS_SIGNAL)
	{
		repeat = 0;
		duration = impulseDurations[parameter];

		initSoftwareTimer(digitalOutput->timer, duration, toggleOutputPin, repeat, digitalOutput);
		startSoftwareTimer(digitalOutput->timer);
	}
	else if(property == FLASHING_SIGNAL)
	{
		repeat = 1;
		duration = flashingDurations[parameter];

		initSoftwareTimer(digitalOutput->timer, duration, toggleOutputPin, repeat, digitalOutput);
		startSoftwareTimer(digitalOutput->timer);
	}

	//exit critical section

}
