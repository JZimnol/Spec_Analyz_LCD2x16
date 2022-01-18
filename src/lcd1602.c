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

#include "lcd1602.h"

/******************************************************************************
 * Private definitions
 ******************************************************************************/

/* LCD functions */
#define LCD_CLEARDISPLAY    0x01
#define LCD_SETDDRAMADDR    0x80
#define LCD_FULLLINE        0x40

/* PCF8574 */
#define PCF8574_ADDRESS     0x27 
#define PCF8574A_ADDRESS    0x3f

/* PCF8574 connections to LCD */
#define PCF8574_BL          0x08    /* Backlight */
#define PCF8574_EN          0x04    /* Enable bit */
#define PCF8574_RW          0x02    /* Read/Write bit (0 = write) */
#define PCF8574_RS          0x01    /* Register select bit */

/****************************************************************************** 
 * Private memory declarations
 ******************************************************************************/

static uint8_t lcd_backlight = 1;
static uint8_t pcf_address = PCF8574_ADDRESS;

/****************************************************************************** 
 * Private prototypes
 ******************************************************************************/

void PCF8574_Write(uint8_t data);
void LCD1602_Write4(uint8_t data, uint8_t rs);
void LCD1602_Write8(uint8_t data, uint8_t rs);
void LCD1602_CheckAddress(void);

char Lvl_1[] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1f};          // lvl 1 bar
char Lvl_2[] = {0x0,0x0,0x0,0x0,0x0,0x0,0x1f,0x1f};         // lvl 2 bar
char Lvl_3[] = {0x0,0x0,0x0,0x0,0x0,0x1f,0x1f,0x1f};        // lvl 3 bar
char Lvl_4[] = {0x0,0x0,0x0,0x0,0x1f,0x1f,0x1f,0x1f};       // lvl 4 bar
char Lvl_5[] = {0x0,0x0,0x0,0x1f,0x1f,0x1f,0x1f,0x1f};      // lvl 5 bar
char Lvl_6[] = {0x0,0x0,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f};     // lvl 6 bar
char Lvl_7[] = {0x0,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f};    // lvl 7 bar
char Lvl_8[] = {0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f};   // lvl 8 bar

void DELAY(uint32_t x);   

/****************************************************************************** 
 * Function definitions
 ******************************************************************************/

/**-----------------------------------------------------------------------------
 * @brief Sets cursor into given position
 */
void LCD1602_Init(void) {                                                                                                                            
    I2C_Init();                    /* via I2C communication */
    LCD1602_CheckAddress();        /* if any PCF connected check which one */
                                                                                        
    DELAY(168);                    /* >15ms */
    
    LCD1602_Write8(0x33,0);        /* 4-bit interface */                                
    LCD1602_Write8(0x32,0);        /* HD44780U datasheet Figure 24 */
    LCD1602_Write8(0x28,0);
    LCD1602_Write8(0x08,0);
    LCD1602_Write8(0x01,0);
    LCD1602_Write8(0x0C,0);        /* cursor off, blink off */
}

/**-----------------------------------------------------------------------------
 * @brief     Sets cursor into given position
 * @param[in] Column number
 * @param[in] Row number
 */
void LCD1602_SetCursor(uint8_t col, uint8_t row) {
    if( row>1 ) 
        row = 1;    /* prevents from too many rows */
    if( col>39 )    
        col = 39;    /* prevents from being over range */
    
    /* prevents from incorrect instruction */
    LCD1602_Write8((LCD_SETDDRAMADDR |(col+(LCD_FULLLINE*row))),0);        
}

/**-----------------------------------------------------------------------------
 * @brief     Prints string to LCD
 * @param[in] Data to send.
 */
void LCD1602_Print(char *str) {
    uint8_t str_len = 0;

    /* until end of string */
    while( str[str_len] != '\0' ) {              
        LCD1602_Write8(str[str_len], 1);
        ++str_len;
    }
}

