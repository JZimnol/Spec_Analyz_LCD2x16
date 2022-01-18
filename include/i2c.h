/******************************************************************************
 * This file is a part of the SM2 Tutorial (C).                               *
 ******************************************************************************/

/**
 * @file   i2c.h
 * @author Koryciak & Maj & Zimnol
 * @date   Dec 2021
 * @brief  File containing enums, structures and declarations for I2C.
 * @ver    0.5
 */

#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include "MKL25Z4.h"  

/****************************************************************************** 
 * Global definitions
 ******************************************************************************/

#define I2C_ERR_TIMEOUT          0x01         /* error = timeout */
#define I2C_ERR_NOACK            0x02         /* error = no ACK from slave  */

/**
 * @brief I2C initialization.
 */
void I2C_Init(void);

/**
 * @brief     Send via I2C only device address (write). In response check error type.
 * @param[in] Address of slave.
 * @return    Errors.
 */
uint8_t I2C_Ping(uint8_t address);

/**
 * @brief     I2C basic write operation. Write 8 bits to specified device address.
 *            Works best with I/O expanders.
 * @param[in] Address of slave.
 * @param[in] Data to write.
 * @return    Errors.
 */
uint8_t I2C_Write(uint8_t address, uint8_t data);

/**
 * @brief        I2C basic read operation. Read 8 bits from specified device address.
 *               Works best with I/O expanders.
 * @param[in]    Address of slave.
 * @param[inout] Data from slave.
 * @return       Errors.
 */
uint8_t I2C_Read(uint8_t address, uint8_t* data);

/**
 * @brief     I2C write to register.  
 * @param[in] Address of slave.
 * @param[in] Register.
 * @param[in] Data to write.
 * @return    Errors.
 */
uint8_t I2C_WriteReg(uint8_t address, uint8_t reg, uint8_t data);

/**
 * @brief        I2C read from register.
 * @param[in]    Address of slave.
 * @param[in]    Register.
 * @param[inout] Data from slave device.
 * @return       Errors.
 */
uint8_t I2C_ReadReg(uint8_t address, uint8_t reg, uint8_t* data);

/**
 * @brief        I2C read from block of registers (autoincrementation).
 * @param[in]    Address of slave.
 * @param[in]    Start register.
 * @param[in]    Count of registers to read.
 * @param[inout] Data from slave device.
 * @return       Errors.
 */
uint8_t I2C_ReadRegBlock(uint8_t address, uint8_t reg, uint8_t size, uint8_t* data);

#endif /* I2C_H */
