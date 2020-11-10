add_executable(test main.cpp)

target_include_directories(test PRIVATE ${CUBUI_DEPENDENT_INC})
target_link_libraries(test PRIVATE CUBUI_LIB)
