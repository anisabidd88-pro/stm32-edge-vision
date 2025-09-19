/*
 * main.c — STM32 Edge Vision example (DCMI + DMA double-buffer + fallback color detector)
 *
 * Notes:
 * - This is a template: adapt pin names, HAL init functions and project settings for your MCU and board.
 */
#include "camera.h"
#include "dma_capture.h"
#include "color_detector.h"
#include "tflm_inference.h"
#include <string.h>
#include <stdio.h>

volatile uint8_t frame_ready = 0;
volatile uint8_t active_buf = 0;

#define FRAME_W 320
#define FRAME_H 240
#define FRAME_SIZE (FRAME_W * FRAME_H * 2) // RGB565 example

/* Double buffers placed in DTCM/SDRAM depending on MCU — tune memory placement via linker script if needed */
static uint8_t frame_buf0[FRAME_SIZE];
static uint8_t frame_buf1[FRAME_SIZE];

/* Callback from DMA/ISR when a frame is captured */
void on_frame_captured(uint8_t which)
{
    active_buf = which;
    frame_ready = 1;
}

int main(void)
{
    HAL_Init();
    SystemClock_Config(); // Provided by CubeMX/CubeIDE project normally
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_DCMI_Init();
    MX_DMA2D_Init();

    camera_init(); // camera sensor init via SCCB/I2C (driver in camera.c)

    /* Start DCMI capture into buffer 0 using DMA (non-blocking).
       The DMA/ISR must call on_frame_captured(0 or 1) when done */
    dma_capture_start(frame_buf0, frame_buf1, FRAME_SIZE, on_frame_captured);

    tflm_init(); // initialize TFLM (if available — plugin is optional)

    printf("Started capture. Waiting frames...\n");

    while (1)
    {
        if (frame_ready)
        {
            /* pick the buffer that just finished */
            uint8_t *frame = (active_buf == 0) ? frame_buf0 : frame_buf1;
            frame_ready = 0;

            // Quick preprocessing (example: convert to grayscale or downsample)
            // Here we call a DMA2D helper or CPU fallback — provided in dma_capture.c
            uint8_t thumbnail[96*96]; // small thumbnail for detector
            if (!dma_downscale_rgb565_to_gray(frame, FRAME_W, FRAME_H, thumbnail, 96, 96))
            {
                // Fallback: CPU path (not implemented here)
            }

            // First, run fast color-detector (works out-of-the-box)
            int color = color_detect_by_average_rgb565(frame, FRAME_W, FRAME_H);
            if (color >= 0) {
                // Example action: print and toggle LED
                printf("Color detected: %d\n", color);
                HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_1);
            }

            // Optionally run TFLM model if integrated and a model blob exists
#ifdef USE_TFLM
            int tflm_label = tflm_run_inference(thumbnail, 96, 96);
            if (tflm_label >= 0) {
                printf("TFLM label: %d\n", tflm_label);
                // send via BLE/WiFi/etc.
            }
#endif
        } // frame_ready
    } // while
}

/* Weak placeholders for MCU-specific functions (real implementations come from CubeMX) */
void SystemClock_Config(void) { /* implement in CubeIDE generated files */ }
void MX_GPIO_Init(void) { /* implement in CubeIDE generated files */ }
void MX_USART1_UART_Init(void) { /* implement in CubeIDE generated files */ }
void MX_DCMI_Init(void) { /* implement in CubeIDE generated files */ }
void MX_DMA2D_Init(void) { /* implement in CubeIDE generated files */ }
