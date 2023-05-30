cmake_minimum_required(VERSION 3.0)
project(LW8-py)

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

add_subdirectory("${LWLib}/wrapper" wrapper)

add_custom_command(OUTPUT python_dependencies
    COMMAND pip install  -r requirements.txt
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "${PROJECT_NAME} Installing python dependencies..."
)
add_custom_target(${PROJECT_NAME} ALL DEPENDS python_dependencies lib_cppgeometry_wrapper)


# create the executable
add_executable(main ${LWLib}/wrapper/main.cpp)
 
target_include_directories(main PUBLIC ${CMAKE_DEFAULT_INCLUDE_PATH})
target_link_libraries(main shared)
