cmake_minimum_required(VERSION 3.10)
project(barcode_minimal C)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/example.png" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE scanbotsdk)
