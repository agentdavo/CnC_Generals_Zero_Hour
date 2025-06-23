add_executable(MapCacheBuilder EXCLUDE_FROM_ALL
    win_main.cpp
)

target_include_directories(MapCacheBuilder PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
