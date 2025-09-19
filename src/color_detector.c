#include "color_detector.h"
#include <stdint.h>

int color_detect_by_average_rgb565(uint8_t *frame, int w, int h)
{
    if (!frame || w<=0 || h<=0) return -1;
    uint64_t sum_r=0, sum_g=0, sum_b=0;
    int pixels = w * h;
    uint16_t *px = (uint16_t*)frame;
    for (int i=0;i<pixels;i++) {
        uint16_t p = px[i];
        int r5 = (p >> 11) & 0x1F;
        int g6 = (p >> 5) & 0x3F;
        int b5 = p & 0x1F;
        sum_r += r5;
        sum_g += g6;
        sum_b += b5;
    }
    // Average and scale to 0..255
    int ar = (int)((sum_r / pixels) * 255 / 31);
    int ag = (int)((sum_g / pixels) * 255 / 63);
    int ab = (int)((sum_b / pixels) * 255 / 31);

    // Simple threshold decision
    if (ar > ag + 30 && ar > ab + 30) return 0; // red
    if (ag > ar + 30 && ag > ab + 30) return 1; // green
    if (ab > ar + 30 && ab > ag + 30) return 2; // blue
    return -1;
}
