add_executable(buildVersionUpdate EXCLUDE_FROM_ALL
    buildVersionUpdate.cpp
)

target_include_directories(buildVersionUpdate PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
