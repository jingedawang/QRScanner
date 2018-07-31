cmake_minimum_required(VERSION 3.5)
project(QRScanner)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV 3.0 QUIET)
if(NOT OpenCV_FOUND)
    find_package(OpenCV 2.4.3 REQUIRED)
    if(NOT OpenCV_FOUND)
        message(FATAL_ERROR "OpenCV > 2.4.3 not found.")
    endif()
endif()

include_directories(
        /usr/include
        ${OpenCV_INCLUDE_DIRS}
)

# Adjust the zbar library path when needed.
add_executable(QRScanner main.cpp)
target_link_libraries(QRScanner
        /usr/lib/x86_64-linux-gnu/libzbar.so
        ${OpenCV_LIBS})