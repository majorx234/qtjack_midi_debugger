cmake_minimum_required(VERSION 3.16)
project(qt_jack_midi_debugger
  DESCRIPTION "a small jack midi debugger"
  VERSION 0.9.0
  LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(Threads)
find_package(qtjack)

list (APPEND RESOURCES 
        ${CMAKE_CURRENT_SOURCE_DIR}/qt_jack_midi_debugger.qrc
)

set(MESSAGE_HISTORY_HEADERS 
  include/messagehistory.hpp
)
set(MESSAGE_HISTORY_SOURCES 
  src/messagehistory.cpp
)


set(QTJACK_MAINWINDOW_HEADERS
  include/qtjack_mainwindow.hpp
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
              ${MESSAGE_HISTORY_HEADERS}
)

QT5_WRAP_CPP(QTJACK_MAINWINDOW_MOCrcs
              ${QTJACK_MAINWINDOW_HEADERS}
)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

add_library(message_history)
target_sources(message_history
PRIVATE
    ${MESSAGE_HISTORY_SOURCES}
    ${MESSAGE_HISTORY_HEADERS}
    ${MESSAGE_HISTORY_MOCrcs}
)
target_include_directories(message_history SYSTEM
PRIVATE 
  include
  ${CMAKE_CURRENT_BINARY_DIR}
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
    ${QTJACK_MAINWINDOW_HEADERS}
    ${QTJACK_MAINWINDOW_MOCrcs}
    ${MAIN_WINDOW_UISrcs}
    ${RCC_SOURCES}
)
target_include_directories(qtjack_midi_debugger SYSTEM
PRIVATE
  include
  ${CMAKE_CURRENT_BINARY_DIR} 
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
    ${QTJACK_MAIN_APP_SOURCES}
)
target_include_directories(qtjack_midi_debugger_app SYSTEM
PRIVATE
  include
  ${CMAKE_CURRENT_BINARY_DIR}   
)
target_link_libraries(qtjack_midi_debugger_app
PRIVATE
  qtjack_midi_debugger
  Qt5::Core
  Qt5::Widgets
  qtjack::qtjack
)