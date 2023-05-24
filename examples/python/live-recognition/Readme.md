This example demonstrates how to recognize barcodes in a live video feed.

----

## Pre-requisites

Follow the instructions in [README.md](../README.md) to set up your environment.

You will also need a USB webcam or a CSI camera to provide the live video feed.

---

## License Key

Please update the license key in `detect_barcodes.py` before running the example.

----
## Usage

The example will connect to the selected camera and begin looking for barcodes. It will print any detected barcodes to the terminal.

The example can be run as follows from the terminal:

* Using a CSI camera on an NVidia Jetson:

```bash
python detect_barcodes.py --input jetson_csi
```


* Using a CSI camera with libcamera on PiOS:
    
```bash
python detect_barcodes.py --input libcamera
```

* Using a camera compatible with video4linux, e.g. a USB webcam (available as /dev/video1 in this example; change the device number as per your system):

```bash
python detect_barcodes.py --input 1
```

### Optional flags

* ```--use-display```: Display a window that shows the found barcodes live. The found barcodes will be indicated with a green border overlaid on top.

* ```--use-tensorrt```: Enable TensorRT acceleration (Jetson only).
