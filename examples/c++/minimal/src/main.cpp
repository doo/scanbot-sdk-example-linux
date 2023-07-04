#include <ScanbotSDK.hpp>
#include <ScanbotSDKOpenCV.hpp>

#include <opencv2/imgcodecs.hpp>
#include <iostream>

const std::string LICENSE_KEY = "<Put license key here>";


/*
 * This is a minimal showcasing how to use ScanbotSDK for barcode recognition.
 * The executable is expected to be called with an image file as argument.
 * For example: 
 *                ./detect_barcodes my_image.jpg
 *
*/
int main(int argc, char** argv) {
    scanbotsdk::initialize({.licenseKey=LICENSE_KEY});
    std::cout << "License status: " << scanbotsdk::getLicenseStatus() << std::endl;

    scanbotsdk::BarcodeRecognizer::InitializationParams params;

    // The engine mode BALANCED is optimized for static image recognition.
    // In case of very high-res images with tiny barcodes, BEST mode is recommended.
    params.engineMode = SCANBOTSDK_BARCODE_ENGINE_MODE_BALANCED;

    // Uncomment the following line to use TensorRT for barcode recognition.
    // params.useTensorRT = true;

    // Create a barcode recognizer
    scanbotsdk::BarcodeRecognizer recognizer(params);

    // Read the image file
    cv::Mat image = cv::imread(argv[1]);

    // Recognize barcodes
    std::vector<scanbotsdk::BarCodeItem> result = recognizer.recognize(scanbotsdk::toImage(image));

    // Print the results
    std::cout << "Found " << result.size() << " barcodes:" << std::endl;
    for (const auto& item : result){
        std::cout << "\t Text: " << item.text << std::endl;
    }

    return 0;
}