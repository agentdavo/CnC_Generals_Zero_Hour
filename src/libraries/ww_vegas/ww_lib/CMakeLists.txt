file(GLOB WWLIB_SRCS CONFIGURE_DEPENDS *.cpp)
add_library(wwlib STATIC ${WWLIB_SRCS})

# Expose headers for dependent targets
# Include root headers and this library's headers
# Source path is provided for possible in-tree includes

# Provide include directories publicly so dependents see ww_lib headers

target_include_directories(wwlib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/libraries/ww_vegas/ww_lib
    ${PROJECT_SOURCE_DIR}/include/libraries/ww_vegas/ww_debug
    ${PROJECT_SOURCE_DIR}/src/libraries/ww_vegas/ww_lib
)

# wwlib does not directly depend on wwdebug
