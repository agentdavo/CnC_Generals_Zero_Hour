file(GLOB COMPRESSION_SRCS CONFIGURE_DEPENDS
    *.cpp
    eac/*.cpp
    lzh_compress/*.cpp
)

add_library(compression STATIC ${COMPRESSION_SRCS})

target_include_directories(compression PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/lib/liblzhl/include/lzhl
    ${PROJECT_SOURCE_DIR}/lib/zlib
    ${CMAKE_CURRENT_SOURCE_DIR}/eac
    ${CMAKE_CURRENT_SOURCE_DIR}/lzh_compress
)

target_link_libraries(compression PUBLIC zlib lzhl)
