cmake_minimum_required(VERSION 3.16)

project(qtgllogger LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(qtgllogger SHARED
  ./src/qtgllogger_global.h
  ./src/qtgllogger_global.cpp
  ./src/qtgllogger.cpp
  ./src/qtgllogger.h
  ./src/qtgllogger_obj.cpp
  ./src/qtgllogger_obj.h
)

target_link_libraries(qtgllogger PUBLIC Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(qtgllogger PRIVATE QTGLLOGGER_LIBRARY)

include("./output/makeme.cmake")


# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS AndroidExtras)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS AndroidExtras)
# target_link_libraries(${PROJECT_NAME} PUBLIC Qt${QT_VERSION_MAJOR}::AndroidExtras)


