#include "pi-pico-ov767X/camera.h"
#include "pi-pico-ov767X/sccb.h"
#include "pi-pico-ov767X/camera_regs.h"

#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

void initialize_OV767X(Camera_OV767X *camera, InitArgs_OV767X args) {
    camera->format = args.format;
    camera->resolution = args.resolution;
    camera->sccb.inst = args.sccb_inst;
    camera->sccb.devAddr = args.sccb_dev_addr;
    camera->clk.pin = args.xclk_pin;
    camera->clk.enabled = false;
}

void reset_OV767X(SCCB_OV767X *sccb) {
    sccbWriteByte(sccb, COM7, 0x80);
    sleep_ms(10);
}

void config_OV767X(SCCB_OV767X *sccb, Format_OV767X fmt, Resolution_OV767X res) {
    for (uint8_t i = 0; i < OV767X_COM_CFG_CNT; i++) {
        sccbWriteByte(sccb, OV767X_COM_CFG[i][0], OV767X_COM_CFG[i][1]);
    }

    setFormat_OV767X(sccb, fmt);
    setResolution_OV767X(sccb, res);
}

void setFormat_OV767X(SCCB_OV767X *sccb, Format_OV767X fmt) {
    uint8_t com7_val = 0;

    sccbReadByte(sccb, COM7, &com7_val);
    com7_val &= ~(0x05);

    switch (fmt) {
        case RGB565:
            com7_val |= 0x04;
            sccbWriteByte(sccb, COM7, com7_val);
            for (uint8_t i = 0; i < OV767X_RGB565_CFG_CNT; i++) {
                sccbWriteByte(sccb, OV767X_RGB565_CFG[i][0], OV767X_RGB565_CFG[i][1]);
            }
            break;
        case YUYV:
            com7_val |= 0x00;
            sccbWriteByte(sccb, COM7, com7_val);
            for (uint8_t i = 0; i < OV767X_YUV422_CFG_CNT; i++) {
                sccbWriteByte(sccb, OV767X_YUV422_CFG[i][0], OV767X_YUV422_CFG[i][1]);
            }
            break;
    }
}

void setResolution_OV767X(SCCB_OV767X *sccb, Resolution_OV767X res) {
    uint8_t com7_val = 0;

    sccbReadByte(sccb, COM7, &com7_val);
    com7_val &= ~(0x38);

    switch (res) {
        case VGA:
            com7_val |= 0x00; // set COM7's Output format to VGA
            sccbWriteByte(sccb, COM7, com7_val);
            for (uint8_t i = 0; i < OV767X_VGA_CFG_CNT; i++) {
                sccbWriteByte(sccb, OV767X_VGA_CFG[i][0], OV767X_VGA_CFG[i][1]);
            }
            setWindow_OV767X(sccb, 158, 10);
            break;
        case QVGA:
            com7_val |= 0x00; // set COM7's Output format to VGA
            sccbWriteByte(sccb, COM7, com7_val);
            for (uint8_t i = 0; i < OV767X_QVGA_CFG_CNT; i++) {
                sccbWriteByte(sccb, OV767X_QVGA_CFG[i][0], OV767X_QVGA_CFG[i][1]);
            }
            setWindow_OV767X(sccb, 176, 12);
            break;
        case QQVGA:
            com7_val |= 0x00; // set COM7's Output format to VGA
            sccbWriteByte(sccb, COM7, com7_val);
            for (uint8_t i = 0; i < OV767X_QQVGA_CFG_CNT; i++) {
                sccbWriteByte(sccb, OV767X_QQVGA_CFG[i][0], OV767X_QQVGA_CFG[i][1]);
            }
            setWindow_OV767X(sccb, 184, 10);
            break;
    }
}

void setupClock_OV767X(CameraClock_OV767X *clk) {
    if (clk->enabled) {
        return;
    }
    clk->enabled = true;

    gpio_set_function(clk->pin, GPIO_FUNC_PWM);

    clk->slice_num = pwm_gpio_to_slice_num(clk->pin);

    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_wrap(&cfg, 3);
    // 16 MHz clock, scaled from prescaled sys clk by factor of 4 [pwm cycles]
    pwm_config_set_clkdiv(&cfg, (clock_get_hz(clk_sys) / 4.f) / (16.f * 1000.f * 1000.f));

    pwm_init(clk->slice_num, &cfg, true);
    pwm_set_gpio_level(clk->pin, 2);
}

void setWindow_OV767X(SCCB_OV767X *sccb, uint16_t hStart, uint16_t vStart) {
    uint8_t tmp;

    uint16_t hStop = hStart + 640 - 784;
    uint16_t vStop = vStart + 480;

    sccbWriteByte(sccb, HSTART, (uint8_t)((hStart & 0x07F8) >> 3));
    sccbWriteByte(sccb, HSTOP, (uint8_t)((hStop & 0x07F8) >> 3));
    sccbReadByte(sccb, HREF, &tmp);
    tmp &= ~(0xC0);
    sccbWriteByte(sccb, HREF, tmp | (uint8_t)(((hStop & 0x0007) << 3) | (hStart & 0x0007)));

    sccbWriteByte(sccb, VSTART, (uint8_t)((vStart & 0x03FC) >> 2));
    sccbWriteByte(sccb, VSTOP, (uint8_t)((vStop & 0x03FC) >> 2));
    sccbReadByte(sccb, VREF, &tmp);
    tmp &= ~(0xF0);
    sccbWriteByte(sccb, VREF, tmp | (uint8_t)(((vStop & 0x0003) << 2) | (vStart & 0x0003)));
}
