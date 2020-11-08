cmake_minimum_required(VERSION 3.5)
project(test)

find_package(GLOG REQUIRED)
find_package(Boost REQUIRED)
find_package(folly REQUIRED)
find_package(Fmt REQUIRED)

find_path(GLOG_INCLUDE_DIRS "glog/logging.h")
find_path(FMT_INCLUDE_DIRS "fmt/format.h")

set(INC_LIST 
${CMAKE_SOURCE_DIR}/../include
${GLOG_INCLUDE_DIRS}
${FMT_INCLUDE_DIRS}
${Boost_INCLUDE_DIRS}
)


add_executable(test main.cpp)

target_include_directories(test PRIVATE ${INC_LIST})
target_link_libraries(test glog::glog Boost::boost)
