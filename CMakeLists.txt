cmake_minimum_required(VERSION 3.20)

project(fps)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# set(CMAKE_CXX_FLAGS "/std:c++17 ${CMAKE_CXX_FLAGS}")

# include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# library directories
link_directories(${PROJECT_SOURCE_DIR}/lib)

# source directories
aux_source_directory(${PROJECT_SOURCE_DIR}/src SOURCE)


add_executable(fps ${SOURCE})
target_link_libraries(fps glfw3.lib)