cmake_minimum_required(VERSION 3.10)
project(scanbotsdk_c_example C)
set(CMAKE_C_STANDARD 99)

# Find or download Scanbot SDK
if(NOT SCANBOTSDK_VERSION)
    message(FATAL_ERROR "SCANBOTSDK_VERSION is not set.")
endif()
if(NOT SCANBOTSDK_API_TOKEN)
    message(FATAL_ERROR "SCANBOTSDK_API_TOKEN is not set.")
endif()
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
set(SCANBOTSDK_DIR "${CMAKE_CURRENT_BINARY_DIR}/scanbotsdk")
find_package(ScanbotSDK REQUIRED)

add_subdirectory(barcode)
