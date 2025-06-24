add_executable(compress EXCLUDE_FROM_ALL
    compress.cpp
)

target_include_directories(compress PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
