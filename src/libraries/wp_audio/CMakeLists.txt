file(GLOB WPAUDIO_SRCS CONFIGURE_DEPENDS
    *.cpp
)

add_library(wp_audio STATIC ${WPAUDIO_SRCS})

target_include_directories(wp_audio PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
