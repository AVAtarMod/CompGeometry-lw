cmake_minimum_required(VERSION 3.0)
project(LW1)

if (NOT "$ENV{CMAKE_DEFAULT_INCLUDE_PATH}" STREQUAL "")
    set(CMAKE_DEFAULT_INCLUDE_PATH "$ENV{CMAKE_DEFAULT_INCLUDE_PATH}" CACHE INTERNAL "Path for external projects and dependencies")
    file(TO_CMAKE_PATH ${CMAKE_DEFAULT_INCLUDE_PATH} CMAKE_DEFAULT_INCLUDE_PATH)
else()
   message(FATAL_ERROR "Cannot find exernal projects path. Set CMAKE_DEFAULT_INCLUDE_PATH environment variable")
   return()
endif()

set (CMAKE_CXX_STANDARD 17)

# Export no symbols by default (if the compiler supports it).
# This makes e.g. GCC's "visibility behavior" consistent with MSVC's. 
# On Windows/MSVC this is a noop.
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Always include the source and build directories in the include path.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# create the executable
add_executable(main 
   main.cpp part2.cpp ${data}
 "part1.hpp" "part1.cpp")

# Copy resources to build dir
file(GLOB_RECURSE data files/*)
foreach(file ${data})
   configure_file(${file} . COPYONLY)
endforeach()

add_subdirectory(${CMAKE_DEFAULT_INCLUDE_PATH}/lib_cppgeometry geometry_lib_bin)
target_include_directories(main PUBLIC ${CMAKE_DEFAULT_INCLUDE_PATH})
target_link_libraries(main shared)
