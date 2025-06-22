file(GLOB WWAUDIO_SRCS CONFIGURE_DEPENDS
    *.cpp
)

add_library(wwaudio STATIC ${WWAUDIO_SRCS})

target_include_directories(wwaudio PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
