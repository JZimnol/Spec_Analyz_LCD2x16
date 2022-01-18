/******************************************************************************
 * This file is a part of the Sysytem Microprocessor Project                  *
 ******************************************************************************/

/**
 * @file   lcd1602.h
 * @author Koryciak & Maj & Zimnol
 * @date   Sep 2020, Dec 2021
 * @brief  File containing enums, structures and declarations for LCD 2x16.
 * @ver    0.2
 */

#ifndef LCD1602_H
#define LCD1602_H

#include "i2c.h"

/****************************************************************************** 
 * Function declarations
 ******************************************************************************/

/**
 * @brief LCD1602 initialization.
 */
void LCD1602_Init(void);

/**
 * @brief     Print on the display.
 * @param[in] String to display.
 */
void LCD1602_Print(char *str);

/**
 * @brief Clear whole display.
 */
void LCD1602_ClearAll(void);

/**
 * @brief     Set cursor on display.
 * @param[in] Column.
 * @param[in] Row.
 */
void LCD1602_SetCursor(uint8_t col, uint8_t row);

/**
 * @brief Load custom characters to LCD
 */
void LCD1602_LVL_CH(void);

/**
 * @brief     Write normalized 8 level amplitude bar to LCD
 * @param[in] Normalized amplitude level: 1-8
 */
void LCD1602_PrintLVL(char *str);

/**
 * @brief     Write normalized 16-levels amplitude bar to LCD
 * @param[in] Normalized amplitude level: 1-16
 * @param[in] Column number
 */
void LCD1602_PrintLVL_16(uint8_t, uint8_t);

/**
 * @brief     Write normalized 16-levels amplitude bar to LCD. Function dedicated
 *            for a spectrum analyzer, does not do any unnecessary prints and 
 *            works faster
 * @param[in] Normalized amplitude level: 0-16
 * @param[in] Previous normalized amplitude level: 0-16
 * @param[in] Column number
 */
void LCD1602_PrintLVL_16_FFT(uint8_t lvl, uint8_t oldLvl, uint8_t col);

/**
 * @brief     Read busy flag and address counter
 * @param[in] pointer to uint8_t which stores address counter value
 */
uint8_t LCD1602_BF_AC(uint8_t *ptr);

#endif  /* LCD1602_H */
