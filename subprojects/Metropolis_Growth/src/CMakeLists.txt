set(Metropolis_Growth_src site.cpp lattice.cpp interaction.cpp montecarlo.cpp filehandler.cpp logfile.cpp latticefile.cpp orderparamfile.cpp)

# Uses the module "/usr/share/cmake-2.8/Modules/FindPNGwriter.cmake" to do the following:

set(OPTIMIZATION_FLAGS "-DNO_FREETYPE")
add_library(Metropolis_Growth STATIC ${Metropolis_Growth_src})
find_package(PNG) 
find_package(ZLIB) 
find_package(Freetype) 
find_package(PNGwriter)
include_directories(${PNG_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIR} ${PNGWRITER_INCLUDE_DIR} /usr/include/freetype2)
target_link_libraries(Metropolis_Growth ${PNG_LIBRARY} ${ZLIB_LIBRARY} ${FREETYPE_LIBRARY} ${PNGWRITER_LIBRARY})