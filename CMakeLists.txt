cmake_minimum_required(VERSION 3.16)

project(GaussianBlur VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_executable(GaussianBlur
        main.cpp
        mainwindow.cpp mainwindow.h
        mainwindow.ui
        matrix.h
)

target_link_libraries(GaussianBlur PRIVATE Qt6::Widgets)
