file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dummy.c "")
add_library(miles6 STATIC ${CMAKE_CURRENT_BINARY_DIR}/dummy.c)
set_target_properties(miles6 PROPERTIES LINKER_LANGUAGE C)
