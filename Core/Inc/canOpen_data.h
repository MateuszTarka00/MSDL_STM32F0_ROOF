/*
 * canOpen_data.h
 *
 *  Created on: 12 paź 2025
 *      Author: mateo
 */

#ifndef INC_CANOPEN_DATA_H_
#define INC_CANOPEN_DATA_H_

#include "CO_app_STM32.h"

#define VIRTUAL_INPUT_MAPPING_SIZE 16
#define VIRTUAL_OUTPUT_MAPPING_SIZE 16


#define DIGITAL_OUTPUTS_COUNT		16

typedef struct
{
	uint8_t InputFunctionID[6];
	uint8_t pending;
} VirtualInputMapping;

extern VirtualInputMapping virtualInputMapping[VIRTUAL_INPUT_MAPPING_SIZE];
extern uint8_t pendingVirtualInputMappings;


#endif /* INC_CANOPEN_DATA_H_ */
