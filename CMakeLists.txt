cmake_minimum_required(VERSION 3.16)
project(qt_jack_midi_debugger
  DESCRIPTION "a small jack midi debugger"
  LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)

find_package(Jack REQUIRED)
find_package( Qt5Widgets REQUIRED)
find_package( Qt5Core REQUIRED)
find_package (Threads)

include_directories(${PROJECT_INCLUDE_DIR})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(include)
#include_directories(${JACK_INCLUDE_DIRS})
include_directories("/usr/include")

set(MAIN_APP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/main_app.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/include/mainwindow.hpp
)

list (APPEND RESOURCES 
        ${CMAKE_CURRENT_SOURCE_DIR}/qt_jack_midi_debugger.qrc
)

set(MESSAGE_HISTORY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/messagehistory.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/include/messagehistory.hpp
)

set(JACK_MIDI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/jackmidi.cpp
                      ${CMAKE_CURRENT_SOURCE_DIR}/include/jackmidi.hpp)

QT5_WRAP_CPP(MAIN_MOCrcs
              ${MAIN_APP_SRC}
)

QT5_WRAP_UI(MAIN_WINDOW_UISrcs 
            ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.ui)

QT5_WRAP_CPP(MESSAGE_HISTORY_MOCrcs
              ${MESSAGE_HISTORY_SOURCES}
)

#QT5_WRAP_CPP( WIDGET_MOC_SOURCES
#              ${CMAKE_CURRENT_SOURCE_DIR}/src/widget.cpp
#              ${CMAKE_CURRENT_SOURCE_DIR}/include/widget.h
#	      )

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

set(MAIN_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  )

QT5_WRAP_CPP(MAIN_MOCsrcs
   ${MAIN_SOURCES}
   )

add_executable(jack_midi_debugger
  ${MAIN_SOURCES})

target_link_libraries(jack_midi_debugger PRIVATE ${JACK_LIBRARIES})

add_library(message_history
    ${MESSAGE_HISTORY_SOURCES}
    ${MESSAGE_HISTORY_MOCrcs}
)

add_library(jack_midi
	${JACK_MIDI_SOURCES}
)
target_link_libraries(jack_midi PRIVATE ${JACK_LIBRARIES})

qt5_add_resources (RCC_SOURCES ${RESOURCES})

add_executable(qt_jack_midi_debugger 
    ${MAIN_MOCrcs}   
    ${MAIN_APP_SRC}
    ${MAIN_WINDOW_UISrcs}
    ${RCC_SOURCES}
)

qt5_use_modules(qt_jack_midi_debugger Core Gui Widgets)
qt5_use_modules(message_history Gui Widgets)

target_link_libraries(qt_jack_midi_debugger
  message_history
	jack_midi
	Threads::Threads
)
