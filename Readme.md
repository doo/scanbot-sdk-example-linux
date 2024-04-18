# Scanbot SDK for Linux

These examples show how to integrate the [Scanbot SDK for Linux](https://scanbot.io) into C++ and Python applications.

**The Linux SDK is currently in closed beta.** The SDK and a trial license are available on request: [Get in contact](https://scanbot.io/developer/linux-barcode-scanner/)

Refer to the respective README files in the examples directory for more details.

## Barcode recognition
The Scanbot SDK allows for the recognition of various barcode types in images and video streams. The SDK is available for both the arm64 and x86_64 architectures. Furthermore, it supports GPU acceleration on NVidia Jetson devices.

Performance with a 1280x720 input image size in FAST mode:
| Device                                       | Frame-rate |
|----------------------------------------------|------------|
| Raspberry Pi 4                               | ~4 FPS     |
| NVidia Jetson Nano 2GB                       | ~5 FPS     |
| NVidia Jetson Nano 2GB with GPU acceleration | ~15 FPS    |

## License

You will need a valid trial or commercial license key to try these examples out. 
[Contact us](https://scanbot.io/developer/linux-barcode-scanner/) to obtain one.
