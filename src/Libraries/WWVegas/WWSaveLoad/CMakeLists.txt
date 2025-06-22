file(GLOB WWSAVELOAD_SRCS CONFIGURE_DEPENDS
    *.cpp
)

add_library(wwsaveload STATIC ${WWSAVELOAD_SRCS})

target_include_directories(wwsaveload PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
