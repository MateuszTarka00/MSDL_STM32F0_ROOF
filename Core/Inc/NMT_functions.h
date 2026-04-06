/*
 * NMT_functions.h
 *
 *  Created on: Oct 11, 2025
 *      Author: mateo
 */

#ifndef INC_NMT_FUNCTIONS_H_
#define INC_NMT_FUNCTIONS_H_

#include "canOpen_data.h"

#endif /* INC_NMT_FUNCTIONS_H_ */

extern volatile CANopenNodeSTM32 canOpenNodeSTM32;

void nmtStateChangedCallback(const CO_NMT_internalState_t state);
