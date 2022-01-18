/******************************************************************************
 * This file is a part of the Sysytem Microprocessor Project                  *
 ******************************************************************************/

/**
 * @file   ADC.c
 * @author Sokolowski & Maj & Zimnol
 * @date   Dec 2021
 * @brief  File containing definitions for ADC0.
 * @ver    0.2
 */

#include "ADC.h"
#include "fft.h"

/******************************************************************************
 * Private memory declarations
 ******************************************************************************/

static float16_t ADC_Read = 0;
static uint16_t SampleCounter = 0;

/******************************************************************************
 * Function definitions
 ******************************************************************************/

/**-----------------------------------------------------------------------------
 * @brief   ADC initialization. PTB0, channel 8.
 * @return  Error status (1 or 0).
 */
uint8_t ADC_Init(void) {
    uint16_t temp_calib;
    
    /* Connect clock to ADC0 and PORTB */
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;          
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    /* PTB0, chanel 8 */   
    PORTB->PCR[0] &= ~(PORT_PCR_MUX(0));                
    
    /* ADCK 24 MHz */
    ADC0->CFG1 = ADC_CFG1_ADICLK(ADICLK_BUS) | ADC_CFG1_ADIV(ADIV_1) 
               | ADC_CFG1_ADLSMP_MASK;
    /* High frequencies support */   
    ADC0->CFG2 = ADC_CFG2_ADHSC_MASK;
    /* Averaging = 4 */    
    ADC0->SC3  = ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(AVGS_4);        
    
    /* Start calibration */
    ADC0->SC3 |= ADC_SC3_CAL_MASK;
    /* Wait until calibration end */    
    while(ADC0->SC3 & ADC_SC3_CAL_MASK);    
    
    /* If calibration error - return 1 */
    if(ADC0->SC3 & ADC_SC3_CALF_MASK) {
      ADC0->SC3 |= ADC_SC3_CALF_MASK;
      return(1);                                
    }
    
    temp_calib = 0x00;
    temp_calib += ADC0->CLP0;
    temp_calib += ADC0->CLP1;
    temp_calib += ADC0->CLP2;
    temp_calib += ADC0->CLP3;
    temp_calib += ADC0->CLP4;
    temp_calib += ADC0->CLPS;
    temp_calib += ADC0->CLPD;
    temp_calib /= 2;
    /* Set MSB as "1" */
    temp_calib |= 0x8000;                       
    /* Save temp_calib in "plus-side gain calibration register" */
    ADC0->PG = ADC_PG_PG(temp_calib);           

    ADC0->SC1[0] = ADC_SC1_ADCH(31);
    ADC0->CFG2 |= ADC_CFG2_ADHSC_MASK;
    
    /* ADCK 24 MHz, 12 bit resolution, long sampling time */
    ADC0->CFG1 = ADC_CFG1_ADICLK(ADICLK_BUS) | ADC_CFG1_ADIV(ADIV_1) | ADC_CFG1_ADLSMP_MASK | ADC_CFG1_MODE(MODE_12);
    /* Hardware trigger select */    
    ADC0->SC2 |= ADC_SC2_ADTRG_MASK;
    /* Trigger ADC0 through PIT0 */   
    SIM->SOPT7 |= SIM_SOPT7_ADC0ALTTRGEN_MASK | SIM_SOPT7_ADC0TRGSEL(4);
    
    /* Enable interrupts */
    NVIC_ClearPendingIRQ(ADC0_IRQn);
    NVIC_EnableIRQ(ADC0_IRQn);
    
    return(0);
}

/**-----------------------------------------------------------------------------
 * @brief Interrupt hanlder for ADC0. Collects samples from converter and sets 
 *        necessary flags.
 */
void ADC0_IRQHandler() {    
    ADC_Read = ADC0->R[0];    // read ADC0, clear COCO flag
    if( FFTstatus.readToBuffer0 ) {
        if( SampleCounter != FFT_SIZE )
            FFT_Buffer[0][SampleCounter++] = ADC_Read-FFT_AVG_VALUE;
        if( SampleCounter == FFT_SIZE && !FFTstatus.isBuffer1Ready ) { 
            SampleCounter = 0;
            FFTstatus.isBuffer0Ready = 1;
            FFTstatus.readToBuffer0 = 0;
            FFTstatus.readToBuffer1 = 1; 
        }        
    }
    else {
        if( SampleCounter != FFT_SIZE )
            FFT_Buffer[1][SampleCounter++] = ADC_Read-FFT_AVG_VALUE;
        if( SampleCounter == FFT_SIZE && !FFTstatus.isBuffer0Ready ) {
            SampleCounter = 0;
            FFTstatus.isBuffer1Ready = 1;
            FFTstatus.readToBuffer0 = 1;
            FFTstatus.readToBuffer1 = 0;
        }
    }
    NVIC_EnableIRQ(ADC0_IRQn);
} 
