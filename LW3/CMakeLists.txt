cmake_minimum_required(VERSION 3.0)
project(LW3)

if (NOT "$ENV{CMAKE_DEFAULT_INCLUDE_PATH}" STREQUAL "")
    set(CMAKE_DEFAULT_INCLUDE_PATH "$ENV{CMAKE_DEFAULT_INCLUDE_PATH}" CACHE INTERNAL "Path for external projects and dependencies")
    file(TO_CMAKE_PATH ${CMAKE_DEFAULT_INCLUDE_PATH} CMAKE_DEFAULT_INCLUDE_PATH)
else()
   message(FATAL_ERROR "Cannot find external projects path. Set CMAKE_DEFAULT_INCLUDE_PATH environment variable")
   return()
endif()

file(TO_CMAKE_PATH ${PROJECT_SOURCE_DIR} LWLib)
set(LWLib "${LWLib}/../lib")

set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ version selection")  # or 11, 14, 17, 20
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # optional, ensure standard is supported
set(CMAKE_CXX_EXTENSIONS OFF)  # optional, keep compiler extensions off

file(GLOB library_sources ${CMAKE_DEFAULT_INCLUDE_PATH}/lib_cppgeometry/*.cpp)
file(GLOB wrapper_sources module/wrapper.cpp)

find_package(Python3 COMPONENTS Interpreter Development)
add_subdirectory(${CMAKE_DEFAULT_INCLUDE_PATH}/pybind11 pybind11_build)
add_subdirectory("${LWLib}/wrapper" wrapper)

# create the executable
add_executable(main ${LWLib}/wrapper/main.cpp)
 
target_include_directories(main PUBLIC ${CMAKE_DEFAULT_INCLUDE_PATH})
target_link_libraries(main shared)
