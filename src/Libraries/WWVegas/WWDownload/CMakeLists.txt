file(GLOB WWDOWNLOAD_SRCS CONFIGURE_DEPENDS
    *.cpp
)

add_library(wwdownload STATIC ${WWDOWNLOAD_SRCS})

target_include_directories(wwdownload PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
