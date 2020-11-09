message(${CUBUI_SRC_FILE})



add_library(CUBUI_LIB STATIC ${CUBUI_SRC_FILE})

target_include_directories(CUBUI_LIB PUBLIC ${INC_LIST})
target_link_libraries(CUBUI_LIB glog::glog Boost::boost ${SDL2_LIBRARIES} ${GLFW3_LIBRARIES})

add_subdirectory(test)