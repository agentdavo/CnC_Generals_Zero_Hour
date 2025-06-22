file(GLOB COMPRESSION_SRCS CONFIGURE_DEPENDS
    *.cpp
    EAC/*.cpp
    LZHCompress/*.cpp
)

add_library(compression STATIC ${COMPRESSION_SRCS})

target_include_directories(compression PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
