add_executable(version_update EXCLUDE_FROM_ALL
    version_update.cpp
)

target_include_directories(version_update PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
