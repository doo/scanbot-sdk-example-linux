include(FindPackageHandleStandardArgs)

if(TARGET ScanbotSDK::ScanbotSDK)
  return()
endif()

if(NOT DEFINED SCANBOTSDK_VERSION)
  message(FATAL_ERROR "SCANBOTSDK_VERSION is not set.")
endif()

if(NOT DEFINED SCANBOTSDK_DIR)
  set(SCANBOTSDK_DIR "${CMAKE_CURRENT_BINARY_DIR}")
endif()

if(NOT EXISTS "${SCANBOTSDK_DIR}/scanbotsdk")
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
      set(SCANBOTSDK_TARGET "windows-x86")
    else()
      set(SCANBOTSDK_TARGET "windows-x64")
    endif()
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if(CMAKE_SYSTEM_PROCESSOR MATCHES ".*arm.*" OR CMAKE_SYSTEM_PROCESSOR MATCHES ".*aarch64.*")
      set(SCANBOTSDK_TARGET "linux-aarch64")
    else()
      set(SCANBOTSDK_TARGET "linux-x86_64")
    endif()
  else()
    message(FATAL_ERROR "System ${CMAKE_SYSTEM_NAME} not supported")
  endif()

  set(_archive "${SCANBOTSDK_DIR}/scanbotsdk-${SCANBOTSDK_VERSION}-${SCANBOTSDK_TARGET}.tar.gz")
  set(_url "https://github.com/doo/scanbot-sdk-example-linux/releases/download/standalone-sdk%2Fv${SCANBOTSDK_VERSION}/scanbotsdk-${SCANBOTSDK_VERSION}-${SCANBOTSDK_TARGET}.tar.gz")

  file(MAKE_DIRECTORY "${SCANBOTSDK_DIR}")

  execute_process(
    COMMAND "${CMAKE_COMMAND}"
      "-DURL=${_url}"
      "-DDEST=${_archive}"
      -P "${CMAKE_CURRENT_LIST_DIR}/Download.cmake"
    RESULT_VARIABLE _rc
  )
  if(NOT _rc EQUAL 0)
    file(REMOVE "${_archive}")
    message(FATAL_ERROR "Failed to download Scanbot SDK from ${_url}")
  endif()

  execute_process(
    COMMAND "${CMAKE_COMMAND}" -E tar -xf "${_archive}"
    WORKING_DIRECTORY "${SCANBOTSDK_DIR}"
    RESULT_VARIABLE _rc
  )
  if(NOT _rc EQUAL 0)
    file(REMOVE "${_archive}")
    file(REMOVE_RECURSE "${SCANBOTSDK_DIR}/scanbotsdk")
    message(FATAL_ERROR "Failed to extract Scanbot SDK archive ${_archive}")
  endif()

  file(REMOVE "${_archive}")
endif()

find_library(ScanbotSDK_LIBS
  NAMES scanbotsdk
  PATHS "${SCANBOTSDK_DIR}/scanbotsdk/lib"
  NO_DEFAULT_PATH
)

find_path(ScanbotSDK_INCLUDE_DIRS
  NAMES ScanbotSDK.h
  PATHS "${SCANBOTSDK_DIR}/scanbotsdk/include"
  NO_DEFAULT_PATH
)

set(SCANBOTSDK_REQUIRED_VARS ScanbotSDK_LIBS ScanbotSDK_INCLUDE_DIRS)

if(WIN32)
  find_file(ScanbotSDK_DLL
    NAMES scanbotsdk.dll
    PATHS "${SCANBOTSDK_DIR}/scanbotsdk/lib"
    NO_DEFAULT_PATH
  )
  list(APPEND SCANBOTSDK_REQUIRED_VARS ScanbotSDK_DLL)
endif()

find_package_handle_standard_args(ScanbotSDK
  REQUIRED_VARS ${SCANBOTSDK_REQUIRED_VARS}
  VERSION_VAR SCANBOTSDK_VERSION
)

message(STATUS "Scanbot SDK third-party licenses are located at: ${SCANBOTSDK_DIR}/scanbotsdk/licenses/Libraries.txt")

add_library(ScanbotSDK::ScanbotSDK SHARED IMPORTED)
set_target_properties(ScanbotSDK::ScanbotSDK PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${ScanbotSDK_INCLUDE_DIRS}"
)

if(WIN32)
  set_target_properties(ScanbotSDK::ScanbotSDK PROPERTIES
    IMPORTED_LOCATION "${ScanbotSDK_DLL}"
    IMPORTED_IMPLIB "${ScanbotSDK_LIBS}"
  )
else()
  set_target_properties(ScanbotSDK::ScanbotSDK PROPERTIES
    IMPORTED_LOCATION "${ScanbotSDK_LIBS}"
  )
endif()
