
![Scanbot SDK Logo](.images/ScanbotSDKLogo.png)

# Example App for the Scanbot Linux Barcode Scanner SDK (Beta)

This example app demonstrates integrating the Scanbot Linux Barcode Scanner SDK into C++ and Python applications.

## What is the Scanbot SDK?

The [Scanbot SDK](https://scanbot.io/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites) is a set of high-level APIs that integrates barcode, document scanning and data extraction functionalities into mobile apps, websites, and software server for environments, embedded systems, and edge devices. It operates entirely on-device, and no data is transmitted to our or third-party servers.

❗Please note that we currently only offer our **Barcode Scanning SDK for Linux through a closed Beta**. However, the SDK and a trial license are available on request. Please [contact us](mailto:beta@scanbot.io) to receive your trial license.

Refer to the respective README files in the examples' directories for more details.

## Barcode Scanner SDK

The Scanbot Linux Barcode Scanner SDK extracts barcode data from images. It returns this information as a simple list. The SDK is perfect for deployment on private clouds, drones, robots, and edge devices running Ubuntu, Raspberry Pi OS, or Debian.

### Technical requirements

* **Supported architectures**: ARM64 and x86_64.
* **Camera**: Camera with autofocus and at least 720p resolution.
* **GPU support**: Uses GPU acceleration on platforms with TensorRT, such as NVIDIA Jetson.

### Performance overview

Performance with a 1280x720 input image size with NEXT_GEN engine mode:
| Device                                            | Frame-rate |
|---------------------------------------------------|------------|
| Raspberry Pi 4                                    | ~8.5 FPS   |
| NVidia Jetson Orin Nano 8GB                       | ~40 FPS    |
| NVidia Jetson Orin Nano 8GB with GPU acceleration | ~85 FPS    |

### Supported barcodes

The Scanbot Barcode Scanner SDK supports all common 1D- or 2D barcode formats, including:

| Barcode type | Barcode symbologies                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 |
|:-------------|:--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| 1D Barcodes  | [EAN](https://scanbot.io/barcode-scanner-sdk/ean/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites), [UPC](https://scanbot.io/barcode-scanner-sdk/upc/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites), [Code 128](https://scanbot.io/barcode-scanner-sdk/code-128/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites), [GS1-128](https://scanbot.io/barcode-scanner-sdk/gs1-128/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites), [Code 39](https://scanbot.io/barcode-scanner-sdk/code-39/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites), [Codabar](https://scanbot.io/barcode-scanner-sdk/codabar/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites), [ITF](https://scanbot.io/barcode-scanner-sdk/itf-code/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites), Code 25, Code 32, Code 93, Code 11, MSI Plessey, Standard 2 of 5, IATA 2 of 5,  Databar (RSS), GS1 Composite                                                                                                                            |
| 2D Barcodes  | [QR Code](https://scanbot.io/glossary/qr-code/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites), Micro QR Code, [Aztec Code](https://scanbot.io/barcode-scanner-sdk/aztec-code/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites), [PDF417 Code](https://scanbot.io/barcode-scanner-sdk/pdf417/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites), [Data Matrix Code,](https://scanbot.io/barcode-scanner-sdk/data-matrix/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites) [GiroCode](https://scanbot.io/glossary/giro-code/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites), [NTIN Code](https://scanbot.io/glossary/gtin/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites), [PPN](https://scanbot.io/glossary/ppn/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites), [UDI](https://scanbot.io/glossary/udi/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites), [Royal Mail Mailmark](https://scanbot.io/barcode-scanner-sdk/royal-mail/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites), MaxiCode |

## Additional information

### Guides and Tutorials

Integrating the Scanbot Barcode Scanner SDK takes just a few minutes, and our step-by-step guides make the process even easier. 

💡Please check out our [Linux Tutorial ](https://scanbot.io/techblog/c-plus-plus-barcode-scanner-raspberry-pi-tutorial/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites)for an overview of how to get started.

Alternatively, visit our [developer blog](https://scanbot.io/techblog/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites) for a collection of in-depth tutorials, use cases, and best practices for the Scanbot SDK.

### Free integration support

Need help integrating our barcode scanning software into your Linux apps? We offer [free developer support](https://docs.scanbot.io/support/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites) via Slack, MS Teams, or email.

As a customer, you also get access to a dedicated support Slack or Microsoft Teams channel to talk directly to your Customer Success Manager and our engineers.

### Licensing and pricing

The barcode scanner example app will run one minute per session without a license. After that, all functionalities will stop working. To try the Barcode Scanner without the one-minute limit, you can request a free trial license [here](mailto:beta@scanbot.io).

Our pricing model is simple: Unlimited barcode scanning for a flat annual license fee, full support included. There are no tiers, usage charges, or extra fees. [Contact](https://scanbot.io/contact-sales/?utm_source=github.com&utm_medium=referral&utm_campaign=dev_sites) our team to receive your quote.

### Other supported platforms

Besides Linux, the Scanbot Barcode Scanner SDK is also available on:

* [Android (native)](https://github.com/doo/scanbot-barcode-scanner-sdk-example-android)
* [iOS (native)](https://github.com/doo/scanbot-barcode-scanner-sdk-example-ios)
* [JavaScript (web)](https://github.com/doo/scanbot-barcode-scanner-sdk-example-web)
* [React Native](https://github.com/doo/scanbot-barcode-scanner-sdk-example-react-native)
* [Flutter](https://github.com/doo/scanbot-barcode-scanner-sdk-example-flutter)
* [Capacitor & Ionic](https://github.com/doo/scanbot-barcode-scanner-sdk-example-capacitor-ionic)
* [Cordova & Ionic](https://github.com/doo/scanbot-barcode-scanner-sdk-example-cordova-ionic)
* [.NET MAUI](https://github.com/doo/scanbot-barcode-sdk-maui-example)
* [Xamarin & Xamarin.Forms](https://github.com/doo/scanbot-barcode-scanner-sdk-example-xamarin)
* [Compose Multiplatform / KMP](https://github.com/doo/scanbot-barcode-scanner-sdk-example-kmp)
* [UWP](https://github.com/doo/scanbot-barcode-scanner-sdk-example-windows) (Windows SDK)