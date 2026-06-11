#include "pi-pico-ov767X/sccb.h"
#include <hardware/i2c.h>

void sccbReadByte(SCCB_OV767X *sccb, uint8_t reg, uint8_t *dest) {
    i2c_write_blocking(sccb->inst, sccb->devAddr, (const uint8_t*)&reg, 1, true);
    i2c_read_blocking(sccb->inst, sccb->devAddr, dest, 1, true);
}

void sccbWriteByte(SCCB_OV767X *sccb, uint8_t reg, uint8_t dat) {
    uint8_t buff[2] = {
        reg,
        dat
    };

    i2c_write_blocking(sccb->inst, sccb->devAddr, buff, 2, false);
}

void sccbWriteBytes(SCCB_OV767X *sccb, const RegVal regVal, uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        sccbWriteByte(sccb, regVal[i][0], regVal[i][1]);
    }
}
