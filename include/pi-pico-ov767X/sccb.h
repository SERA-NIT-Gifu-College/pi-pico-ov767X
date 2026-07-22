/**
 * @file sccb.h
 * @brief Delclaration of simple SCCB IO functions
 * @details This file contains declaration of SCCB IO functions and descriptor structure.
 * @author kenryuS
 */

#ifndef __PI_PICO_OV767X_SCCB_H__
#define __PI_PICO_OV767X_SCCB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "hardware/i2c.h"

/**
 * SCCB Interface Descriptor
 */
struct SCCB_OV767X {
    i2c_inst_t *inst; /**< I2C instance that is connected to the camera */
    uint8_t devAddr; /**< SCCB address of the camera */
};
typedef struct SCCB_OV767X SCCB_OV767X;

/**
 * Read a byte from a register
 * @param sccb A pointer to SCCB interface descriptor
 * @param reg Register address to be read from
 * @param dest A pointer to buffer where the retrived data is stored on
 */
void sccbReadByte(SCCB_OV767X *sccb, uint8_t reg, uint8_t *dest);

/**
 * Write a byte to a register
 * @param sccb A pointer to SCCB interface descriptor
 * @param reg Register address to be read from
 * @param dat A byte of data to be written to
 */
void sccbWriteByte(SCCB_OV767X *sccb, uint8_t reg, uint8_t dat);

#ifdef __cplusplus
}
#endif

#endif // __PI_PICO_OV767X_SCCB_H__
