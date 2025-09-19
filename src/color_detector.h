#ifndef COLOR_DETECTOR_H
#define COLOR_DETECTOR_H
#include <stdint.h>

// Returns: -1 for none, 0 for red, 1 for green, 2 for blue (simple example)
int color_detect_by_average_rgb565(uint8_t *frame, int w, int h);

#endif
