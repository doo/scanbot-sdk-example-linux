# Download.cmake
if(NOT URL OR NOT DEST)
    message(FATAL_ERROR "Both URL and DEST variables must be defined.")
endif()

file(DOWNLOAD "${URL}" "${DEST}"
     STATUS status
     TLS_VERIFY ON
)

list(GET status 0 status_code)
list(GET status 1 status_string)

if(NOT status_code EQUAL 0)
    file(REMOVE "${DEST}")
    message(FATAL_ERROR "Download failed (${status_code}): ${status_string}")
endif()
