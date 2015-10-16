# - Try to find LMDB
# Once done this will define
#  LMDB_FOUND - System has LMDB
#  LMDB_INCLUDE_DIRS - The LMDB include directories
#  LMDB_LIBRARIES - The libraries needed to use LMDB

find_path(LMDB_INCLUDE_DIR lmdb.h HINTS /usr/include /usr/local/include /opt/local/include )
find_library(LMDB_LIBRARY NAMES liblmdb.a liblmdb HINTS /usr/lib /usr/lib64 /usr/local/lib /opt/local/lib )

set(LMDB_LIBRARIES ${LMDB_LIBRARY} )
set(LMDB_INCLUDE_DIRS ${LMDB_INCLUDE_DIR} )

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(LMDB_PROCESS_INCLUDES LMDB_INCLUDE_DIR LMDB_INCLUDE_DIRS)
set(LMDB_PROCESS_LIBS LMDB_LIBRARY LMDB_LIBRARIES)
libfind_process(Lmdb)
