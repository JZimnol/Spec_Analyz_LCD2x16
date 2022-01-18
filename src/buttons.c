/******************************************************************************
 * This file is a part of the Sysytem Microprocessor Project                  *
 ******************************************************************************/

/**
 * @file   buttons.c
 * @author Maj & Zimnol
 * @date   Dec 2021
 * @brief  File containing definitions for simple 4x4 keyboard matrix.
 * @ver    0.1
 */

#include "buttons.h"
#include "lcd1602.h"
#include "fft.h"

/****************************************************************************** 
 * Function definitions
 ******************************************************************************/

/**-----------------------------------------------------------------------------
 * @brief initialize button pins, enable interrupts
 */
void buttons_Initialize(void){
    /* Enable clock for PORT_A */
    SIM->SCGC5 |=  SIM_SCGC5_PORTA_MASK;                 
    PORTA->PCR[BUT_R2A12] |= PORT_PCR_MUX(1);          
    PORTA->PCR[BUT_R1A5]  |= PORT_PCR_MUX(1);          
    
    /* Activate pull up for PORT_A */
    PORTA->PCR[BUT_R2A12] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;      
    PORTA->PCR[BUT_R1A5]  |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;             
    
    /* falling edge interrupts for PORT_A */
    PORTA->PCR[BUT_R2A12] |= PORT_PCR_IRQC(0xa);
    PORTA->PCR[BUT_R1A5]  |= PORT_PCR_IRQC(0xa);    
    
    /* Clear NVIC any pending interrupts on PORT_A */
    NVIC_ClearPendingIRQ(PORTA_IRQn);
    
    /* Enable NVIC interrupts source for PORT_A module */    
    NVIC_EnableIRQ(PORTA_IRQn);
    /* High priority */
    NVIC_SetPriority(PORTA_IRQn, 0);
    
    /* Enable clock for PORT_C */
    SIM->SCGC5 |=  SIM_SCGC5_PORTC_MASK;
    PORTC->PCR[BUT_C4C0] |= PORT_PCR_MUX(1);          
    PORTC->PCR[BUT_C3C3] |= PORT_PCR_MUX(1);          
    PORTC->PCR[BUT_C2C4] |= PORT_PCR_MUX(1);
    PORTC->PCR[BUT_C1C5] |= PORT_PCR_MUX(1);
                          
    /* Set columns as low output for PORT_A */
    PTC->PDDR |= (1<<BUT_C4C0) | (1<<BUT_C3C3) | (1<<BUT_C2C4) | (1<<BUT_C1C5);
    PTC->PDOR &= ~( (1<<BUT_C4C0) | (1<<BUT_C3C3) | (1<<BUT_C2C4) | (1<<BUT_C1C5) );
} 

/**-----------------------------------------------------------------------------
 * @brief PORT_A interrupt hanlder, reads pressed keys from keyboard
 */
