cmake_minimum_required(VERSION 3.14)
project(um-desktop)

# set(wxWidgets_ROOT_DIR D:\\Projetos\\MinGW_Projects\\wxWidgets-3.0.4)
# set(wxWidgets_LIB_DIR D:\\Projetos\\MinGW_Projects\\wxWidgets-3.0.4\\lib\\gcc_lib)
find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES src/*.cpp src/**/*.cpp)

add_executable(um-desktop ${SOURCES})
target_precompile_headers(um-desktop PRIVATE src/pch.h)
target_link_libraries(um-desktop ${wxWidgets_LIBRARIES})
