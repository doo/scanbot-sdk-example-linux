include(ExternalProject)
include(FindPackageHandleStandardArgs)

if(NOT SCANBOTSDK_DIR)
message(FATAL_ERROR "SCANBOTSDK_DIR not set")
endif()

if (NOT EXISTS ${SCANBOTSDK_DIR})

  find_program(WGET_PATH wget REQUIRED)

  message(STATUS "Downloading ScanbotSDK ${SCANBOTSDK_VERSION} to ${SCANBOTSDK_DIR}")

  if ("${CMAKE_HOST_SYSTEM_PROCESSOR}" MATCHES ".*arm.*" OR "${CMAKE_HOST_SYSTEM_PROCESSOR}" MATCHES ".*aarch64.*")
    set(SCANBOTSDK_ARCHITECTURE "aarch64")
  else ()
    set(SCANBOTSDK_ARCHITECTURE "x86_64")
  endif ()
  string(TOLOWER "${CMAKE_HOST_SYSTEM_NAME}-${SCANBOTSDK_ARCHITECTURE}" PLATFORM_ID)
  set(URL "https://github.com/doo/scanbot-sdk-example-linux/releases/download/standalone-sdk%2Fv${SCANBOTSDK_VERSION}/scanbotsdk-${SCANBOTSDK_VERSION}-linux-${SCANBOTSDK_ARCHITECTURE}.tar.gz")
  execute_process(
    COMMAND ${WGET_PATH} "${URL}"
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

add_library(scanbotsdk SHARED IMPORTED)
set_target_properties(scanbotsdk PROPERTIES
        IMPORTED_LOCATION "${ScanbotSDK_LIBS}"
        INTERFACE_INCLUDE_DIRECTORIES "${ScanbotSDK_INCLUDE_DIRS}"
)