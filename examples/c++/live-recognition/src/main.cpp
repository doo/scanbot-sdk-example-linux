#include <ScanbotSDK.h>
#include <ScanbotSDK.hpp>
#include <ScanbotSDKOpenCV.hpp>

#include <signal.h>
#include <iostream>
#include <fstream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


const std::string LICENSE_KEY = "<Put license key here>";

const auto FONT = cv::FONT_HERSHEY_PLAIN;
const auto GREEN = cv::Scalar(0, 255, 0);
const auto CYAN = cv::Scalar(255, 255, 0);
const auto BLACK = cv::Scalar(0, 0, 0);
const int DEVICE_CAPTURE_WIDTH=1280;
const int DEVICE_CAPTURE_HEIGHT=720;
const int DEVICE_CAPTURE_FRAMERATE=30;
const int DISPLAY_HEIGHT=640;

void print_help(){
    std::cout << "Usage: detect_barcodes [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << " -h, --help" << std::endl;
    std::cout << "     Print this usage information." << std::endl;
    std::cout << " -i, --input <a number|jetson_csi|jetson_usb|libcamera>" << std::endl;
    std::cout << "     Specify the device name for live recognition:" << std::endl;
    std::cout << "     - a number - 0, 1, etc.: connect to /dev/video0, /dev/video1, etc., using V4L2" << std::endl;
    std::cout << "     - jetson_csi - NVidia Jetson: connect to the CSI camera using nvarguscamerasrc" << std::endl;
    std::cout << "     - libcamera - Raspberry Pi (bullseye): connect to the CSI camera using libcamerasrc." << std::endl;
    std::cout << "                   If your camera supports autofocus, run libcamera-hello once" << std::endl;
    std::cout << "                   to trigger focusing, then run the example." << std::endl;
    std::cout << " --use-display" << std::endl;
    std::cout << "     Display live results in a window" << std::endl;
    std::cout << " --use-tensorrt" << std::endl;
    std::cout << "     Use TensorRT backend" << std::endl;
}


struct InputArgs{
    std::string device = "";
    bool use_display = false;
    bool use_tensorrt = false;
};

InputArgs parse_args(int argc, char** argv){
    InputArgs retval;
    if (argc > 0){
        for (int i = 0; i <argc; i++){
            std::string arg = argv[i];
            if ((arg == "-h") || (arg == "--help")){
                print_help();
                exit(0);
            }
            else if ((arg == "-i") || (arg == "--input")){
                if (i+1 < argc){
                    retval.device = argv[i+1];
                }
                else{
                    std::cout << "Missing input source" << std::endl;
                    print_help();
                    exit(1);
                }
            }
            else if ((arg == "--use-display")){
                retval.use_display = true;
            }
            else if ((arg == "--use-tensorrt")){
                retval.use_tensorrt = true;
            }
        }
    }

    if (retval.device.empty()) {
        std::cout << "Please specify a device id or an image file for recognition" << std::endl;
        print_help();
        exit(1);
    }

    return retval;
}


std::string gstreamer_pipeline(const std::string& device) {
    if (device == "jetson_csi") { 
    return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + 
            std::to_string(DEVICE_CAPTURE_WIDTH) + ", height=(int)" + std::to_string(DEVICE_CAPTURE_HEIGHT) + 
            ",  framerate=(fraction)" + std::to_string(DEVICE_CAPTURE_FRAMERATE) + "/1" +
           " ! nvvidconv flip-method=0 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink drop=1";
    }
    else if(device == "libcamera") {
        return " libcamerasrc ! video/x-raw, format=BGR, "
            " width=(int)" + std::to_string(DEVICE_CAPTURE_WIDTH) + ","
            " height=(int)" + std::to_string(DEVICE_CAPTURE_HEIGHT) + ","
            " framerate=(fraction)" + std::to_string(DEVICE_CAPTURE_FRAMERATE) +"/1 "
            " ! appsink";
    }
    else{
        std::cout << "Invalid device type: " << device << std::endl;
        print_help();
        exit(1);
    }
}

