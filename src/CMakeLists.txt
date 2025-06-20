add_executable(GeneralsStub main.cpp)
target_include_directories(GeneralsStub PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/Precompiled
)
