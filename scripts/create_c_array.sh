#!/bin/sh
# Usage: ./create_c_array.sh model.tflite model_data.h
if [ "$#" -ne 2 ]; then
  echo "Usage: $0 input.tflite output.h"
  exit 1
fi
xxd -i "$1" > "$2"
echo "Created C array: $2"
