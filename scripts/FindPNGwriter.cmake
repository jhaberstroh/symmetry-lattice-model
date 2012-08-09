FIND_PATH(PNGWRITER_INCLUDE_DIR pngwriter.h /usr/local/include)
FIND_LIBRARY(PNGWRITER_LIBRARY pngwriter /usr/local/lib) 

IF (PNGWRITER_INCLUDE_DIR AND PNGWRITER_LIBRARY)
SET(PNGWRITER_FOUND TRUE)
ENDIF (PNGWRITER_INCLUDE_DIR AND PNGWRITER_LIBRARY)

IF (PNGWRITER_FOUND)
    IF (NOT PNGwriter_FIND_QUIETLY)
        MESSAGE(STATUS "Found PNGwriter: ${PNGWRITER_LIBRARY}")
    ENDIF (NOT PNGwriter_FIND_QUIETLY)
ELSE (PNGWRITER_FOUND)
    IF (PNGwriter_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find PNGwriter")
    ENDIF (PNGwriter_FIND_REQUIRED)
ENDIF (PNGWRITER_FOUND)
