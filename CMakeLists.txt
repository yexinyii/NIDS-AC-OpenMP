cmake_minimum_required(VERSION 3.14)
project(NIDS-AC-OpenMP)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

include(FetchContent)
find_package(nlohmann_json 3.11.2 QUIET)
if(NOT nlohmann_json_FOUND)
    FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        URL_HASH SHA256=d6c65aca6b1ed68e7a182f4757257b107ae403032760ed6ef121c9d55e81757d
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif()

cmake_policy(SET CMP0074 NEW)
find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

set(CORE_SOURCES
    src/ac_automaton.cpp
    src/parallel_ac.cpp
    src/system-core.cpp
    src/eigen_simple.cpp
    src/file_io.cpp
    src/rules_manager.cpp
    src/preprocess.cpp
)

set(CORE_HEADERS
    src/ac_automaton.h
    src/file_io.h
    src/rules_manager.h
    src/preprocess.h
)

add_library(core_lib STATIC ${CORE_SOURCES} ${CORE_HEADERS})

target_include_directories(core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(core_lib PUBLIC OpenMP::OpenMP_CXX nlohmann_json::nlohmann_json)

add_executable(nids_qt
    src/ui_qt/qt_mainwindow.ui
    src/ui_qt/md_message_box.cpp
    src/ui_qt/md_message_box.h
    src/ui_qt/md_ripple.cpp
    src/ui_qt/md_ripple.h
    src/ui_qt/mainwindow.cpp
)
target_link_libraries(nids_qt PRIVATE core_lib Qt6::Core Qt6::Widgets)
set_target_properties(nids_qt PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER "io.github.nids-ac-openmp"
    MACOSX_BUNDLE_BUNDLE_NAME "NIDS-AC-OpenMP"
)
