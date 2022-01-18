/******************************************************************************
 * This file is a part of the SM2 Tutorial (C).                               *
 ******************************************************************************/

/**
 * @file   PIT.c
 * @author Koryciak & Maj & Zimnol
 * @date   Dec 2021
 * @brief  File containing definitions for PIT.
 * @ver    0.3
 */

#include "pit.h"

/****************************************************************************** 
 * Function definitions
 ******************************************************************************/

/**-----------------------------------------------------------------------------
 * @brief     PIT initialization. Channel 0.
 * @param[in] value of TSV register
 */
void PIT_Initialize(unsigned tsv_value) {
    /* Enable clock to PIT module */
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; 
    
    /* Enable module, freeze timers in debug mode */
    PIT->MCR &= ~PIT_MCR_MDIS_MASK; 
    PIT->MCR |= PIT_MCR_FRZ_MASK;  
    
    /* Initialize PIT0 to count down from argument */
    /* TSV Value = (Bus Clock Frequency)/(Wanted Frequency)+1 */
    PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(tsv_value);

    /* No chaining */
    PIT->CHANNEL[0].TCTRL &= PIT_TCTRL_CHN_MASK;
    
    /* Generate interrupts */
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;

    /* Enable Interrupts */
    NVIC_SetPriority(PIT_IRQn, 1);
    NVIC_ClearPendingIRQ(PIT_IRQn); 
    NVIC_EnableIRQ(PIT_IRQn);    
    
    /* Enable counter */
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK; 
} 

/**-----------------------------------------------------------------------------
 * @brief     Change value in timer[0] load value register.
 * @param[in] value of TSV register
 */
void PIT_SetTSV(uint32_t value) {
    /* count down from */
    PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(value); 
} 

/**-----------------------------------------------------------------------------
 * @brief Interrupt handler for PIT.
 */
void PIT_IRQHandler(void) {
    /* check if request comes from PIT0 */
    if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
        /* clear status flag */
        PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK; 
    } 
    /* clear pending IRQ */
    NVIC_ClearPendingIRQ(PIT_IRQn);
} 
