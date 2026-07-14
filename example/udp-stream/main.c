#include <cyw43_ll.h>
#include <hardware/gpio.h>
#include <hardware/structs/io_bank0.h>
#include <lwip/err.h>
#include <lwip/ip_addr.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <pico/time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pico/cyw43_arch.h>

#include "camera-io.pio.h"
#include "pi-pico-ov767X/camera.h"

#include <hardware/dma.h>
#include <hardware/i2c.h>
#include <hardware/irq.h>
#include <hardware/pio.h>

#include "lwip/pbuf.h"
#include "lwip/udp.h"

#define UDP_PORT 4444
#ifndef TARGET
#define TARGET "192.168.1.120"
#endif

struct udp_pcb* pcb;
ip_addr_t addr;
err_t er;

static uint dma_chan_framebuffer_tx;
volatile uint16_t frameBuffer[QQVGA_FRAME_SIZE_OV767X] = {0};
volatile bool isNewFrameAvailable = false;

Camera_IO cam_io = {
    .pio = pio0,
    .sm_line_scan = 0,
    .sm_frame_capture = 1,
    .sm_stream_frames = 2,
    .ofst_line_scan = 0,
    .ofst_frame_capture = 0,
    .ofst_stream_frames = 0
};

void pio_frame_complete_handler(void) {
    isNewFrameAvailable = true;
    dma_channel_set_write_addr(dma_chan_framebuffer_tx, &frameBuffer[0], true);
    pio_interrupt_clear(cam_io.pio, 0);
}

void init_udp_server() {
    pcb = udp_new();

    ipaddr_aton(TARGET, &addr);
}

void udp_broadcast() {
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, QQVGA_FRAME_SIZE_OV767X*sizeof(uint16_t), PBUF_RAM);
    er = pbuf_take(p, frameBuffer, QQVGA_FRAME_SIZE_OV767X*sizeof(uint16_t));
    if (er != ERR_OK) {
        printf("Failed to copy from framebuffer\r\n");
        goto free_buffer;
    }
    er = udp_connect(pcb, &addr, UDP_PORT);
    if (er != ERR_OK) {
        printf("Failed to connect\r\n");
        goto free_buffer;
    }
    er = udp_send(pcb, p);
    if (er != ERR_OK) {
        printf("Failed to send UDP packet! error=%d\r\n", er);
        goto disconnect;
    }
disconnect:
    udp_disconnect(pcb);
free_buffer:
    pbuf_free(p);
    return;
}

int main(void) {
    stdio_init_all();

    sleep_ms(5000);

    if(cyw43_arch_init()) {
        printf("Failed to Init CYW43\r\n");
        return 1;
    }

    cyw43_arch_enable_sta_mode();

    printf("Connecting to WiFi...\r\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Failed to Connect to WiFi\r\n");
        return 1;
    }
    printf("Connected!\r\n");

    printf("Setting up Camera...\r\n");

    setup_camera_io(&cam_io, 2, 160, 120, false, pio_frame_complete_handler);

    dma_chan_framebuffer_tx = dma_claim_unused_channel(true);
    dma_channel_config dma_cfg = dma_channel_get_default_config(dma_chan_framebuffer_tx);
    channel_config_set_dreq(&dma_cfg, pio_get_dreq(cam_io.pio, cam_io.sm_line_scan, false));
    channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_8);
    channel_config_set_read_address_update_type(&dma_cfg, DMA_ADDRESS_UPDATE_NONE);
    channel_config_set_write_address_update_type(&dma_cfg, DMA_ADDRESS_UPDATE_INCREMENT);
    dma_channel_configure(dma_chan_framebuffer_tx,
        &dma_cfg,
        &frameBuffer[0],
        &((cam_io.pio)->rxf[cam_io.sm_line_scan]),
        QQVGA_FRAME_SIZE_OV767X * 2,
        true
    );

    gpio_init(0);
    gpio_init(1);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    i2c_init(i2c0, 400 * 1000);

    Camera_OV767X camera = {0};
    initialize_OV767X(&camera, (InitArgs_OV767X){
        .sccb_inst = i2c0,
        .sccb_dev_addr = 0x21,
        .xclk_pin = 13,
        .format = RGB565,
        .resolution = QQVGA
    });

    setupClock_OV767X(&(camera.clk));
    reset_OV767X(&(camera.sccb));
    config_OV767X(&(camera.sccb), camera.format, camera.resolution);

    printf("Camera Setup Complete\r\n");

    init_udp_server();

    uint i = 0;
    while (1) {
        start_capture_single(&cam_io);
        while (!isNewFrameAvailable) ;

        if (isNewFrameAvailable) {
            printf("Broadcasting frame #%d ...\r\n", i++);
            udp_broadcast();
            isNewFrameAvailable = false;
        }

        sleep_ms(100);
    }

    cyw43_arch_disable_sta_mode();
    cyw43_arch_deinit();

    return 0;
}
