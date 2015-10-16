
# - Try to find VIPS
# Once done this will define
#  VIPS_FOUND - System has VIPS
#  VIPS_INCLUDE_DIRS - The VIPS include directories
#  VIPS_LIBRARIES - The libraries needed to use VIPS
#  VIPS_DEFINITIONS - Compiler switches required for using VIPS

find_package(PkgConfig)
pkg_check_modules(PC_VIPS QUIET libvips)
set(VIPS_DEFINITIONS ${PC_VIPS_CFLAGS_OTHER})

find_path(VIPS_INCLUDE_DIR vips/vips8
       HINTS ${PC_VIPS_INCLUDEDIR} ${PC_VIPS_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/thirdparty/include )

find_library(VIPS_LIBRARY NAMES libvips.a vips libvips
       HINTS ${PC_VIPS_LIBDIR} ${PC_VIPS_LIBRARY_DIRS} ${PROJECT_SOURCE_DIR}/thirdparty/lib )

set(VIPS_LIBRARIES ${VIPS_LIBRARY} )
set(VIPS_INCLUDE_DIRS ${VIPS_INCLUDE_DIR} )

#include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set VIPS_FOUND to TRUE
# if all listed variables are TRUE
#find_package_handle_standard_args(VIPS  DEFAULT_MSG VIPS_LIBRARY VIPS_INCLUDE_DIR)
#mark_as_advanced(VIPS_INCLUDE_DIR VIPS_LIBRARY )

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(VIPS_PROCESS_INCLUDES VIPS_INCLUDE_DIR VIPS_INCLUDE_DIRS)
set(VIPS_PROCESS_LIBS VIPS_LIBRARY VIPS_LIBRARIES)
libfind_process(vips)
