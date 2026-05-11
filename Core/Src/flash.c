/*
 * flash.c
 *
 *  Created on: 23 paź 2025
 *      Author: mateo
 *  Adapted for STM32F091CCT6 (F0 series)
 */

#include "flash.h"
#include <string.h>

/* ===================== FLASH CONFIG ===================== */

#define FLASH_BASE_ADDR     0x08000000U             // 1 KB pages (STM32F091)
#define FLASH_TOTAL_SIZE    (127U * 2048U)         // 256 KB total
#define FLASH_PAGE_COUNT    (FLASH_TOTAL_SIZE / FLASH_PAGE_SIZE)

#define FLASH_LAST_PAGE     (FLASH_PAGE_COUNT - 1)
#define FLASH_STORAGE_PAGE  FLASH_LAST_PAGE

/* ===================== GLOBAL ===================== */

Flash_virtualInputOutput flash_virtualInputOutput = {0};

/* ===================== INTERNAL ===================== */

// Calculate page start address
static uint32_t Flash_GetPageAddress(uint32_t pageIndex)
{
    return FLASH_BASE_ADDR + (pageIndex * FLASH_PAGE_SIZE);
}

/* ===================== ERASE ===================== */

static void Flash_ErasePage(uint32_t pageIndex)
{
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef eraseInit = {0};
    uint32_t pageError = 0;

    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInit.PageAddress = Flash_GetPageAddress(FLASH_STORAGE_PAGE);   // STM32F0 uses page index
    eraseInit.NbPages = 1;

    if (HAL_FLASHEx_Erase(&eraseInit, &pageError) != HAL_OK)
    {
        HAL_FLASH_Lock();
        return;
    }

    HAL_FLASH_Lock();
}

/* ===================== WRITE ===================== */

void Flash_WriteStruct(const Flash_virtualInputOutput *data)
{
    uint32_t address = Flash_GetPageAddress(FLASH_STORAGE_PAGE);

    HAL_FLASH_Unlock();

    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP |
                           FLASH_FLAG_WRPERR |
                           FLASH_FLAG_PGERR);

    // Check if erase is needed
    Flash_virtualInputOutput temp;
    memcpy(&temp, (const void *)address, sizeof(temp));

    if (!checkStructEmpty(&temp))
    {
        HAL_FLASH_Lock();
        Flash_ErasePage(FLASH_STORAGE_PAGE);
        HAL_FLASH_Unlock();
    }

    const uint8_t *src = (const uint8_t *)data;

    for (uint32_t i = 0; i < sizeof(Flash_virtualInputOutput); i += 4)
    {
        uint32_t word = 0xFFFFFFFF;

        uint32_t remaining = sizeof(Flash_virtualInputOutput) - i;
        memcpy(&word, src + i, (remaining >= 4) ? 4 : remaining);

        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address + i, word) != HAL_OK)
        {
            break;
        }
    }

    HAL_FLASH_Lock();
}

/* ===================== READ ===================== */

void Flash_ReadStruct(Flash_virtualInputOutput *data)
{
    uint32_t address = Flash_GetPageAddress(FLASH_STORAGE_PAGE);
    memcpy(data, (const void *)address, sizeof(Flash_virtualInputOutput));
}

/* ===================== UTIL ===================== */

uint8_t checkStructEmpty(const Flash_virtualInputOutput *data)
{
    const uint8_t *src = (const uint8_t *)data;

    for (uint16_t i = 0; i < sizeof(*data); ++i)
    {
        if (*src != 0xFF)
        {
            return 0;
        }
        src++;
    }
    return 1;
}
