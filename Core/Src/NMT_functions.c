/*
 * NMT_functions.c
 *
 *  Created on: Oct 11, 2025
 *      Author: mateo
 */


#include "NMT_functions.h"
#include "OD.h"
#include "flash.h"

//#define FLASH_PAGE 255 //For 256 flash pages
#define FLASH_PAGE 127 //For 128 flash pages

volatile CANopenNodeSTM32 canOpenNodeSTM32;

void nmtStateChangedCallback(const CO_NMT_internalState_t state)
{
	CO_LOCK_OD(canOpenNodeSTM32.canOpenStack->CANmodule);

	OD_entry_t *entry;

	if(state != CO_NMT_OPERATIONAL)
	{
		Flash_ReadStruct(FLASH_PAGE, &flash_virtualInputOutput);

		uint8_t isNotEmpty = !checkStructEmpty(&flash_virtualInputOutput);

		if(isNotEmpty)
		{
			entry = OD_find(OD, 0x6100);

			for(uint8_t subIndex = 1; subIndex <= OD_CNT_ARR_6100; ++subIndex)
			{
				OD_IO_t io;
				{
					OD_getSub(entry, subIndex, &io, false);
				}

				uint8_t identifier[6];

				OD_stream_t ioStreamCopy = io.stream;
				{
					OD_size_t bytesRead;
					io.read(&io.stream, identifier, sizeof(identifier), &bytesRead);
				}

				memcpy(identifier, flash_virtualInputOutput.virtualInputs[subIndex-1], 5);	// load saved input functions

				OD_size_t bytesWritten;
				io.write(&ioStreamCopy, identifier, sizeof(identifier), &bytesWritten);
			}
		}


		OD_entry_t *entry = OD_find(OD, 0x6200);

			for(uint8_t subIndex = 1; subIndex <= OD_CNT_ARR_6200; ++subIndex)
			{
				OD_IO_t io;
				{
					OD_getSub(entry, subIndex, &io, false);
				}

				uint8_t identifier[6];

				OD_stream_t ioStreamCopy = io.stream;
				{
					OD_size_t bytesRead;
					io.read(&io.stream, identifier, sizeof(identifier), &bytesRead);
				}

				identifier[5] = 0;	// reset output

				if(isNotEmpty)
				{
					memcpy(identifier, flash_virtualInputOutput.virtualOutputs[subIndex-1], 5); //load saved output function
				}

				OD_size_t bytesWritten;
				io.write(&ioStreamCopy, identifier, sizeof(identifier), &bytesWritten);
			}
			CO_UNLOCK_OD(canOpenNodeSTM32.canOpenStack->CANmodule);
			return;

	}

//if it is operational clear pending input messages
	if(state == CO_NMT_OPERATIONAL)
	{
		for(uint8_t i = 0; i < VIRTUAL_INPUT_MAPPING_SIZE; ++i)
		{
			memset(virtualInputMapping[i].InputFunctionID, 0, sizeof(virtualInputMapping[i].InputFunctionID));
			virtualInputMapping[i].pending = 0;
		}

		pendingVirtualInputMappings = 0;


	}

	CO_UNLOCK_OD(canOpenNodeSTM32.canOpenStack->CANmodule);
}
