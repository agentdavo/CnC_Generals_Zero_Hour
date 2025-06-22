file(GLOB WWUTIL_SRCS CONFIGURE_DEPENDS
    *.cpp
)

add_library(wwutil STATIC ${WWUTIL_SRCS})

target_include_directories(wwutil PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
