#ifndef DMA_CAPTURE_H
#define DMA_CAPTURE_H
#include <stdint.h>

typedef void (*frame_cb_t)(uint8_t which);

void dma_capture_start(uint8_t *buf0, uint8_t *buf1, uint32_t len, frame_cb_t cb);
int dma_downscale_rgb565_to_gray(uint8_t *src, int src_w, int src_h, uint8_t *dst, int dst_w, int dst_h);

#endif
