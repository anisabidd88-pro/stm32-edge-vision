# STM32 Edge Vision — Example Project (TinyML + DMA optimized pipeline)

**Quick Note :**
This project is designed to provide you with a solid foundation for showcasing your expertise in **STM32 + TinyML + Image Pipeline (DMA, DMA2D, DCMI)**.
The project contains:
- Example C code for capturing an image using DCMI + DMA (double-buffering).
- A simple color detection module that works "out-of-the-box" (pure-C) so the project can work without TensorFlow Lite Micro.
- A demonstration file for linking TFLM (TinyML) and converting a model to a C file.
- Helper scripts (converting the model to tflite, creating a C array).
- Detailed instructions in English and Arabic for building, flashing, and performance optimization.

**Important (English):**
This is a ready-to-inspect, well-documented template intended for STM32H7-family MCUs (e.g. STM32H743) but it can be adapted for other lines (F7, F4 with adjustments).
The archive intentionally includes a **pure-C color detector** so you can run the firmware on-board immediately (no TFLM required). Full TinyML integration steps are documented and helper scripts are provided — you'll need TensorFlow installed on your PC to run them.

## What's inside
- `src/` : Example firmware sources (main, camera, DMA, color detector, TFLM wrapper).
- `scripts/` : Python script to convert Keras -> quantized TFLite and a shell helper to make a C array.
- `Makefile` : template build commands (adjust to your toolchain / CubeIDE).
- `README.md` : this file.

## Quick start (summary)
1. Open `src/` in STM32CubeIDE or compile with arm-none-eabi toolchain (edit include paths and HAL drivers).
2. Flash to STM32H7 board wired to an OV2640/OV5640 camera (DCMI) or similar.
3. The firmware boots, captures frames using DCMI+DMA double-buffering, runs a color detection algorithm and toggles an LED / prints a message over UART when detection occurs.
   - This provides an immediately runnable demo without TinyML.
4. To integrate TinyML (TensorFlow Lite for Microcontrollers):
   - Train a tiny model on your PC.
   - Use `scripts/convert_to_tflite.py` to quantize to int8.
   - Use `scripts/create_c_array.sh` (or xxd) to convert the `.tflite` into a C array and replace `src/model_dummy.h`.
   - Enable `USE_TFLM` in `tflm_inference.c` and follow README instructions to link TFLM and CMSIS-NN.

## Why this structure (expert-level features)
- **Double buffering with DCMI + DMA** — minimizes latency and maximizes throughput.
- **DMA2D-based preprocessing path** — offloads resizing / RGB->grayscale to hardware.
- **Fallback pure-C detector** for immediate testing and power profiling without the complexity of TFLM.
- **Scripts + instructions** for quantized model creation and producing a C model blob.
- **Placeholders and profiling hooks** to measure latency, RAM and power (explain how to measure with ST-LAB or external equipment).

## Hardware suggestions
- STM32H743 Nucleo or STM32H7 Discovery board (high-performance ML on edge).
- Camera: OV2640/OV5640 (DCMI + SCCB) or similar.
- Optional: ESP32 (for Wi-Fi/BLE relay), LoRa module for long-range alerts, small LCD for debugging overlay.

## License
MIT — see LICENSE file.

## Support
I included plenty of inline comments in the source. If you want I can:
- Replace the pure-C detector with a sample TFLM-inferred model blob that I prepare for you,
- Or produce a CubeIDE project (.ioc + project files) targeted to a specific board (STM32H7 / NUCLEO).

