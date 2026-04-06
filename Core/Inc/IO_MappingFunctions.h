/*
 * IO_MappingFunctions.h
 *
 *  Created on: Oct 11, 2025
 *      Author: mateo
 */

#ifndef INC_IO_MAPPINGFUNCTIONS_H_
#define INC_IO_MAPPINGFUNCTIONS_H_

#pragma once

//#include "DigitalOutput.h"
#include "CO_app_STM32.h"

ODR_t virtualInputMappingRead(OD_stream_t* const stream, void* const buffer, const OD_size_t size, OD_size_t* const bytesRead);
ODR_t virtualOutputMappingWrite(OD_stream_t* const stream, const void* const buffer, const OD_size_t size, OD_size_t* const bytesWritten);
ODR_t outputGroupWrite(OD_stream_t* const stream, const void* const buffer, const OD_size_t size, OD_size_t* const bytesWritten);
ODR_t saveParametersWrite(OD_stream_t* const stream, const void* const buffer, const OD_size_t size, OD_size_t* const bytesWritten);

#endif /* INC_IO_MAPPINGFUNCTIONS_H_ */
