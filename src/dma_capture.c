#include "dma_capture.h"
#include <string.h>
#include <stdio.h>

static uint8_t *g_buf0;
static uint8_t *g_buf1;
static uint32_t g_len;
static frame_cb_t g_cb;
static uint8_t g_next = 0;

/* Start DCMI capture in circular double-buffer mode (HAL + DMA). This is a simplified stub:
   - In your CubeIDE project, configure DCMI + DMA in double buffer mode and call HAL_DCMI_Start_DMA with DCMI_DMA_CONTINUOUS.
   - The DMA IRQ should call the user callback `g_cb(0 or 1)` depending on which buffer finished.
*/
void dma_capture_start(uint8_t *buf0, uint8_t *buf1, uint32_t len, frame_cb_t cb)
{
    g_buf0 = buf0;
    g_buf1 = buf1;
    g_len = len;
    g_cb = cb;
    g_next = 0;
    // Start DCMI+DMA capture here (platform-specific)
    printf("dma_capture_start: buffers registered (len=%lu)\n", (unsigned long)len);
}

/* Simple CPU fallback: convert RGB565 -> grayscale and shrink by nearest neighbor.
   Prefer DMA2D hardware path if available (faster and low CPU).
*/
int dma_downscale_rgb565_to_gray(uint8_t *src, int src_w, int src_h, uint8_t *dst, int dst_w, int dst_h)
{
    if (!src || !dst) return 0;
    for (int y = 0; y < dst_h; y++) {
        for (int x = 0; x < dst_w; x++) {
            int sx = (x * src_w) / dst_w;
            int sy = (y * src_h) / dst_h;
            uint16_t pixel = ((uint16_t*)src)[sy * src_w + sx];
            // RGB565 -> components
            int r = (pixel >> 11) & 0x1F;
            int g = (pixel >> 5) & 0x3F;
            int b = pixel & 0x1F;
            // scale to 0..255
            r = (r * 255) / 31;
            g = (g * 255) / 63;
            b = (b * 255) / 31;
            uint8_t gray = (uint8_t)((r*30 + g*59 + b*11) / 100);
            dst[y*dst_w + x] = gray;
        }
    }
    return 1;
}
