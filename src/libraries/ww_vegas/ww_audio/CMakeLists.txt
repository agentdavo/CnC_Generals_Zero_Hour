file(GLOB WWAUDIO_SRCS CONFIGURE_DEPENDS
    *.cpp
)

add_library(wwaudio STATIC ${WWAUDIO_SRCS})

target_include_directories(wwaudio PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/lib/miles_sdk_stub
    ${PROJECT_SOURCE_DIR}/lib/miles_sdk_stub/mss
    ${PROJECT_SOURCE_DIR}/include/libraries/ww_vegas/ww_save_load
)

target_link_libraries(wwaudio PUBLIC milesstub wwlib wwsaveload)
