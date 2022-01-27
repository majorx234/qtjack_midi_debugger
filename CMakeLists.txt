cmake_minimum_required(VERSION 3.16)
project(qt_jack_midi_debugger
  DESCRIPTION "a small jack midi debugger"
  LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)

find_package(Jack REQUIRED)
find_package( Qt5Widgets REQUIRED)
find_package( Qt5Core REQUIRED)


include_directories(${PROJECT_INCLUDE_DIR})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(include)
#include_directories(${JACK_INCLUDE_DIRS})
include_directories("/usr/include")

QT5_WRAP_CPP( WIDGET_MOC_SOURCES
              ${CMAKE_CURRENT_SOURCE_DIR}/src/widget.cpp
              ${CMAKE_CURRENT_SOURCE_DIR}/include/widget.h
	      )

QT5_WRAP_CPP(MAIN_MOCsrcs
   ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
   )

set(MAIN_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  )

add_executable(qt_jack_midi_debugger
  ${MAIN_SOURCES})

target_link_libraries(qt_jack_midi_debugger PRIVATE ${JACK_LIBRARIES})
