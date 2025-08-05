cmake_minimum_required(VERSION 3.10)
project(scanbotsdk_c_example C)

set(CMAKE_C_STANDARD 99)

set(SCANBOTSDK_DIR "${CMAKE_CURRENT_BINARY_DIR}/scanbotsdk")
set(SCANBOTSDK_API_TOKEN "<SCANBOTSDK_API_KEY>") # Replace with your actual API key
set(SCANBOTSDK_VERSION "<SCANBOTSDK_VERSION>") # Replace with the desired version, e.g., "1.0.0"
include(FindScanbotSDK.cmake)

add_executable(scanbotsdk_c_example main.c)
target_link_libraries(scanbotsdk_c_example PRIVATE ${ScanbotSDK_LIBS})
target_include_directories(scanbotsdk_c_example PRIVATE ${ScanbotSDK_INCLUDE_DIRS})
