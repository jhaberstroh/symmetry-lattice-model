cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(PROJ_NAME "ModelQT_GUI")
set(LIBR_NAME "Metropolis_Growth")

# Build the Metropolis_Growth library first
add_subdirectory(${LIBR_NAME})

#
# Build the Qt GUI for the library
#
#
project(${PROJ_NAME} CXX)


#Gets the QT path directory
find_path(QT_INC_DIR
  NAMES QtGui
  PATHS /usr/include
  /usr/include/qt4
  /usr/include/qt4/core)
if(NOT QT_INC_DIR)
  message(FATAL_ERROR "Unable to find Qt include files")
else(NOT QT_INC_DIR)
  string(REGEX REPLACE "/QtGui" "" QT_INC_DIR "${QT_INC_DIR}")
  message("Found Qt include files in " ${QT_INC_DIR})
endif(NOT QT_INC_DIR)

#We want to be able to reference LIBR_NAME.a
include_directories(${QT_INC_DIR} 
  ${PROJ_NAME}
  ${PROJ_NAME}/inc/${PROJ_NAME} 
  ${LIBR_NAME}/bin 
  ${LIBR_NAME}/inc/${LIBR_NAME})

add_definitions(-g -O2 -Wall)

#loads package which contains macros beginning with QT_
find_package(Qt4 REQUIRED)

#Define macros for sources and headers
SET(ModelQT_GUI_SOURCES   
  ${PROJ_NAME}/src/main.cpp 
  ${PROJ_NAME}/src/functionthread.cpp 
  ${PROJ_NAME}/src/mainwindow.cpp 
  ${PROJ_NAME}/src/simpleclass.cpp)
SET(ModelQT_GUI_HEADERS 
  ${PROJ_NAME}/inc/${PROJ_NAME}/functionthread.h 
  ${PROJ_NAME}/inc/${PROJ_NAME}/mainwindow.h 
  ${PROJ_NAME}/inc/${PROJ_NAME}/simpleclass.h)
SET(ModelQT_GUI_FORMS ${PROJ_NAME}/ui/mainwindow.ui)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJ_NAME}/bin)
#Code is unhappy about this being the current directory. But then, the WRAP_UI doesn't seem to be going to this directory. But it is going to the working directory, so that seems to be a satisfactory resolution?
set(CMAKE_CURRENT_BINARY_DIRECTORY ./)

#Build the moc headers which are needed to process Q_OBJECT header macros
QT4_WRAP_CPP(ModelQT_GUI_HEADERS_MOC ${ModelQT_GUI_HEADERS})
#Create a .h file for the .ui file, wham bam.
QT4_WRAP_UI(ModelQT_GUI_FORMS_HEADERS ${ModelQT_GUI_FORMS})
#Adds resources (like images) used by the QT program
# QT4_ADD_RESOURCES()

include_directories("${CMAKE_CURRENT_BINARY_DIRECTORY}")
include_directories(${PNG_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIR} ${PNGWRITER_INCLUDE_DIR} /usr/include/freetype2)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

add_executable(${PROJ_NAME}.exe #output name
  ${ModelQT_GUI_SOURCES} 
  ${ModelQT_GUI_HEADERS_MOC} 
  ${ModelQT_GUI_FORMS_HEADERS})
target_link_libraries(${PROJ_NAME}.exe ${QT_LIBRARIES} ${LIBR_NAME})