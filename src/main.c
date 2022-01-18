/******************************************************************************
 * This file is a part of the Sysytem Microprocessor Project                  *
 ******************************************************************************/

/**
 * @file   main.c
 * @author Maj & Zimnol
 * @date   Dec 2021
 * @brief  File containing main function of Spectrum Analyzer Project
 * @ver    0.1
 */

#include "MKL25Z4.h"                         /* Devider header file */
#include "arm_math.h"                        /* Basic arm math header */
#include "arm_const_structs.h"               /* Structs for FFT */
#include "arm_math_types_f16.h"              /* Math types, for float16_t */
#include "dsp/transform_functions_f16.h"     /* FFT functions for float16_t */
#include "dsp/complex_math_functions_f16.h"  /* FFT functions for float16_t */

#include "lcd1602.h"    /* 2x16 LCD display header file */
#include "pit.h"        /* PIT header file*/
#include "ADC.h"        /* ADC header file*/
#include "buttons.h"    /* button matrix header file*/
#include "fft.h"        /* complementary FFT header file*/

#define GREAT_PROJECT   (1)                     

int main() {
    uint8_t cal_error;
    
    /* Initialize LCD */
    LCD1602_Init();
    LCD1602_LVL_CH();
    LCD1602_SetCursor(0,0);
    LCD1602_Print("Initialization.");
    LCD1602_SetCursor(0,1);
    LCD1602_Print("Please wait...");
    FFT_DELAY(1000); // looks cool
    
    /* Initialize FFT status */
    arm_status FFT_InitStatus = ARM_MATH_SUCCESS;
    
    /* Initialize instance for FFT */
    arm_rfft_fast_instance_f16 fft;
    
    /* Initialize rest of FFT */
    FFT_InitStatus = arm_rfft_fast_init_f16(&fft, FFT_SIZE);
    FFTstatus.readToBuffer0 = 1;
    FFTstatus.readToBuffer1 = 0; 
    FFTstatus.isBuffer0Ready = 0;
    FFTstatus.isBuffer1Ready = 0; 
    FFTstatus.mode = 1;
    
    if( FFT_InitStatus != ARM_MATH_SUCCESS ) {
        LCD1602_ClearAll();
        LCD1602_SetCursor(0,0);
        LCD1602_Print("FFT Init failed.");
        while(1);
    }
    
    /* Initialize PIT0 */
    /* TSV Value = (Bus Clock Frequency)/(Wanted Frequency)+1 */
    PIT_Initialize(601U);
    
    /* Initialize ADC0, perform calibration */
    if( (cal_error=ADC_Init()) == 1 ) {
        LCD1602_ClearAll();
        LCD1602_SetCursor(0,0);
        LCD1602_Print("ADC Init failed.");
        while(1); // calibration failed
    }

    /* Initialize buttons */
    buttons_Initialize();
    
    /* Trigger ADC0 on channel 8 */
    ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(8);    
        
    LCD1602_ClearAll();
    
    /* infinite loop */
    while( GREAT_PROJECT ) {
        __WFI();
        if( FFTstatus.isBuffer0Ready ) {
            /* apply window function od samples */
            for( uint16_t i=0; i<FFT_SIZE; i++ )
                FFT_Buffer[0][i] *= Hann_Window[i];
            
            /* calculate FFT */
            arm_rfft_fast_f16(&fft, FFT_Buffer[0], FFT_Output, 0);
            /* calculate magnitude of complex output */
            arm_cmplx_mag_f16(FFT_Output, FFT_Buffer[0], FFT_SIZE);
            
            /* colect proper samples to the LCD */
            FFT_CalculateColumns_256(BUFFER_0);
            FFTstatus.isBuffer0Ready = 0;
            
            /* print frequency bins on lcd display */
            NVIC_DisableIRQ(PORTA_IRQn);
            FFT_PrintColumns();
            NVIC_EnableIRQ(PORTA_IRQn);
        }
        if( FFTstatus.isBuffer1Ready ) {
            /* apply window function od samples */
            for( uint16_t i=0; i<FFT_SIZE; i++ )
                FFT_Buffer[1][i] *= Hann_Window[i];
            
            /* calculate FFT */
            arm_rfft_fast_f16(&fft, FFT_Buffer[1], FFT_Output, 0);
            /* calculate magnitude of complex output */
            arm_cmplx_mag_f16(FFT_Output, FFT_Buffer[1], FFT_SIZE);
            
            /* colect proper samples to the LCD */
            FFT_CalculateColumns_256(BUFFER_1);
            FFTstatus.isBuffer1Ready = 0;
            
            /* print frequency bins on lcd display */
            NVIC_DisableIRQ(PORTA_IRQn);
            FFT_PrintColumns();
            NVIC_EnableIRQ(PORTA_IRQn);
        }
    }
}
