cmake_minimum_required(VERSION 3.0)
project(barcode_recognizer_minimal)

# Find or download Scanbot SDK
if(NOT SCANBOTSDK_VERSION)
    message(FATAL_ERROR "SCANBOTSDK_VERSION is not set.")
endif()

if(NOT SCANBOTSDK_API_TOKEN)
    message(FATAL_ERROR "SCANBOTSDK_API_TOKEN is not set.")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
set(SCANBOTSDK_DIR "${CMAKE_BINARY_DIR}/scanbotsdk")

find_package(ScanbotSDK REQUIRED)
include_directories(${ScanbotSDK_INCLUDE_DIRS})

# Link to OpenCV for the example app
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# example app
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(detect_barcodes ${SOURCES})
set_property(TARGET detect_barcodes PROPERTY CXX_STANDARD 17)
target_link_libraries( detect_barcodes ${OpenCV_LIBS} ${ScanbotSDK_LIBS})
