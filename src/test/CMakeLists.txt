function(cubui_add_test exec_name)
    if(${BUILDING_PLATFORM} STREQUAL WINDOWS)
    add_executable(${exec_name} WIN32 ${ARGN})
    else()
    add_executable(${exec_name} ${ARGN})
    endif()
    target_include_directories(${exec_name} PRIVATE ${CUBUI_DEPENDENT_INC})
    target_link_libraries(${exec_name} PRIVATE CUBUI_LIB)
endfunction()

function(cubui_add_test_console exec_name)
    add_executable(${exec_name} ${ARGN})
    target_include_directories(${exec_name} PRIVATE ${CUBUI_DEPENDENT_INC})
    target_link_libraries(${exec_name} PRIVATE CUBUI_LIB)
endfunction()

cubui_add_test(test test.cpp)
cubui_add_test_console(testarrayref testarrayref.cpp)


