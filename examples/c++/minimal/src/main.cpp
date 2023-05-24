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
    scanbotsdk::initialize(LICENSE_KEY);
    std::cout << "License status: " << scanbotsdk::getLicenseStatus() << std::endl;

    // Create a barcode recognizer
    // The engine mode BALANCED is optimized for static image recognition.
    // In case of very high-res images with tiny barcodes, BEST mode is recommended.
    scanbotsdk::BarcodeRecognizer recognizer(SCANBOTSDK_BARCODE_ENGINE_MODE_BALANCED);

    // Read the image file
    cv::Mat image = cv::imread(argv[1]);

    // Recognize barcodes
    std::vector<scanbotsdk::BarCodeItem> result = recognizer.recognize(scanbotsdk::toScanbotSDKImage(image));

    // Print the results
    std::cout << "Found " << result.size() << " barcodes:" << std::endl;
    for (const auto& item : result){
        std::cout << "\t Text: " << item.text << std::endl;
    }

    return 0;
}