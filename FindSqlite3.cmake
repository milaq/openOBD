FIND_PATH(Sqlite3_INCLUDE_DIR sqlite3.h /usr/include/)

FIND_LIBRARY(Sqlite3_LIBRARY NAMES sqlite3 PATH /usr/lib /usr/local/lib) 

IF (Sqlite3_INCLUDE_DIR AND Sqlite3_LIBRARY)
   SET(Sqlite3_FOUND TRUE)
ENDIF (Sqlite3_INCLUDE_DIR AND Sqlite3_LIBRARY)


IF (Sqlite3_FOUND)
   IF (NOT Sqlite3_FIND_QUIETLY)
      MESSAGE(STATUS "Found Sqlite3: ${Sqlite3_LIBRARY}")
   ENDIF (NOT Sqlite3_FIND_QUIETLY)
ELSE (Sqlite3_FOUND)
   IF (Sqlite3_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find Sqlite3")
   ENDIF (Sqlite3_FIND_REQUIRED)
ENDIF (Sqlite3_FOUND)

