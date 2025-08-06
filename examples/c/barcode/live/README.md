# Barcode Scanner Live Mode Example

This example demonstrates how to use barcode scanner functionality in live mode.
It uses mock camera to simulate frames coming from a camera device.

You should provide it with the image you want to be used as a camera frame.

## Usage
```bash
<build_directory>/barcode/live/barcode_live <license_key> <image_path>
```

In order to run with TensorRT support, you can use the `--use-tensor-rt` flag:

```bash
<build_directory>/barcode/live/barcode_live <license_key> <image_path> --use-tensor-rt
```