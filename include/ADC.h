/******************************************************************************
 * This file is a part of the Sysytem Microprocessor Project                  *
 ******************************************************************************/

/**
 * @file   ADC.h
 * @author Sokolowski & Zimnol & Maj
 * @date   Dec 2021
 * @brief  File containing enums, structures and declarations for ADC0.
 * @ver    0.2
 */

#ifndef ADC_H
#define ADC_H

#include "MKL25Z4.h"

/****************************************************************************** 
 * Global definitions
 ******************************************************************************/

/* Clock divider */
#define ADIV_1            0x00
#define ADIV_2            0x01
#define ADIV_4            0x02
#define ADIV_8            0x03

/* Resolution (single ended) */
#define MODE_8            0x00
#define MODE_12           0x01
#define MODE_10           0x02
#define MODE_16           0x03

/* Clock */
#define ADICLK_BUS        0x00
#define ADICLK_BUS_2      0x01
#define ADICLK_ALTCLK     0x02
#define ADICLK_ADACK      0x03

/* Sampling time */
#define ADLSTS_20         0x00
#define ADLSTS_12         0x01
#define ADLSTS_6          0x02
#define ADLSTS_2          0x03

/* Voltage reference */
#define REFSEL_EXT        0x00
#define REFSEL_ALT        0x01
#define REFSEL_RES        0x02 
#define REFSEL_RES_EXT    0x03 

/* Averaging */
#define AVGS_4            0x00
#define AVGS_8            0x01
#define AVGS_16           0x02
#define AVGS_32           0x03

/****************************************************************************** 
 * Function declarations
 ******************************************************************************/

/**
 * @brief   ADC initialization. PTB0, channel 8.
 * @return  Error status (1 or 0).
 */
uint8_t ADC_Init(void);

#endif /* ADC_H */