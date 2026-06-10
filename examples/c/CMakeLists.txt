cmake_minimum_required(VERSION 3.20)
project(scanbotsdk_c_example C)
set(CMAKE_C_STANDARD 99)

# Find or download Scanbot SDK
if(NOT SCANBOTSDK_VERSION)
    set(SCANBOTSDK_VERSION "9.0.0")
endif()
list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(ScanbotSDK REQUIRED)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
add_executable(scanbotsdk_example ${SOURCE_FILES})
target_include_directories(scanbotsdk_example PRIVATE include)
target_link_libraries(scanbotsdk_example PRIVATE ScanbotSDK::ScanbotSDK)

if(WIN32)
    add_custom_command(TARGET scanbotsdk_example POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:ScanbotSDK::ScanbotSDK>"
            "$<TARGET_FILE_DIR:scanbotsdk_example>"
    )
endif()