/**-----------------------------------------------------------------------------
 * @brief Clears whole display
 */
void LCD1602_ClearAll(void) {
    LCD1602_Write8(LCD_CLEARDISPLAY, 0);
}

/**-----------------------------------------------------------------------------
 * @brief     Write byte to LCD including backlight info.
 * @param[in] Data to send.
 */
void PCF8574_Write(uint8_t data) {
    I2C_Write(pcf_address, data | (lcd_backlight?PCF8574_BL:0x00));
}

/**-----------------------------------------------------------------------------
 * @brief     Write 4 bits to LCD.
 * @param[in] A lower nibble of the byte.
 * @param[in] Register select.
 */
void LCD1602_Write4(uint8_t data, uint8_t rs) {
    PCF8574_Write(((data << 4)&0xF0) | (rs?PCF8574_RS:0x00) | PCF8574_EN);
    PCF8574_Write(((data << 4)&0xF0) | (rs?PCF8574_RS:0x00));
    DELAY(1);
}

/**-----------------------------------------------------------------------------
 * @brief     Write byte to LCD.
 * @param[in] Data to send.
 * @param[in] Register select
 */
void LCD1602_Write8(uint8_t data, uint8_t rs) {
    /* Prevent keypad interrupt from corrupting transmission */
    NVIC_DisableIRQ(PORTA_IRQn);
    
    LCD1602_Write4(((data >> 4)&0x0F), rs);
    LCD1602_Write4(( data      &0x0F), rs);
    
    /* Enable keypad interrupt */
    NVIC_EnableIRQ(PORTA_IRQn);
}

/**-----------------------------------------------------------------------------
 * @brief Check which version of PCF is connected.
 */
void LCD1602_CheckAddress(void) {
    if( (I2C_Write(PCF8574_ADDRESS, 0x00) & I2C_ERR_NOACK)==0 ) 
        pcf_address = PCF8574_ADDRESS;
    if( (I2C_Write(PCF8574A_ADDRESS, 0x00) & I2C_ERR_NOACK)==0 ) 
        pcf_address = PCF8574A_ADDRESS;
}

/**-----------------------------------------------------------------------------
 * @brief     Read busy flag and address counter
 * @param[in] pointer to uint8_t which stores address counter value
 */
uint8_t LCD1602_BF_AC(uint8_t *ptr) {
    uint8_t buf, bf_flag;
    uint8_t temp_dh, temp_dl;
    buf=0xf0;
    
    buf |=((lcd_backlight?PCF8574_BL:0x00) | PCF8574_RW);
    I2C_Write(pcf_address, buf);
    buf |= PCF8574_EN;
    I2C_Write(pcf_address, buf);
    I2C_Read(pcf_address, &temp_dh);
    
    buf &= (~PCF8574_EN);
    I2C_Write(pcf_address, buf);
    buf |= PCF8574_EN;
    I2C_Write(pcf_address, buf);
    I2C_Read(pcf_address, &temp_dl);
    
    buf &= (~(PCF8574_EN | PCF8574_RW));
    I2C_Write(pcf_address, buf);
    buf = ((temp_dh & 0xf0) | (temp_dl>>4));
    *ptr = buf;
    bf_flag = buf & 0x80;
    
    return bf_flag;    
}

/**-----------------------------------------------------------------------------
 * @brief Load custom characters to LCD
 */
