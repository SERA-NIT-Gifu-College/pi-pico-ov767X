#include "pi-pico-ov767X/sccb.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pi-pico-ov767X/camera.h"

int main() {
    i2c_init(i2c0, 400000);

    SCCB_OV767X camera = {
        .inst = i2c0,
        .devAddr = 0x21
    };

    reset_OV767X(&camera);
    config_OV767X(&camera, RGB565, VGA);

    while (1) ;

    return test();
}
