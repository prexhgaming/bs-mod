cmake_minimum_required(VERSION 3.22.1)
project("bs_dumper")

add_library(
    bs_dumper
    SHARED
    sdk_dumper.cpp
)

find_library(log-lib log)

target_link_libraries(
    bs_dumper
    ${log-lib}
    dl
)