add_library(wwshade INTERFACE)

target_include_directories(wwshade INTERFACE
  ${PROJECT_SOURCE_DIR}/src/libraries/ww_vegas/ww_shaders
)
