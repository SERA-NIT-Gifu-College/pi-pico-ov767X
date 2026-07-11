#include "pi-pico-ov767X/camera.h"
#include "camera-io.pio.h"

#include <hardware/dma.h>
#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <hardware/irq.h>
#include <hardware/pio.h>
#include <hardware/regs/intctrl.h>
#include <hardware/structs/io_bank0.h>
#include <hardware/uart.h>
#include <pico/stdio.h>
#include <pico/stdio_uart.h>
#include <pico/stdlib.h>
#include <stdio.h>

static uint dma_chan_framebuffer_tx;
volatile uint16_t frameBuffer[QVGA_FRAME_SIZE_OV767X] = {0};
volatile bool isNewFrameAvailable = false;

void pio_handler(void) {
    printf("Interrupt from PIO\n");
    isNewFrameAvailable = true;
    dma_channel_set_write_addr(dma_chan_framebuffer_tx, &frameBuffer[0], true);
    pio_interrupt_clear(pio0, 0);
}

int main() {
    stdio_uart_init_full(uart0, 960000, 16, 17);
    gpio_set_function(16, GPIO_FUNC_UART);
    gpio_set_function(17, GPIO_FUNC_UART);

    sleep_ms(3000);

    Camera_IO cam_io = {
        .pio = pio0,
        .sm_line_scan = 0,
        .sm_frame_capture = 1,
        .sm_stream_frames = 2,
        .ofst_line_scan = 0,
        .ofst_frame_capture = 0,
        .ofst_stream_frames = 0
    };

    setup_camera_io(&cam_io, 2, 320, 240, false, pio_handler);

    dma_chan_framebuffer_tx = dma_claim_unused_channel(true);
    dma_channel_config dma_cfg = dma_channel_get_default_config(dma_chan_framebuffer_tx);
    channel_config_set_dreq(&dma_cfg, pio_get_dreq(cam_io.pio, cam_io.sm_line_scan, false));
    channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_8);
    channel_config_set_read_address_update_type(&dma_cfg, DMA_ADDRESS_UPDATE_NONE);
    channel_config_set_write_address_update_type(&dma_cfg, DMA_ADDRESS_UPDATE_INCREMENT);
    dma_channel_configure(
        dma_chan_framebuffer_tx,
        &dma_cfg,
        &frameBuffer[0],
        &((cam_io.pio)->rxf[cam_io.sm_line_scan]),
        QVGA_FRAME_SIZE_OV767X * 2,
        true
    );

    gpio_init(0);
    gpio_init(1);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    i2c_init(i2c0, 400000);

    Camera_OV767X camera = {0};

    initialize_OV767X(&camera, (InitArgs_OV767X){
        .sccb_inst = i2c0,
        .sccb_dev_addr = 0x21,
        .xclk_pin = 13,
        .format = RGB565,
        .resolution = QVGA
    });

    setupClock_OV767X(&(camera.clk));
    reset_OV767X(&(camera.sccb));
    config_OV767X(&(camera.sccb), camera.format, camera.resolution);

    while (1) {
        printf("Press 'c' to start capture\n");
        while (getchar() != 'c') ;
        start_capture_single(&cam_io);

        while (!isNewFrameAvailable) ; // wait for new frame

        // print out image in ANSI color esacpe sequence
        uint8_t r, g, b;
        uint16_t c;
        for (uint i = 0; i < QVGA_FRAME_SIZE_OV767X; i++) {
            c = frameBuffer[i];
            r = (c & 0xF800) >> 11;
            g = (c & 0x07E0) >> 5;
            b = (c & 0x001F);

            r = r * 255 / 31;
            g = g * 255 / 63;
            b = b * 255 / 31;
            printf("\x1B[48;2;%d;%d;%dm  \x1B[0m", r, g, b);

            if (i % 320 == 319) {
                printf("\r\n");
            }
        }
        isNewFrameAvailable = false;
        fflush(stdout);
    }

    return 0;
}
