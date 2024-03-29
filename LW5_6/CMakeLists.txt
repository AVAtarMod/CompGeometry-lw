cmake_minimum_required(VERSION 3.0)
project(LW5_6)

if (NOT "$ENV{CMAKE_DEFAULT_INCLUDE_PATH}" STREQUAL "")
    set(CMAKE_DEFAULT_INCLUDE_PATH "$ENV{CMAKE_DEFAULT_INCLUDE_PATH}" CACHE INTERNAL "Path for external projects and dependencies")
    file(TO_CMAKE_PATH ${CMAKE_DEFAULT_INCLUDE_PATH} CMAKE_DEFAULT_INCLUDE_PATH)
else()
   message(FATAL_ERROR "Cannot find exernal projects path. Set CMAKE_DEFAULT_INCLUDE_PATH environment variable")
   return()
endif()

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)  # optional, keep compiler extensions off

# Export no symbols by default (if the compiler supports it).
# This makes e.g. GCC's "visibility behavior" consistent with MSVC's. 
# On Windows/MSVC this is a noop.
cmake_policy(SET CMP0063 NEW)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Always include the source and build directories in the include path.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# create the executable
add_executable(main main.cpp part1.hpp part1.cpp part2.hpp part2.cpp)

# Copy resources to build dir
if (EXISTS ${PROJECT_SOURCE_DIR}/files)
   file(GLOB_RECURSE data files/*)
   foreach(file ${data})
      configure_file(${file} . COPYONLY)
   endforeach()
else()
   message(MESSAGE_WARNING "'files' directory not found")
endif()

add_subdirectory(${CMAKE_DEFAULT_INCLUDE_PATH}/lib_cppgeometry geometry_lib_bin)
target_include_directories(main PUBLIC ${CMAKE_DEFAULT_INCLUDE_PATH})
target_link_libraries(main shared)
