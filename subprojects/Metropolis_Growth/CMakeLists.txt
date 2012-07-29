cmake_minimum_required( VERSION 2.8 )

set(PROJ_NAME "Metropolis_Growth")

project(${PROJ_NAME} CXX)

include_directories(inc/${PROJ_NAME} src)
add_definitions(-g -O2 -Wall)

add_subdirectory(src bin)
add_subdirectory(testing testbin)