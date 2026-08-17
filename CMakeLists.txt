cmake_minimum_required(VERSION 3.10)
project(NIDS-AC-OpenMP)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

cmake_policy(SET CMP0074 NEW)
find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

include_directories(third_party)

set(SOURCES
    src/main.cpp
    src/ac_automaton.cpp
    src/parallel_ac.cpp
    src/system-core.cpp
    src/eigen_simple.cpp
    src/file_io.cpp
    src/preprocess.cpp
    src/ui_qt/mainwindow.cpp
)

set(HEADERS
    src/ac_automaton.h
    src/file_io.h
    src/preprocess.h
)

set(UI_FILES
    src/ui_qt/qt_mainwindow.ui
)

add_library(core_lib STATIC
    src/main.cpp
    src/ac_automaton.cpp
    src/parallel_ac.cpp
    src/system-core.cpp
    src/eigen_simple.cpp
    src/file_io.cpp
    src/preprocess.cpp
)

target_include_directories(core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(nids_qt
    src/ui_qt/qt_mainwindow.ui
    src/ui_qt/mainwindow.cpp
)
target_link_libraries(nids_qt PRIVATE core_lib Qt6::Core Qt6::Widgets)