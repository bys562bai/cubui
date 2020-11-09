add_executable(test main.cpp)

target_include_directories(test PRIVATE ${INC_LIST})
target_link_libraries(test CUBUI_LIB)
