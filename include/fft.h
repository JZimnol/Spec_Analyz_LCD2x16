/******************************************************************************
 * This file is a part of the Sysytem Microprocessor Project                  *                                                 
 ******************************************************************************/

/**
 * @file   fft.h
 * @author Maj & Zimnol
 * @date   Dec 2021
 * @brief  File containing structures and declarations for FFT and spectrum analyzer.
 * @ver    0.1
 */

#ifndef FFT_H
#define FFT_H

#include "arm_math_types_f16.h"
#include "math.h"

/******************************************************************************
 * Global definitions
 ******************************************************************************/

/* size of a FFT, must be 2^N, where N is a positive integer */
#define FFT_SIZE                 (256)

#define BUFFER_0                 (0)
#define BUFFER_1                 (1)

/* simple delay */
#define FFT_DELAY(x)             for(volatile uint32_t i=0;i<(x*10000);i++)

/* typical max value of a sample, used to normalize frequency bins */
#define FFT_AVG_MAX_VALUE        (3.5)
/* average value of a sampled signal, used to delete constant value of a signal */
#define FFT_AVG_VALUE            (2681)  
 
/******************************************************************************
 * Global variable declarations
 ******************************************************************************/

/* struct with necessary FFT status flags */
typedef struct {
    uint8_t readToBuffer0:1;
    uint8_t readToBuffer1:1; 
    uint8_t isBuffer0Ready:1; 
    uint8_t isBuffer1Ready:1; 
    uint8_t mode:4;
} FFT_Flags;

extern FFT_Flags FFTstatus;

/* FFT buffers */
extern float16_t FFT_Buffer[2][FFT_SIZE];
extern float16_t FFT_Output[2*FFT_SIZE];
extern uint8_t FrequencyBins[16];
extern uint8_t oldBins[16];

/* FFT window ceofficients */ 
extern const float16_t Hann_Window[FFT_SIZE];
extern const float16_t Blackman_Harris_Window[FFT_SIZE];

/******************************************************************************
 * Function declarations
 ******************************************************************************/

/**
 * @brief       Calculate column length for choosen frequencies.
 * @param[in]   Buffer number (0 or 1)
 * @descritpion Frequencies on given columns are respectively:
 * 
 * For Mode 1:
 *  [1] [2] [3] [4] [5] [6]  [7]  [8]  [9] [10] [11] [12] [13] [14]  [15]  [16] 
 * |156|312|469|625|781|938|1094|1250|1407|2500|3750|5000|7031|9062|12031|14844|
 *   
 * For Mode 2-8:
 * ((Mode-2)*16+N+1)*156, where N is integer in range 0-15 
 */
void FFT_CalculateColumns_256(uint8_t bufferNumber);

/**
 * @brief Print frequency bin columns on LCD
 */
void FFT_PrintColumns(void);

#endif /* FFT_H */