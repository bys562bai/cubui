cmake_minimum_required(VERSION 3.5)
project(cubui)

find_package(GLOG REQUIRED)
find_package(Boost REQUIRED)
find_package(Fmt REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

find_path(GLOG_INCLUDE_DIRS "glog/logging.h")
find_path(FMT_INCLUDE_DIRS "fmt/format.h")

set(INC_LIST 
${CMAKE_SOURCE_DIR}/src/include
${VULKAN_INCLUDE_DIRS}
${SDL2_INCLUDE_DIRS}
${GLFW3_INCLUDE_DIRS}
${GLOG_INCLUDE_DIRS}
${FMT_INCLUDE_DIRS}
${Boost_INCLUDE_DIRS}
)

include(${CMAKE_SOURCE_DIR}/build-util/cmake-module/cubui_var.gen.cmake)

add_subdirectory(src)