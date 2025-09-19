// model_dummy.h - placeholder model blob
// To create a real model blob:
//   xxd -i model.tflite > model_data.h
//
#ifndef MODEL_DUMMY_H
#define MODEL_DUMMY_H
const unsigned char model_data[] = { 0x00, 0x01, 0x02, 0x03 };
const unsigned int model_data_len = 4;
#endif
