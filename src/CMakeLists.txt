source_group("Header Files" FILES ${CUBUI_HEARDER_FILE}) 
add_library(CUBUI_LIB STATIC ${CUBUI_SRC_FILE} ${CUBUI_HEARDER_FILE})

target_include_directories(CUBUI_LIB PUBLIC ${CUBUI_DEPENDENT_INC})

target_link_libraries(CUBUI_LIB PUBLIC ${CUBUI_DEPENDENT_LIB})

# TODO if enable
add_subdirectory(test)

# TODO if enable
add_subdirectory(demo)