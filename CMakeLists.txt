cmake_minimum_required(VERSION 3.10)
project(NIDS-AC-OpenMP LANGUAGES CXX)

# Compile only these sources
set(SRC_FILES
    src/main.cpp
    src/file_io.cpp
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Put the produced binary in the project root (source dir)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${SRC_FILES})

# Optionally enable OpenMP if available
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()
