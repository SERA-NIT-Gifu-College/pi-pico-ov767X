#ifndef __PI_PICO_OV767X_H__
#define __PI_PICO_OV767X_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "sccb.h"

typedef enum {
    RGB565,
    YUYV
} Format_OV767X;

typedef enum {
    VGA,
    QVGA,
    QQVGA
} Resolution_OV767X;

uint8_t test(void);

void reset_OV767X(SCCB_OV767X *sccb);

void config_OV767X(
    SCCB_OV767X *sccb,
    Format_OV767X fmt,
    Resolution_OV767X res
);

#ifdef __cplusplus
}
#endif

#endif // __PI_PICO_OV767X_H__
