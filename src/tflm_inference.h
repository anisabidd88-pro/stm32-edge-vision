#ifndef TFLM_INFERENCE_H
#define TFLM_INFERENCE_H
#include <stdint.h>

void tflm_init(void);
int tflm_run_inference(uint8_t *input_gray, int w, int h); // returns label index or -1

#endif