void LCD1602_LVL_CH(void) {
    uint8_t i,temp;
    
    /* Set CGRAM address = 0 */
    LCD1602_Write8(0x40,0);        
    
    for( i=0; i<8; i++ ) {
        while( LCD1602_BF_AC(&temp) );
        LCD1602_Write8(Lvl_1[i],1);
    }
    for( i=0; i<8; i++ ) {
        while( LCD1602_BF_AC(&temp) );
        LCD1602_Write8(Lvl_2[i],1);
    }
    for( i=0; i<8; i++ ) {
        while( LCD1602_BF_AC(&temp) );
        LCD1602_Write8(Lvl_3[i],1);
    }
    for( i=0; i<8; i++ ) {
        while( LCD1602_BF_AC(&temp) );
        LCD1602_Write8(Lvl_4[i],1);
    }
    for( i=0; i<8; i++ ) {
        while( LCD1602_BF_AC(&temp) );
        LCD1602_Write8(Lvl_5[i],1);
    }
    for( i=0; i<8; i++ ) {
        while( LCD1602_BF_AC(&temp) );
        LCD1602_Write8(Lvl_6[i],1);
    }
    for( i=0; i<8; i++ ) {
        while( LCD1602_BF_AC(&temp) );
        LCD1602_Write8(Lvl_7[i],1);
    }
    for( i=0; i<8; i++ ) {
        while( LCD1602_BF_AC(&temp) );
        LCD1602_Write8(Lvl_8[i],1);
    }
    
    /* Set DDRAM address = 0 */
    LCD1602_Write8(0x80,0);        
}

/**-----------------------------------------------------------------------------
 * @brief     Write normalized 8 level amplitude bar to LCD
 * @param[in] Normalized amplitude level: 1-8
 */
void LCD1602_PrintLVL(char *str) {
    uint8_t str_len = 0;
    char buf;
    
    /* until end of string */
    while( str[str_len] != '\0' ) {
        buf = str[str_len];
        if( buf=='/' ) {
            if( (str[str_len+1] != '\0') && (str[str_len+1]=='/') ) {
                if( str[str_len+2] != '\0' ) {
                    str_len+=2;
                    switch( str[str_len] ) {
                        case '1': buf=0x0;
                                  break;
                        case '2': buf=0x01;
                                  break;
                        case '3': buf=0x02;
                                  break;
                        case '4': buf=0x03;
                                  break;
                        case '5': buf=0x04;
                                  break;
                        case '6': buf=0x05;
                                  break;
                        case '7': buf=0x06;
                                  break;
                        case '8': buf=0x07;
                                  break;
                        default:  str_len-=2;
                                  break;
                    }
                }
            }
        }
    LCD1602_Write8(buf, 1);
    ++str_len;
  }
}

/**-----------------------------------------------------------------------------
 * @deprecated
 * @brief      Write normalized 16-levels amplitude bar to LCD
 * @param[in]  Normalized amplitude level: 0-16
 * @param[in]  Column select
 */
void LCD1602_PrintLVL_16(uint8_t lvl, uint8_t col){
    switch( lvl ){
        case 0: 
            LCD1602_SetCursor(col,0);
            LCD1602_Print(" ");
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL(" ");
            break;
        case 1: 
            LCD1602_SetCursor(col,0);   // select top row
            LCD1602_Print(" ");         // clear top row
            LCD1602_SetCursor(col,1);   // select bottom row
            LCD1602_PrintLVL("//1");    // write one strip
            break;
        case 2: 
            LCD1602_SetCursor(col,0);        
            LCD1602_Print(" ");
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL("//2");
            break;
        case 3: 
            LCD1602_SetCursor(col,0);
            LCD1602_Print(" ");
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL("//3");
            break;
        case 4: 
            LCD1602_SetCursor(col,0);
            LCD1602_Print(" ");
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL("//4");
            break;
        case 5: 
            LCD1602_SetCursor(col,0);
            LCD1602_Print(" ");
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL("//5");
            break;
        case 6: 
            LCD1602_SetCursor(col,0);
            LCD1602_Print(" ");
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL("//6");
            break;
        case 7: 
            LCD1602_SetCursor(col,0);
            LCD1602_Print(" ");
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL("//7");
            break;
        case 8: 
            LCD1602_SetCursor(col,0);            
            LCD1602_Print(" ");
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL("//8");
            break;
        case 9: 
            LCD1602_SetCursor(col,0);   // select top row
            LCD1602_PrintLVL("//1");    // write first strip
            LCD1602_SetCursor(col,1);   // select bottom row
            LCD1602_PrintLVL("//8");    // write first 8 strips
            break;
        case 10: 
            LCD1602_SetCursor(col,0);
            LCD1602_PrintLVL("//2");
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL("//8");
            break;
        case 11: 
            LCD1602_SetCursor(col,0);
            LCD1602_PrintLVL("//3");
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL("//8");
            break;
        case 12: 
            LCD1602_SetCursor(col,0);
            LCD1602_PrintLVL("//4");
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL("//8");
            break;
        case 13: 
            LCD1602_SetCursor(col,0);
            LCD1602_PrintLVL("//5");
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL("//8");
            break;
        case 14: 
            LCD1602_SetCursor(col,0);
            LCD1602_PrintLVL("//6");
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL("//8");
            break;
        case 15: 
            LCD1602_SetCursor(col,0);
            LCD1602_PrintLVL("//7");
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL("//8");
            break;
        case 16: 
            LCD1602_SetCursor(col,0);
            LCD1602_PrintLVL("//8");
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL("//8");
            break;
        default:
            break;
    }
}

