cmake_minimum_required(VERSION 3.0)
project(lib_cppgeometry_wrapper)

if (NOT "$ENV{CMAKE_DEFAULT_INCLUDE_PATH}" STREQUAL "")
    set(CMAKE_DEFAULT_INCLUDE_PATH "$ENV{CMAKE_DEFAULT_INCLUDE_PATH}" CACHE INTERNAL "Path for external projects and dependencies")
else()
   message(FATAL_ERROR "Cannot find exernal projects path. Set CMAKE_DEFAULT_INCLUDE_PATH environment variable")
   return()
endif()

set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ version selection")  # or 11, 14, 17, 20
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # optional, ensure standard is supported
set(CMAKE_CXX_EXTENSIONS OFF)  # optional, keep compiler extensions off

file(GLOB library_sources ${CMAKE_DEFAULT_INCLUDE_PATH}/lib_cppgeometry/*.cpp)
file(GLOB wrapper_sources module/*.cpp)

find_package(Python3 COMPONENTS Interpreter Development)

add_subdirectory(${CMAKE_DEFAULT_INCLUDE_PATH}/pybind11 pybind11_build)

pybind11_add_module(${PROJECT_NAME} ${library_sources} ${wrapper_sources})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND pip install --force-reinstall .
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Installing wrapper with pip..."
)