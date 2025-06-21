add_library(gameenginedevice EXCLUDE_FROM_ALL
    W3DDevice/GameLogic/W3DGameLogic.cpp
)

target_include_directories(gameenginedevice PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/GameEngine
    ${PROJECT_SOURCE_DIR}/include/GameEngineDevice
    ${PROJECT_SOURCE_DIR}/Generals/Code/GameEngine/Include
    ${PROJECT_SOURCE_DIR}/include/Precompiled
)

target_link_libraries(gameenginedevice PUBLIC milesstub)
