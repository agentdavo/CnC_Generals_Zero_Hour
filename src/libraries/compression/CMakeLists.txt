file(GLOB COMPRESSION_SRCS CONFIGURE_DEPENDS
    *.cpp
    EAC/*.cpp
    LZHCompress/*.cpp
)

add_library(compression STATIC ${COMPRESSION_SRCS})

target_include_directories(compression PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/lib/liblzhl/include/lzhl 
    ${PROJECT_SOURCE_DIR}/lib/zlib
)

target_link_libraries(compression PUBLIC zlib lzhl)
