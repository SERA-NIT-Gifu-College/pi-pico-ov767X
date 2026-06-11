#include "pi-pico-ov767X/camera_regs.h"
#include <stdint.h>

const uint8_t OV767X_COM_CFG[OV767X_COM_CFG_CNT][2] = {
    { TSLB, 0x04 },
    { CLKRC, 0x80 | 0x00 }, // no clk div with reserved set
    { GAIN, 0x00 },
    { AECH, 0x00 },
    { COM4, 0x40 },
    { COM9, 0x10 | 0x08 }, // AGC 4x | reserved
    { BD50MAX, 0x05 },
    { BD60MAX, 0x07 },
    { AEW, 0x95 },
    { VPT, 0xE3 },
    { AEB, 0x33 },
    { HAECC1, 0x78 },
    { HAECC2, 0x68 },
    { 0xA1, 0x03 }, // reserved
    { HAECC3, 0xD8 },
    { HAECC4, 0xD8 },
    { HAECC5, 0xF0 },
    { HAECC6, 0x90 },
    { HAECC7, 0x94 },
    { COM5, 0x61 }, // reserved
    { COM6, 0x4b }, // enable timing reset on format change
    { MVFP, 0x07 }, // virtical & horizontal flip
    { COM12, 0x78 },
    { DBLV, 0x00 }, // Bypass PLL, Enable Internal Regulator
    { REG74, 0x19 }, // Digital Gain x1
    { 0xb0, 0x84 }, // reserved
    { ABLC1, 0x0C },
    { THL_ST, 0x82 },
    { AWBCTR0, 0x9F },
    { AWBCTR1, 0x11 },
    { AWBCTR2, 0x55 },
    { AWBCTR3, 0x0A },
    { GGAIN, 0x40 },
    { BLUE, 0x40 },
    { RED, 0x60 },
    { COM8, 0xFF },
    { COM16, 0x08 }, // AWB Gain Enable
    { EDGE, 0x00 },
    { REG75, 0x05 },
    { REG76, 0xE1 },
    { DNSTH, 0x00 },
    { REG77, 0x01 },
    { COM13, 0xC3 },
    { SATCTR, 0x60 },
    { COM16, 0x38 }, // YUV Enhancement, De-noise, AWB Gain Enable
    { CONTRAS, 0x40 },
    { COM11, 0x10 | 0x02 }, // 50/60 FPS Auto detect, Exposure timing
    { NT_CTRL, 0x80 | 0x08 | 0x00 } // reserved, auto frame rate adjust, dummy row x2 gain
};

const uint8_t OV767X_RGB565_CFG[OV767X_RGB565_CFG_CNT][2] = {
    { COM1, 0x00},
    { COM15, 0xC0 | 0x10 }, // Value Range 00 ~ FF, RGB565
    { MTX1, 0xB3 },
    { MTX2, 0xB3 },
    { MTX3, 0x00 },
    { MTX4, 0x3D },
    { MTX5, 0xA7 },
    { MTX6, 0xE4 },
    { MTXS, 0x9E },
    { COM13, 0x80 | 0x40 }, // Gamma enable, UV Saturation enable
};

const uint8_t OV767X_YUV422_CFG[OV767X_YUV422_CFG_CNT][2] = {
    { COM1, 0x00 },
    { COM15, 0xC0 },
    { MTX1, 0x80 },
    { MTX2, 0x80 },
    { MTX3, 0x00 },
    { MTX4, 0x22 },
    { MTX5, 0x5E },
    { MTX6, 0x80 },
    { MTXS, 0x9E },
    { COM13, 0x80| 0x40 | 0x01 } // Gamma, UV Saturation, UV Swap Enable
};

const uint8_t OV767X_VGA_CFG[OV767X_VGA_CFG_CNT][2] = {
    { COM3, 0x00 }, // no scaling
    { COM14, 0x00 },
    { SCALING_XSC, 0x3A },
    { SCALING_YSC, 0x35 },
    { SCALING_DCWCTL, 0x00 },
    { SCALING_PCLK_DIV, 0xF0 | 0x00 }, // reserved, PCLK div 1
    { SCALING_PCLK_DELAY, 0x02 }
};

const uint8_t OV767X_QVGA_CFG[OV767X_QVGA_CFG_CNT][2] = {
    { COM3, 0x02 }, // PCLK scaling
    { COM14, 0x10 | 0x01 }, // PCLK scaling, div 2
    { SCALING_XSC, 0x3A },
    { SCALING_YSC, 0x35 },
    { SCALING_DCWCTL, 0x10 | 0x01 }, // VDS div 2, HDS div 2
    { SCALING_PCLK_DIV, 0xF0 | 0x01 }, // reserved, PCLK div 2
    { SCALING_PCLK_DELAY, 0x02 }
};

const uint8_t OV767X_QQVGA_CFG[OV767X_QQVGA_CFG_CNT][2] = {
    { COM3, 0x02 }, // PCLK scaling
    { COM14, 0x10 | 0x08 | 0x02 }, // PCLK scaling, manual scaling, div 4
    { SCALING_XSC, 0x3A },
    { SCALING_YSC, 0x35 },
    { SCALING_DCWCTL, 0x20 | 0x02 }, // VDS div 4, HDS div 4
    { SCALING_PCLK_DIV, 0xF0 | 0x02 }, // reserved, PCLK div 4
    { SCALING_PCLK_DELAY, 0x02 }
};
