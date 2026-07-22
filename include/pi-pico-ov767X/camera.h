/**
 * @file camera.h
 * @brief Declaration of camera related functions
 * @details This file contains declaration of camera control functions and descriptor structures.
 * @author kenryuS
 */

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

/**
 * @enum
 * Color Format
 */
enum Format_OV767X {
    RGB565,
    YUV422
};
typedef enum Format_OV767X Format_OV767X;

/**
 * @enum
 * Image Resolution
 */
enum Resolution_OV767X {
    VGA,
    QVGA,
    QQVGA
};
typedef enum Resolution_OV767X Resolution_OV767X;

/**
 * XCLK Generator Descriptor
 */
struct CameraClock_OV767X {
    uint8_t pin; /**< GPIO pin to output clock signal */
    uint8_t slice_num; /**< PWM slice number corresponding to pin */
    bool enabled; /**< Whether if clock is running */
};
typedef struct CameraClock_OV767X CameraClock_OV767X;

/**
 * Camera Descriptor
 */
struct Camera_OV767X {
    SCCB_OV767X sccb; /**< SCCB interface to camera */
    CameraClock_OV767X clk; /**< XCLK configuration for camera */
    Format_OV767X format; /**< Color format configuration for camera */
    Resolution_OV767X resolution; /**< Image resolution configuration for camera */
};
typedef struct Camera_OV767X Camera_OV767X;

/**
 * Arguments for initialize_OV767X
 * @see initialize_OV767X
 */
struct InitArgs_OV767X {
    i2c_inst_t* sccb_inst; /**< I2C instance which camera SCCB interface is connected to */
    uint8_t sccb_dev_addr; /**< 7 bit device address to camera */
    uint8_t xclk_pin; /**< Pin to be used for XCLK generation */
    Format_OV767X format; /**< Color format to use */
    Resolution_OV767X resolution; /**< Image resultion to use */
};
typedef struct InitArgs_OV767X InitArgs_OV767X;

void reset_OV767X(SCCB_OV767X *sccb);

void setupClock_OV767X(CameraClock_OV767X *clk);

void startClock_OV767X(CameraClock_OV767X *clk);

void stopClock_OV767X(CameraClock_OV767X *clk);

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
