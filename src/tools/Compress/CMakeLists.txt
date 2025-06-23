add_executable(Compress EXCLUDE_FROM_ALL
    Compress.cpp
)

target_include_directories(Compress PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
