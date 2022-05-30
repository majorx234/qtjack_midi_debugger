cmake_minimum_required(VERSION 3.16)
project(qt_jack_midi_debugger
  DESCRIPTION "a small jack midi debugger"
  LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Threads)
find_package(qtjack)

include_directories(${PROJECT_INCLUDE_DIR})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(include)

list (APPEND RESOURCES 
        ${CMAKE_CURRENT_SOURCE_DIR}/qt_jack_midi_debugger.qrc
)

set(MESSAGE_HISTORY_SOURCES 
  src/messagehistory.cpp
  include/messagehistory.hpp
)

set(QTJACK_MAINWINDOW_SOURCES
  src/qtjack_mainwindow.cpp
  include/qtjack_mainwindow.hpp
)

set(QTJACK_MAIN_APP_SOURCES
  src/qtjack_main_app.cpp
)

QT5_WRAP_UI(MAIN_WINDOW_UISrcs 
            ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.ui)

QT5_WRAP_CPP(MESSAGE_HISTORY_MOCrcs
              ${MESSAGE_HISTORY_SOURCES}
)

QT5_WRAP_CPP(QTJACK_MAINWINDOW_MOCrcs
              ${QTJACK_MAINWINDOW_SOURCES}
)

QT5_WRAP_CPP(QTJACK_MAIN_MOCrcs
              ${QTJACK_MAIN_APP_SOURCES}
)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

add_library(message_history)
target_sources(message_history
PRIVATE
    ${MESSAGE_HISTORY_SOURCES}
    ${MESSAGE_HISTORY_MOCrcs}
)
target_link_libraries(message_history
  Qt5::Core
  Qt5::Widgets
)
qt5_add_resources (RCC_SOURCES ${RESOURCES})

add_library(qtjack_midi_debugger)
target_sources(qtjack_midi_debugger
PRIVATE
    ${QTJACK_MAINWINDOW_SOURCES}
    ${QTJACK_MAINWINDOW_MOCrcs}
    ${MAIN_WINDOW_UISrcs}
    ${RCC_SOURCES}
)
target_include_directories(qtjack_midi_debugger SYSTEM
PUBLIC 
  qtjack::qtjack
)
target_link_libraries(qtjack_midi_debugger
PRIVATE
  message_history
  qtjack::qtjack
  Qt5::Core
)

add_executable(qtjack_midi_debugger_app)
target_sources(qtjack_midi_debugger_app
PRIVATE
    ${QTJACK_MAIN_MOCrcs}
    ${QTJACK_MAIN_APP_SOURCES}
)
target_include_directories(qtjack_midi_debugger_app SYSTEM
PUBLIC 
  qtjack::qtjack
)
target_link_libraries(qtjack_midi_debugger_app
PRIVATE
  qtjack_midi_debugger
  Qt5::Core
  qtjack::qtjack
)