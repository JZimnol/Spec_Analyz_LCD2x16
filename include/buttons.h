/******************************************************************************
 * This file is a part of the Sysytem Microprocessor Project                  *                                                 
 ******************************************************************************/

/**
 * @file   buttons.h
 * @author Maj & Zimnol
 * @date   Dec 2021
 * @brief  File containing enums, structures and declarations for 4x4 keyboard.
 * @ver    0.1
 */

#ifndef BUTTONS_H
#define BUTTONS_H

#include "MKL25Z4.h"

/****************************************************************************** 
 * Global enums
 ******************************************************************************/

/**
 * @brief simple enum for buttons from keyboard
 *        BUT_<row/column><number><pin> = <pin number> 
 * @note  R1 on keyboard is R4 here etc.
 */
typedef enum { 
    BUT_R2A12 = 12, 
    BUT_R1A5  = 5, 
    BUT_C4C0  = 0,
    BUT_C3C3  = 3,
    BUT_C2C4  = 4,
    BUT_C1C5  = 5
} ButtonType;

/****************************************************************************** 
 * Function declarations
 ******************************************************************************/

/**
 * @brief initialize button pins, enable interrupts
 */
void buttons_Initialize(void);

#endif /* BUTTONS_H */
