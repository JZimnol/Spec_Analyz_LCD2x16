/******************************************************************************
 * This file is a part of the SM2 Tutorial (C).                               *
 ******************************************************************************/

/**
 * @file   pit.h
 * @author Koryciak & Maj & Zimnol
 * @date   Dec 2021
 * @brief  File containing declarations for PIT.
 * @ver    0.6
 */

#ifndef PIT_H
#define PIT_H

#include "MKL25Z4.h" 

/****************************************************************************** 
 * Function declarations
 ******************************************************************************/

/**
 * @brief     PIT initialization. Channel 0.
 * @param[in] value of TSV register
 */
void PIT_Initialize(unsigned tsv_value);

/**
 * @brief     Change value in timer[0] load value register.
 * @param[in] value of TSV register
 */
void PIT_SetTSV(uint32_t period);

#endif /* PIT_H */
