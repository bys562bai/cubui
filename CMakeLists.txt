cmake_minimum_required(VERSION 3.5)
project(cubui)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    set(BUILDING_PLATFORM  WINDOWS)
endif()



find_package(spdlog REQUIRED)
find_package(Boost REQUIRED)
find_package(Fmt REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

find_path(FMT_INCLUDE_DIRS "fmt/format.h")


set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/out/lib)



set(CUBUI_DEPENDENT_INC 
${CMAKE_SOURCE_DIR}/src/include
${GLFW3_INCLUDE_DIRS}
${FMT_INCLUDE_DIRS}
${Boost_INCLUDE_DIRS}
)



set(CUBUI_DEPENDENT_LIB
Boost::boost 
spdlog::spdlog
Vulkan::Vulkan
glfw
)


set(CUBUI_CMAKE_MODULE_PATH  ${CMAKE_SOURCE_DIR}/build-util/cmake-module)


execute_process(COMMAND python ${CMAKE_SOURCE_DIR}/build-util/code_maker.py)
include(${CUBUI_CMAKE_MODULE_PATH}/cubui_var.cmake)

add_subdirectory(src)