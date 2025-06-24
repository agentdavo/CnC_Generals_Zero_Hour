add_executable(build_version_update EXCLUDE_FROM_ALL
    build_version_update.cpp
)

target_include_directories(build_version_update PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
