file(GLOB WWDEBUG_SRCS CONFIGURE_DEPENDS
    *.cpp
)

add_library(wwdebug STATIC ${WWDEBUG_SRCS})

target_include_directories(wwdebug PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(wwdebug PUBLIC wwlib)
