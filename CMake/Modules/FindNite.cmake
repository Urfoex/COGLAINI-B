# Locate Nite library
# This module defines
# NITE_LIBRARY, the name of the library to link against
# NITE_FOUND, if false, do not try to link against
# NITE_INCLUDE_DIR, where to find the headers
#
# $NITEDIR is an environment variable that would
# correspond to the ./configure --prefix=$NITE
# used in building Nite.

FIND_PATH(NITE_INCLUDE_DIR XnVNite.h
  HINTS
  $ENV{NITEDIR}
  PATH_SUFFIXES include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include/nite
  /usr/include/nite
  /usr/local/include
  /usr/include
  /sw/include/nite
  /sw/include
  /opt/local/include/nite
  /opt/local/include
  /opt/csw/include/nite
  /opt/csw/include 
  /opt/include/nite
  /opt/include
)
FIND_LIBRARY(NITE_LIBRARY 
  NAMES XnVNite_1_4_2
  HINTS
  $ENV{NITEDIR}
  PATH_SUFFIXES lib64 lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

SET(NITE_FOUND "NO")
IF(NITE_LIBRARY AND NITE_INCLUDE_DIR)
  SET(NITE_FOUND "YES")
ENDIF(NITE_LIBRARY AND NITE_INCLUDE_DIR)

