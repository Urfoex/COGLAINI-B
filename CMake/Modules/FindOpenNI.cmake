# Locate OpenNI library
# This module defines
# OPENNI_LIBRARY, the name of the library to link against
# OPENNI_FOUND, if false, do not try to link against
# OPENNI_INCLUDE_DIR, where to find the headers
#
# $OPENNIDIR is an environment variable that would
# correspond to the ./configure --prefix=$OPENNI
# used in building OpenNI.

FIND_PATH(OPENNI_INCLUDE_DIR XnOpenNI.h
  HINTS
  $ENV{OPENNIDIR}
  PATH_SUFFIXES include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include/ni
  /usr/include/ni
  /usr/local/include
  /usr/include
  /sw/include/ni
  /sw/include
  /opt/local/include/ni
  /opt/local/include
  /opt/csw/include/ni
  /opt/csw/include 
  /opt/include/ni
  /opt/include
)
FIND_LIBRARY(OPENNI_LIBRARY 
  NAMES OpenNI
  HINTS
  $ENV{OPENNIDIR}
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

SET(OPENNI_FOUND "NO")
IF(OPENNI_LIBRARY AND OPENNI_INCLUDE_DIR)
  SET(OPENNI_FOUND "YES")
ENDIF(OPENNI_LIBRARY AND OPENNI_INCLUDE_DIR)

