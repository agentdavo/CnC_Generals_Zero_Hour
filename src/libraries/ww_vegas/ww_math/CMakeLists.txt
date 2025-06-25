file(GLOB WWMATH_SRCS CONFIGURE_DEPENDS
    *.cpp
)

add_library(wwmath STATIC ${WWMATH_SRCS})

target_include_directories(wwmath PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src/libraries/ww_vegas/ww_math
    ${PROJECT_SOURCE_DIR}/src/libraries/ww_vegas/ww_save_load
)

target_link_libraries(wwmath PUBLIC wwlib wwsaveload)
