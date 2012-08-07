set(Testing_src metrotest.cpp)
set(RUNTIME_OUTPUT_DIRECTORY ../testbin)

add_executable(MetroTest ${Testing_src})
include_directories(${PNG_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIR} ${PNGWRITER_INCLUDE_DIR} /usr/include/freetype2)
target_link_libraries(MetroTest Metropolis_Growth)