/**-----------------------------------------------------------------------------
 * @brief     Write normalized 16-levels amplitude bar to LCD. Function dedicated
 *            for a spectrum analyzer, does not do any unnecessary prints and 
 *            works faster
 * @param[in] Normalized amplitude level: 0-16
 * @param[in] Previous normalized amplitude level: 0-16
 * @param[in] Column number
 */
void LCD1602_PrintLVL_16_FFT(uint8_t lvl, uint8_t oldLvl, uint8_t col){
    switch( lvl ){
        case 0:
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL(" ");
            break;
        case 1:
            LCD1602_SetCursor(col,1);   // select bottom row
            LCD1602_PrintLVL("//1");    // write one strip
            break;
        case 2:
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL("//2");
            break;
        case 3:
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL("//3");
            break;
        case 4:
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL("//4");
            break;
        case 5:
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL("//5");
            break;
        case 6:
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL("//6");
            break;
        case 7:
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL("//7");
            break;
        case 8:
            LCD1602_SetCursor(col,1);
            LCD1602_PrintLVL("//8");
            break;
        case 9: 
            LCD1602_SetCursor(col,0);   // select top row
            LCD1602_PrintLVL("//1");    // write first strip
            break;
        case 10: 
            LCD1602_SetCursor(col,0);
            LCD1602_PrintLVL("//2");
            break;
        case 11: 
            LCD1602_SetCursor(col,0);
            LCD1602_PrintLVL("//3");
            break;
        case 12: 
            LCD1602_SetCursor(col,0);
            LCD1602_PrintLVL("//4");
            break;
        case 13: 
            LCD1602_SetCursor(col,0);
            LCD1602_PrintLVL("//5");
            break;
        case 14: 
            LCD1602_SetCursor(col,0);
            LCD1602_PrintLVL("//6");
            break;
        case 15: 
            LCD1602_SetCursor(col,0);
            LCD1602_PrintLVL("//7");
            break;
        case 16: 
            LCD1602_SetCursor(col,0);
            LCD1602_PrintLVL("//8");
            break;
        default:
            break;
    }
    
    /* if current level is lower than 9 (upper row needs to be empty) but
       previos value was greater than 8 (upper row was not empty), clear 
       upper row; similarly for current lever higher than 8 */
    if( lvl < 9 && oldLvl > 8 ) {
        LCD1602_SetCursor(col,0);
        LCD1602_Print(" ");
    }
    else if( lvl >= 9 && oldLvl < 8 ) {
        LCD1602_SetCursor(col,1);
        LCD1602_PrintLVL("//8");
    }
}

/**-----------------------------------------------------------------------------
 * @brief Simple delay function used only in I2C
 */
void DELAY(uint32_t x) {
    for( volatile uint32_t i=0; i<(x*8000); i++ )
        ;
}
