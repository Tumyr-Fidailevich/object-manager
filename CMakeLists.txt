cmake_minimum_required(VERSION 3.5)

project(object-manager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(SOURCES src/mainwindow.cpp src/frame.cpp src/mainwindow.ui)
set(HEADERS include/mainwindow.h include/frame.h )


add_executable(object-manager src/main.cpp ${SOURCES} ${HEADERS}
    resources/resources.qrc)
target_link_libraries(object-manager PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_include_directories(object-manager PRIVATE include)