cv::Mat parseBarcodes(std::vector<scanbotsdk::BarCodeItem>& barcodes, cv::Mat& image){
    if( barcodes.size() > 0){
       
        std::cout << "Barcode(s): " << std::endl;
        for (auto& barcode : barcodes) {
            auto text = barcode.text;
            text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
            std::cout << "\t ==> " << text << std::endl;
            cv::Point2i points[4] = {
                cv::Point2i(barcode.quad[0], barcode.quad[1]),
                cv::Point2i(barcode.quad[2], barcode.quad[3]),
                cv::Point2i(barcode.quad[4], barcode.quad[5]),
                cv::Point2i(barcode.quad[6], barcode.quad[7])
            };

            cv::line(image, points[0], points[1], GREEN, 2);
            cv::line(image, points[1], points[2], GREEN, 2);
            cv::line(image, points[2], points[3], GREEN, 2);
            cv::line(image, points[3], points[0], GREEN, 2);
        }

    }
    return image;
}


bool exit_flag = false;
void exit_program(int s){
    exit_flag = true;
}


int recognizeLive(const InputArgs& args ){
    
    // Let's add a signal handler to gracefully exit the program
    // on receiving a Ctrl + C and not hang the camera
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = exit_program;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    scanbotsdk::BarcodeRecognizer::InitializationParams params;
    params.engineMode = SCANBOTSDK_BARCODE_ENGINE_MODE_FAST;
    if(args.use_tensorrt){
        params.useTensorRT = true;
    }
    // Initialize the barcode recognizer with the appropriate engine mode
    auto recognizer = scanbotsdk::BarcodeRecognizer(params);
    
    cv::VideoCapture cap;
    try {
        int deviceId = std::stoi(args.device);
        cap.open(deviceId, cv::CAP_V4L2);
        cap.set(cv::CAP_PROP_FRAME_WIDTH, DEVICE_CAPTURE_WIDTH);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, DEVICE_CAPTURE_HEIGHT);
        cap.set(cv::CAP_PROP_FPS, DEVICE_CAPTURE_FRAMERATE);
    } catch (std::invalid_argument) {
        std::string pipeline = gstreamer_pipeline(args.device);
        cap.open(pipeline, cv::CAP_GSTREAMER);
    }

    if (!cap.isOpened()) {
        std::cout << "Failed to open camera. Aborting recognition." << std::endl;
        return (-1);
    }


    try{
        if (args.use_display) {
            cv::namedWindow("BarcodeRecognizer", cv::WINDOW_AUTOSIZE);
        }

        while(!exit_flag) {
            cv::Mat image;
            cap >> image;
            if (image.empty()) {
                std::cout << "Failed to capture image" << std::endl;
                break;
            }

            // Recognize barcodes
            auto barcodes = recognizer.recognize(scanbotsdk::toImage(image));

            // Print the results
            parseBarcodes(barcodes, image);

            if(!args.use_display)
                continue;
	
	        // Visualize the results
            auto width = image.cols * DISPLAY_HEIGHT / image.rows;
            cv::resize(image, image, cv::Size(width, DISPLAY_HEIGHT));
            cv::rectangle(image, cv::Point2i(0, DISPLAY_HEIGHT-30), cv::Point2i(width, DISPLAY_HEIGHT), BLACK, -1);

            cv::imshow("BarcodeRecognizer", image);  
            cv::waitKey(1);

            if (cv::getWindowProperty("BarcodeRecognizer", cv::WND_PROP_AUTOSIZE) == -1)
                exit_flag = true;
        }
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
        cap.release();
        return 1;
    }

    std::cout << "Stopping recognition..." << std::endl;
    cap.release();
    return 0;
}

int main(int argc, char** argv) {
    
    scanbotsdk::initialize({.licenseKey=LICENSE_KEY});
    std::cout << "License status: " << scanbotsdk::getLicenseStatus() << std::endl;

    auto args = parse_args(argc, argv);
    recognizeLive(args);
}
