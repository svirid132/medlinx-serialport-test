cmake_minimum_required(VERSION 3.14)

project(medlinx-serialport-test LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core SerialPort)

add_executable(medlinx-serialport-test
  main.cpp
)
target_link_libraries(medlinx-serialport-test Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::SerialPort)

install(TARGETS medlinx-serialport-test
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
