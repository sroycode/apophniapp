
# - Try to find ZQRPC
# Once done this will define
#  ZQRPC_FOUND - System has ZQRPC
#  ZQRPC_INCLUDE_DIRS - The ZQRPC include directories
#  ZQRPC_LIBRARIES - The libraries needed to use ZQRPC
#  ZQRPC_DEFINITIONS - Compiler switches required for using ZQRPC

find_package(PkgConfig)
pkg_check_modules(PC_ZQRPC QUIET libzqrpc)
set(ZQRPC_DEFINITIONS ${PC_ZQRPC_CFLAGS_OTHER})

find_path(ZQRPC_INCLUDE_DIR zqrpc.hpp
       HINTS ${PC_ZQRPC_INCLUDEDIR} ${PC_ZQRPC_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/thirdparty/include )

find_library(ZQRPC_LIBRARY NAMES libzqrpc.a zqrpc libzqrpc
       HINTS ${PC_ZQRPC_LIBDIR} ${PC_ZQRPC_LIBRARY_DIRS} ${PROJECT_SOURCE_DIR}/thirdparty/lib )

find_program(PROTOC_GEN_CPP_ZQRPC NAMES protoc-gen-cpp_zqrpc
       HINTS ${PC_ZQRPC_BINDIR} ${PC_ZQRPC_BIN_DIRS} ${PROJECT_SOURCE_DIR}/thirdparty/bin )
find_program(PROTOC_GEN_PYTHON_ZQRPC NAMES protoc-gen-python_zqrpc
       HINTS ${PC_ZQRPC_BINDIR} ${PC_ZQRPC_BIN_DIRS} ${PROJECT_SOURCE_DIR}/thirdparty/bin )

set(ZQRPC_LIBRARIES ${ZQRPC_LIBRARY} )
set(ZQRPC_INCLUDE_DIRS ${ZQRPC_INCLUDE_DIR} )
set(PROTOC_GEN_CPP_ZQRPC ${PROTOC_GEN_CPP_ZQRPC} )

#include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set ZQRPC_FOUND to TRUE
# if all listed variables are TRUE
#find_package_handle_standard_args(ZQRPC  DEFAULT_MSG ZQRPC_LIBRARY ZQRPC_INCLUDE_DIR)
#mark_as_advanced(ZQRPC_INCLUDE_DIR ZQRPC_LIBRARY )

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(ZQRPC_PROCESS_INCLUDES ZQRPC_INCLUDE_DIR ZQRPC_INCLUDE_DIRS)
set(ZQRPC_PROCESS_LIBS ZQRPC_LIBRARY ZQRPC_LIBRARIES)
libfind_process(zqrpc)
