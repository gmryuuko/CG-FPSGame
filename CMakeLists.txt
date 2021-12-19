cmake_minimum_required(VERSION 3.20)

project(fps)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "/utf-8 ${CMAKE_CXX_FLAGS}")

# include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# library directories
link_directories(${PROJECT_SOURCE_DIR}/lib)

# source directories
aux_source_directory(${PROJECT_SOURCE_DIR}/src SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/include/imgui-1.85 IMGUI_SRC)


add_executable(fps ${SOURCE} ${IMGUI_SRC})
target_link_libraries(fps glfw3.lib)