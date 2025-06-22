file(GLOB WWMATH_SRCS CONFIGURE_DEPENDS
    *.cpp
)

add_library(wwmath STATIC ${WWMATH_SRCS})

target_include_directories(wwmath PUBLIC
    ${PROJECT_SOURCE_DIR}/src/Libraries/WWVegas/WWMath
)
