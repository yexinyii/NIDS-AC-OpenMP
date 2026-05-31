cmake_minimum_required(VERSION 3.10)
project(NIDS-AC-OpenMP)

# C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt配置
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

# OpenMP
# CMP0074: 让 find_package 使用 <PackageName>_ROOT 变量（macOS Homebrew libomp 需要）
cmake_policy(SET CMP0074 NEW)
find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# Eigen 头文件目录
include_directories(third_party)

# 所有源文件
set(SOURCES
    src/main.cpp
    src/ac_automaton.cpp
    src/parallel_ac.cpp
    src/system-core.cpp
    src/eigen_simple.cpp
    src/file_io.cpp
    src/tool.cpp
    src/preprocess.cpp
    ui_qt/qt_ui.cpp
)

# Qt外的头文件
set(HEADERS
    src/ac_automaton.h
    src/file_io.h
    src/preprocess.h
)

# Qt UI文件
set(UI_FILES
    ui_qt/qt_mainwindow.ui
)

# 生成可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UI_FILES})

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX Qt5::Core Qt5::Widgets)
