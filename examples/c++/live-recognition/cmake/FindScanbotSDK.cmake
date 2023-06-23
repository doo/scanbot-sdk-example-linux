include(ExternalProject)
include(FindPackageHandleStandardArgs)

if(NOT SCANBOTSDK_DIR)
message(FATAL_ERROR "SCANBOTSDK_DIR not set")
endif()

if (NOT EXISTS ${SCANBOTSDK_DIR})
  # TODO: take version information from use before final release
  set(URL "https://scanbotsdk-deployment.s3.amazonaws.com/ScanbotSDK-Linux/HZMdMdpCxVuaro3GE4G2gMmt/scanbot-sdk-0.0.1-linux-aarch64.tar.gz")
  execute_process(
    COMMAND wget -nc -q -O scanbotsdk.tgz ${URL}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
  execute_process(
    COMMAND tar -xf scanbotsdk.tgz
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
  execute_process(
    COMMAND mkdir -p ${SCANBOTSDK_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
  execute_process(
    COMMAND mv scanbotsdk ${SCANBOTSDK_DIR}/..
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
endif()

find_library(ScanbotSDK_LIBS
  NAMES libscanbotsdk.so
  HINTS ${SCANBOTSDK_DIR}/lib/
  )
  
find_path(ScanbotSDK_INCLUDE_DIRS
  NAMES ScanbotSDK.h
  HINTS ${SCANBOTSDK_DIR}/include/)

find_package_handle_standard_args(ScanbotSDK 
  REQUIRED_VARS ScanbotSDK_LIBS ScanbotSDK_INCLUDE_DIRS)
