cmake_minimum_required(VERSION 3.5)
project(cubui)

set(CMAKE_CXX_STANDARD 17)

find_package(GLOG REQUIRED)
find_package(Boost REQUIRED)
find_package(Fmt REQUIRED)
# find_package(SDL2 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

find_path(GLOG_INCLUDE_DIRS "glog/logging.h")
find_path(FMT_INCLUDE_DIRS "fmt/format.h")


set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/out/lib)



set(CUBUI_DEPENDENT_INC 
${CMAKE_SOURCE_DIR}/src/include
${VULKAN_INCLUDE_DIRS}
${GLFW3_INCLUDE_DIRS}
${GLOG_INCLUDE_DIRS}
${FMT_INCLUDE_DIRS}
${Boost_INCLUDE_DIRS}
)

set(CUBUI_DEPENDENT_LIB
Boost::boost 
glog::glog 
# SDL2::SDL2main SDL2::SDL2-static 
${GLFW3_LIBRARIES}
)


set(CUBUI_CMAKE_MODULE_PATH  ${CMAKE_SOURCE_DIR}/build-util/cmake-module)


execute_process(COMMAND python ${CMAKE_SOURCE_DIR}/build-util/code_maker.py)
include(${CUBUI_CMAKE_MODULE_PATH}/cubui_var.cmake)

add_subdirectory(src)