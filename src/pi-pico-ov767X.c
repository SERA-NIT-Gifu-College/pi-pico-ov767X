#include "pi-pico-ov767X/camera.h"
#include "pi-pico-ov767X/sccb.h"
#include "pi-pico-ov767X/camera_regs.h"
#include "pico/time.h"

uint8_t test() {
    return 0x69;
}

void reset_OV767X(SCCB_OV767X *sccb) {
    sccbWriteByte(sccb, COM7, 0x80);
    sleep_us(5);
}

void config_OV767X(SCCB_OV767X *sccb, Format_OV767X fmt, Resolution_OV767X res) {
    sccbWriteBytes(sccb, (RegVal)OV767X_COM_CFG, OV767X_COM_CFG_CNT);
    switch (res) {
        case VGA:
            sccbWriteBytes(sccb, (RegVal)OV767X_VGA_CFG, OV767X_VGA_CFG_CNT);
            break;
        case QVGA:
            sccbWriteBytes(sccb, (RegVal)OV767X_QVGA_CFG, OV767X_QVGA_CFG_CNT);
            break;
        case QQVGA:
            sccbWriteBytes(sccb, (RegVal)OV767X_QQVGA_CFG, OV767X_QQVGA_CFG_CNT);
            break;
    }
    switch (fmt) {
        case RGB565:
            sccbWriteBytes(sccb, (RegVal)OV767X_RGB565_CFG, OV767X_RGB565_CFG_CNT);
            break;
        case YUYV:
            sccbWriteBytes(sccb, (RegVal)OV767X_YUV422_CFG, OV767X_YUV422_CFG_CNT);
            break;
    }
}
