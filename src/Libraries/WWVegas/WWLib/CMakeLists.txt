file(GLOB WWLIB_SRCS CONFIGURE_DEPENDS
    *.cpp
)

add_library(wwlib STATIC ${WWLIB_SRCS})

target_include_directories(wwlib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src/Libraries/WWVegas/WWLib
    ${PROJECT_SOURCE_DIR}/src/Libraries/WWVegas/WWDebug
)

target_link_libraries(wwlib PUBLIC wwdebug)
