#ifndef __PI_PICO_OV767X_H__
#define __PI_PICO_OV767X_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "sccb.h"

#define VGA_FRAME_SIZE_OV767X 640*480
#define QVGA_FRAME_SIZE_OV767X 320*240
#define QQVGA_FRAME_SIZE_OV767X 160*120

typedef enum {
    RGB565,
    YUV422
} Format_OV767X;
typedef enum {
    VGA,
    QVGA,
    QQVGA
} Resolution_OV767X;

typedef struct {
    uint8_t pin;
    uint8_t slice_num;
    bool enabled;
} CameraClock_OV767X;
typedef struct {
    SCCB_OV767X sccb;
    CameraClock_OV767X clk;
    Format_OV767X format;
    Resolution_OV767X resolution;
} Camera_OV767X;
typedef struct {
    i2c_inst_t* sccb_inst;
    uint8_t sccb_dev_addr;
    uint8_t xclk_pin;
    Format_OV767X format;
    Resolution_OV767X resolution;
} InitArgs_OV767X;

void reset_OV767X(SCCB_OV767X *sccb);
void setupClock_OV767X(CameraClock_OV767X *clk);
void initialize_OV767X(
    Camera_OV767X *camera,
    InitArgs_OV767X args
);
void config_OV767X(
    SCCB_OV767X *sccb,
    Format_OV767X fmt,
    Resolution_OV767X res
);

void setFormat_OV767X(
    SCCB_OV767X *sccb,
    Format_OV767X fmt
);
void setResolution_OV767X(
    SCCB_OV767X *sccb,
    Resolution_OV767X res
);
void setWindow_OV767X(
    SCCB_OV767X *sccb,
    uint16_t hStart,
    uint16_t vStart
);

#ifdef __cplusplus
}
#endif

#endif // __PI_PICO_OV767X_H__
