add_executable(versionUpdate EXCLUDE_FROM_ALL
    versionUpdate.cpp
)

target_include_directories(versionUpdate PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
