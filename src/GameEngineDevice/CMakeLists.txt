add_library(gameenginedevice EXCLUDE_FROM_ALL
    W3DDevice/GameLogic/W3DGameLogic.cpp
)

target_include_directories(gameenginedevice PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/GameEngineDevice
    ${PROJECT_SOURCE_DIR}/include/Precompiled
)

# Link against the Miles Sound System stub which in turn
# bridges legacy audio calls to miniaudio. This removes the
# remaining DirectSound dependency from the GameEngineDevice
# module.
target_link_libraries(gameenginedevice PUBLIC milesstub)
# Partial migration of GameEngineDevice sources
