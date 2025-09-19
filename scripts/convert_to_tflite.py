# Example script: Keras model -> int8 quantized TFLite
# Requires: tensorflow (>=2.4)
import argparse
import tensorflow as tf

parser = argparse.ArgumentParser()
parser.add_argument('--keras', required=True, help='Keras .h5 file input')
parser.add_argument('--tflite', required=True, help='Output tflite filename')
args = parser.parse_args()

# Load model
model = tf.keras.models.load_model(args.keras)

# Representative dataset generator — user should edit to point to representative images
def representative_data_gen():
    for _ in range(100):
        # Create a fake batch of 1 image with shape (height, width, channels)
        # Edit this to read real images for best accuracy
        yield [tf.random.uniform([1, 96, 96, 3], dtype=tf.float32)]

converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]
converter.representative_dataset = representative_data_gen
converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
converter.inference_input_type = tf.uint8
converter.inference_output_type = tf.uint8

tflite_model = converter.convert()
with open(args.tflite, 'wb') as f:
    f.write(tflite_model)
print('Saved quantized tflite to', args.tflite)
