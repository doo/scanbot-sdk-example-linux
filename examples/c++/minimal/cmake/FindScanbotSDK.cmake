include(ExternalProject)
include(FindPackageHandleStandardArgs)

if(NOT SCANBOTSDK_DIR)
message(FATAL_ERROR "SCANBOTSDK_DIR not set")
endif()

if (NOT EXISTS ${SCANBOTSDK_DIR})
  message(STATUS "Downloading ScanbotSDK ${SCANBOTSDK_VERSION} to ${SCANBOTSDK_DIR}")
  string(TOLOWER "${CMAKE_HOST_SYSTEM_NAME}-${CMAKE_HOST_SYSTEM_PROCESSOR}" PLATFORM_ID)
  set(URL "https://scanbotsdk-deployment.s3.amazonaws.com/ScanbotSDK-Linux/${SCANBOTSDK_API_TOKEN}/scanbotsdk-${SCANBOTSDK_VERSION}-${PLATFORM_ID}.tar.gz")
  execute_process(
    COMMAND wget ${URL}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
  execute_process(
    COMMAND tar -xf scanbotsdk-${SCANBOTSDK_VERSION}-${PLATFORM_ID}.tar.gz
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
