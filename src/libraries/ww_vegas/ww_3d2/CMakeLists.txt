file(GLOB WW3D2_SRCS CONFIGURE_DEPENDS *.cpp)
add_library(ww3d2 STATIC ${WW3D2_SRCS})
target_include_directories(ww3d2 PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src/Libraries/WWVegas/WW3D2
    ${PROJECT_SOURCE_DIR}/src/tools/WW3D/pluglib)

target_link_libraries(ww3d2 PUBLIC wwlib)
