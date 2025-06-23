file(GLOB WWAUDIO_SRCS CONFIGURE_DEPENDS
    *.cpp
)

add_library(wwaudio STATIC ${WWAUDIO_SRCS})

target_include_directories(wwaudio PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/lib/miles-sdk-stub
    ${PROJECT_SOURCE_DIR}/lib/miles-sdk-stub/mss
)

target_link_libraries(wwaudio PUBLIC milesstub)
