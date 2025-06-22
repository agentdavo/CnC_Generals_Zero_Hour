add_executable(mangler EXCLUDE_FROM_ALL
    mangler.cpp
    crc.cpp
    manglertest.cpp)
# old code uses non-standard headers and will require cleanup before this target builds
