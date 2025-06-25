file(GLOB WWSAVELOAD_SRCS CONFIGURE_DEPENDS
    *.cpp
)

add_library(wwsaveload STATIC ${WWSAVELOAD_SRCS})

target_include_directories(wwsaveload PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/libraries/ww_vegas/ww_lib
    ${PROJECT_SOURCE_DIR}/include/libraries/ww_vegas/ww_debug
    ${PROJECT_SOURCE_DIR}/include/libraries/ww_vegas/ww_math
)