void PORTA_IRQHandler(void){  
    /*-----------------
      | CHECK 2nd ROW |
      -----------------*/
    if( PORTA->ISFR & (1<<BUT_R2A12) ){
        /* set columns as inputs */
        PTC->PDDR &= ~( (1<<BUT_C4C0) | (1<<BUT_C3C3) | (1<<BUT_C2C4) | (1<<BUT_C1C5) );
        PORTC->PCR[BUT_C4C0] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;      
        PORTC->PCR[BUT_C3C3] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;             
        PORTC->PCR[BUT_C2C4] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
        PORTC->PCR[BUT_C1C5] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
        
        /* set R2A12 as low output */
        PTA->PDDR |= (1<<BUT_R2A12);
        PTA->PDOR &= ~(1<<BUT_R2A12);
        
        if( (PTC->PDIR & (1<<BUT_C4C0)) == 0 ) {
            LCD1602_ClearAll();
            LCD1602_SetCursor(0,0);
            LCD1602_Print("Mode: 8");
            LCD1602_SetCursor(0,1);
            LCD1602_Print("15000 - 17500 Hz");
            FFT_DELAY(1000);
            LCD1602_ClearAll();
            FFTstatus.mode = 8;
        } 
        else if( (PTC->PDIR & (1<<BUT_C3C3)) == 0 ) {
            LCD1602_ClearAll();
            LCD1602_SetCursor(0,0);
            LCD1602_Print("Mode: 7");
            LCD1602_SetCursor(0,1);
            LCD1602_Print("12500 - 15000 Hz");
            FFT_DELAY(1000);
            LCD1602_ClearAll();
            FFTstatus.mode = 7;
        } 
        else if( (PTC->PDIR & (1<<BUT_C2C4)) == 0 ) {
            LCD1602_ClearAll();
            LCD1602_SetCursor(0,0);
            LCD1602_Print("Mode: 6");
            LCD1602_SetCursor(0,1);
            LCD1602_Print("10000 - 12500 Hz");
            FFT_DELAY(1000);
            LCD1602_ClearAll();
            FFTstatus.mode = 6;
        } 
        else if( (PTC->PDIR & (1<<BUT_C1C5)) == 0 ) {
            LCD1602_ClearAll();
            LCD1602_SetCursor(0,0);
            LCD1602_Print("Mode: 5");
            LCD1602_SetCursor(0,1);
            LCD1602_Print("7500 - 10000 Hz");
            FFT_DELAY(1000);
            LCD1602_ClearAll();
            FFTstatus.mode = 5;
        } 
        
        /* set R2A12 as input */
        PTA->PDDR &= ~(1<<BUT_R2A12);
        PORTA->PCR[BUT_R2A12]  |=  PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;    
                              
        /* set columns as low output */
        PTC->PDDR |= (1<<BUT_C4C0) | (1<<BUT_C3C3) | (1<<BUT_C2C4) | (1<<BUT_C1C5);
        PTC->PDOR &= ~( (1<<BUT_C4C0) | (1<<BUT_C3C3) | (1<<BUT_C2C4) | (1<<BUT_C1C5) );
        
        /* clear interrupt service flag (ISF) in Port Control Register */   
        PORTA->PCR[BUT_R2A12] |= PORT_PCR_ISF_MASK;
    } 
    /*-----------------
      | CHECK 1st ROW |
      -----------------*/
    if( PORTA->ISFR & (1<<BUT_R1A5) ){
        /* set columns as inputs */
        PTC->PDDR &= ~( (1<<BUT_C4C0) | (1<<BUT_C3C3) | (1<<BUT_C2C4) | (1<<BUT_C1C5) );
        PORTC->PCR[BUT_C4C0] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;      
        PORTC->PCR[BUT_C3C3] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;             
        PORTC->PCR[BUT_C2C4] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
        PORTC->PCR[BUT_C1C5] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
        
        /* set R1A5 as low output */
        PTA->PDDR |= (1<<BUT_R1A5);
        PTA->PDOR &= ~(1<<BUT_R1A5);
        
        if( (PTC->PDIR & (1<<BUT_C4C0)) == 0 ) {
            LCD1602_ClearAll();
            LCD1602_SetCursor(0,0);
            LCD1602_Print("Mode: 4");
            LCD1602_SetCursor(0,1);
            LCD1602_Print("5000 - 7500 Hz");
            FFT_DELAY(1000);
            LCD1602_ClearAll();
            FFTstatus.mode = 4;
        } 
        else if( (PTC->PDIR & (1<<BUT_C3C3)) == 0 ) {
            LCD1602_ClearAll();
            LCD1602_SetCursor(0,0);
            LCD1602_Print("Mode: 3");
            LCD1602_SetCursor(0,1);
            LCD1602_Print("2500 - 5000 Hz");
            FFT_DELAY(1000);
            LCD1602_ClearAll();
            FFTstatus.mode = 3;
        } 
        else if( (PTC->PDIR & (1<<BUT_C2C4)) == 0 ) {
            LCD1602_ClearAll();
            LCD1602_SetCursor(0,0);
            LCD1602_Print("Mode: 2");
            LCD1602_SetCursor(0,1);
            LCD1602_Print("0 - 2500 Hz");
            FFT_DELAY(1000);
            LCD1602_ClearAll();
            FFTstatus.mode = 2;
        } 
        else if( (PTC->PDIR & (1<<BUT_C1C5)) == 0 ) {
            LCD1602_ClearAll();
            LCD1602_SetCursor(0,0);
            LCD1602_Print("Mode: 1");
            LCD1602_SetCursor(0,1);
            LCD1602_Print("0 - 20000 Hz");
            FFT_DELAY(1000);
            LCD1602_ClearAll();
            FFTstatus.mode = 1;
        } 
        
        /* set R1A5 as input */
        PTA->PDDR &= ~(1<<BUT_R1A5);
        PORTA->PCR[BUT_R1A5]  |=  PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;    
                              
        /* set columns as low output */
        PTC->PDDR |= (1<<BUT_C4C0) | (1<<BUT_C3C3) | (1<<BUT_C2C4) | (1<<BUT_C1C5);
        PTC->PDOR &= ~( (1<<BUT_C4C0) | (1<<BUT_C3C3) | (1<<BUT_C2C4) | (1<<BUT_C1C5) );
        
        /* clear interrupt service flag (ISF) in Port Control Register */   
        PORTA->PCR[BUT_R2A12] |= PORT_PCR_ISF_MASK;
    } 
    
    /* reset all frequency bins */
    for( uint8_t i=0; i<16; i++ ) {
        FrequencyBins[i] = 0;
        oldBins[i] = 0;
    }
} 
