#ifndef __PI_PICO_OV767X_SCCB_H__
#define __PI_PICO_OV767X_SCCB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "hardware/i2c.h"

typedef unsigned char** RegVal;

typedef struct {
    i2c_inst_t *inst;
    uint8_t devAddr;
} SCCB_OV767X;

void sccbReadByte(SCCB_OV767X *sccb, uint8_t reg, uint8_t *dest);
void sccbWriteByte(SCCB_OV767X *sccb, uint8_t reg, uint8_t dat);
void sccbWriteBytes(SCCB_OV767X *sccb, const RegVal regVal, uint8_t len);


#ifdef __cplusplus
}
#endif

#endif // __PI_PICO_OV767X_SCCB_H__
