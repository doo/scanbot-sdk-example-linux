<p align="left">
  <img src=".images/ScanbotSDKLogo.png#gh-light-mode-only" width="15%" />
</p>
<p align="left">
  <img src=".images/ScanbotSDKLogo_darkmode.png#gh-dark-mode-only" width="15%" />
</p>

# Example App for the Scanbot Linux SDK

This example app demonstrates integrating the Scanbot Linux SDK into Python, Java, Node.js, and C applications.

## What is the Scanbot SDK?

The [Scanbot SDK](https://scanbot.io/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites) is a set of high-level APIs for implementing **barcode scanning**, **document scanning**, and **data extraction** functionalities into mobile and web apps. The Linux SDK extends the range of platforms to **server environments**, **embedded systems**, and **edge devices**. It uses on-device processing without sending any data to third-party servers.

The Scanbot Linux SDK is **a pure C library** and comes with wrappers for **Python**, **Java**, and **Node.js**, making it compatible with a wide range of development environments. On devices with **TensorRT support**, like the NVIDIA Jetson, GPU acceleration increases the barcode scanning speed up to 3x.

💡 For more details about the Scanbot SDK, check out the [documentation](https://docs.scanbot.io/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites).

## Technical requirements

- **Supported platforms:**

  - **Linux systems with glibc ≥ 2.27** (e.g., Ubuntu 18.04 / 20.04 / 22.04 LTS, Debian 11 / 12, Raspberry Pi OS 64-bit)

  - **NVIDIA Jetson devices**

* **Supported architectures**:
  * ARM64 and x86_64
* **Camera**:
  * Camera with autofocus and at least 720p resolution
* **GPU support**:
  * Uses GPU acceleration on platforms with TensorRT, such as NVIDIA Jetson

## Performance overview

Performance with a 1280x720 input image size and using the `NEXT_GEN` engine mode:
| Device                                            | Framerate |
| ------------------------------------------------- | --------- |
| Raspberry Pi 4                                    | ~8.5 FPS  |
| NVidia Jetson Orin Nano 8GB                       | ~40 FPS   |
| NVidia Jetson Orin Nano 8GB with GPU acceleration | ~85 FPS   |

## Feature overview

- [Barcode scanning](https://docs.scanbot.io/linux/barcode-scanner-sdk/introduction/)
- [Document scanning](https://docs.scanbot.io/linux/document-scanner-sdk/introduction/)
- [Document quality analysis](https://docs.scanbot.io/linux/document-scanner-sdk/08-document-quality-analyzer/introduction/)
- [OCR (optical character recognition)](https://docs.scanbot.io/linux/data-capture-modules/ocr/introduction/)
- [Text pattern scanning](https://docs.scanbot.io/linux/data-capture-modules/text-pattern-scanner/introduction/)
- [MRZ scanning](https://docs.scanbot.io/linux/data-capture-modules/mrz-scanner/introduction/)
- [Document data extraction](https://docs.scanbot.io/linux/data-capture-modules/document-data-extractor/introduction/)
- [VIN (vehicle identification number) scanning](https://docs.scanbot.io/linux/data-capture-modules/vin-scanner/introduction/)
- [Credit card scanning](https://docs.scanbot.io/linux/data-capture-modules/credit-card-scanner/introduction/)
- [Check (MICR) scanning](https://docs.scanbot.io/linux/data-capture-modules/check-scanner/introduction/)
- [Medical certificate scanning](https://docs.scanbot.io/linux/data-capture-modules/medical-certificate-scanner/introduction/)

## Licensing and pricing

A trial license key is required for evaluation or testing. To get a free trial license, please contact us at csm@scanbot.io.

Our pricing model is simple: Unlimited scanning for a flat annual license fee, full support included. There are no tiers, usage charges, or extra fees.

[Contact our team](https://scanbot.io/contact-sales/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites) to receive your quote.

## Other supported platforms

The Scanbot SDK is also available on Android, iOS, and most common cross-platform environments, such as React Native, Flutter, and .NET MAUI:

- [Android](https://github.com/doo/scanbot-sdk-example-android) (native)
- [iOS](https://github.com/doo/scanbot-sdk-example-ios) (native)
- [Web](https://github.com/doo/scanbot-sdk-example-web)
- [React Native](https://github.com/doo/scanbot-sdk-example-react-native)
- [Flutter](https://github.com/doo/scanbot-sdk-example-flutter)
- [Capacitor & Ionic (Angular)](https://github.com/doo/scanbot-sdk-example-capacitor-ionic)
- [Capacitor & Ionic (React)](https://github.com/doo/scanbot-sdk-example-ionic-react)
- [Capacitor & Ionic (Vue.js)](https://github.com/doo/scanbot-sdk-example-ionic-vuejs)
- [.NET MAUI](https://github.com/doo/scanbot-sdk-maui-example)
- [Xamarin](https://github.com/doo/scanbot-sdk-example-xamarin) & [Xamarin.Forms](https://github.com/doo/scanbot-sdk-example-xamarin-forms)
- [Cordova & Ionic](https://github.com/doo/scanbot-sdk-example-ionic)



- Barcode scanning only:
  - [Compose Multiplatform / KMP](https://github.com/doo/scanbot-barcode-scanner-sdk-example-kmp)
  - [UWP](https://github.com/doo/scanbot-barcode-scanner-sdk-example-windows) (Windows)