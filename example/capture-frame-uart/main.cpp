#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pi-pico-ov767X/camera.h"

int main() {
    i2c_init(i2c0, 400000);

    Camera_OV767X camera = {0};

    initialize_OV767X(&camera, (InitArgs_OV767X){
        .sccb_inst = i2c0,
        .sccb_dev_addr = 0x21,
        .xclk_pin = 0,
        .format = YUYV,
        .resolution = QQVGA
    });

    while (1) ;

    return 0;
}
