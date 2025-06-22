file(GLOB WWSHADE_SRCS CONFIGURE_DEPENDS
    *.cpp
)

add_library(wwshade STATIC ${WWSHADE_SRCS})

target_include_directories(wwshade PUBLIC
    ${PROJECT_SOURCE_DIR}/src/Libraries/WWVegas/wwshade
)
