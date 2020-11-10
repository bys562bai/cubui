
if(${BUILDING_PLATFORM} STREQUAL WINDOWS)
add_executable(test WIN32 main.cpp)
else()
add_executable(test main.cpp)
endif()

target_include_directories(test PRIVATE ${CUBUI_DEPENDENT_INC})
target_link_libraries(test PRIVATE CUBUI_LIB)
