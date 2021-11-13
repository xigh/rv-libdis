cmake_minimum_required(VERSION 2.80)

project(libdis)

add_subdirectory(rv-libdec)

add_library(libdis
    src/csr.c
    src/fmt.c
    src/reg.c
)

target_link_libraries(libdis rv-libdec)
target_include_directories(libdis PUBLIC include rv-libdec/include rv-libdec/rv-libsys/include rv-libdec/rv-libopc/include)
