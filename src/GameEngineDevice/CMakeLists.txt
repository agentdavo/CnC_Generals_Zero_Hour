add_library(gameenginedevice EXCLUDE_FROM_ALL
    W3DDevice/GameLogic/W3DGameLogic.cpp
)

target_include_directories(gameenginedevice PUBLIC ${PROJECT_SOURCE_DIR}/include)
# Partial migration of GameEngineDevice sources
