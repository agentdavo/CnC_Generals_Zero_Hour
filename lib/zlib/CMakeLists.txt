cmake_minimum_required(VERSION 3.10)
project(zlib C)

add_library(zlib STATIC
    adler32.c
    compress.c
    crc32.c
    gzio.c
    uncompr.c
    deflate.c
    trees.c
    zutil.c
    infblock.c
    infcodes.c
    inftrees.c
    infutil.c
    inflate.c
    inffast.c
    maketree.c
)

target_include_directories(zlib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(zlib PROPERTIES POSITION_INDEPENDENT_CODE ON)
