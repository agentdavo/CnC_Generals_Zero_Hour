add_library(gameenginedevice EXCLUDE_FROM_ALL
    W3DDevice/GameLogic/W3DGameLogic.cpp
)

target_include_directories(gameenginedevice PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/GameEngineDevice
    ${PROJECT_SOURCE_DIR}/include/Precompiled
)
# Partial migration of GameEngineDevice sources
