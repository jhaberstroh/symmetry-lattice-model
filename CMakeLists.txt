CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET( PROJ_NAME     "mcarlogui")
SET( PROJ_PATH     ${CMAKE_SOURCE_DIR} )
SET( PROJ_OUT_PATH ${CMAKE_BINARY_DIR} )
SET( PROJ_SOURCES  
  "src/filehandler.cpp" 
  "src/site.cpp" 
  "src/lattice.cpp" 
  "src/interaction.cpp" 
  "src/montecarlo.cpp" )
SET( PROJ HEADERS 
  "inc/Metropolis_Growth/filehandler.h"
  "inc/Metropolis_Growth/site.h"
  "inc/Metropolis_Growth/lattice.h"
  "inc/Metropolis_Growth/interaction.h"
  "inc/Metropolis_Growth/montecarlo.h" )
SET( PROJ_LIBRARIES "" )
SET( PROJ_INCLUDES "inc" )
  

PROJECT( ${PROJ_NAME} CXX )

INCLUDE_DIRECTORIES( ${PROJ_INCLUDES} )
ADD_EXECUTABLE( ${PROJ_NAME} ${PROJ_SOURCES} )
TARGET_LINK_LIBRARIES( ${PROJ_NAME} ${PROJ_LIBRARIES} )
ADD_DEFINITIONS(-g -O2 -Wall)

ADD_EXECUTABLE(mcarlo main.cpp)

TARGET_LINK_LIBRARIES(
