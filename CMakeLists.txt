cmake_minimum_required(VERSION 3.10)
project(NIDS-AC-OpenMP LANGUAGES CXX)

# 所有源文件
set(SRC_FILES
    src/main.cpp
    src/file_io.cpp
    src/ac_automaton.cpp
    src/parallel_ac.cpp
    src/system_core.cpp
    src/eigen_simple.cpp
    src/tools.cpp
)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 输出可执行文件到项目根目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# ===================== 关键配置 =====================

include_directories(${CMAKE_SOURCE_DIR}/third_party/Eigen)

# 让 Eigen 自动开启 OpenMP 多核加速
add_definitions(-DEIGEN_USE_OPENMP)
# ====================================================

add_executable(${PROJECT_NAME} ${SRC_FILES})

# OpenMP 多线程加速
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()
