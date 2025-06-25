file(GLOB WW3D2_SRCS CONFIGURE_DEPENDS *.cpp)
add_library(ww3d2 STATIC ${WW3D2_SRCS})
target_include_directories(ww3d2 PUBLIC
    ${PROJECT_SOURCE_DIR}/src/tools/WW3D/pluglib
    ${PROJECT_SOURCE_DIR}/src/tools/ww_3d/pluglib
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/libraries/ww_vegas/ww_lib
    ${PROJECT_SOURCE_DIR}/src/libraries/ww_vegas/ww_3d2)

target_link_libraries(ww3d2 PUBLIC
    wwlib
    wwmath
    d3d8_to_gles
)
