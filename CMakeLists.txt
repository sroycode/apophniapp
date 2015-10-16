set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

cmake_minimum_required (VERSION 2.8)
project (apophniapp)

# compiler version check
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
		message(FATAL_ERROR "GCC version >=4.9 required.")
	endif()
	set (CMAKE_COMPILER_IS_GNUCC 1)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.3)
		message(FATAL_ERROR "Clang version >=3.3 required.")
	endif()
	set (CMAKE_COMPILER_IS_CLANG 1)
else()
	message(WARNING "Your compiler (${CMAKE_CXX_COMPILER_ID}) has not been tested on this project.")
endif()
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)

if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Release")
endif ()
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set (DEBUGMODE ON)
endif ()
if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    # cmake bug workaround -- on some platforms, cmake doesn't set NDEBUG for RelWithDebInfo mode
    add_definitions ("-DNDEBUG")
endif ()

# disallow in-source builds
if ("${PROJECT_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message (FATAL_ERROR "Not allowed to run in-source build!")
endif ()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message (STATUS "Project source dir = ${PROJECT_SOURCE_DIR}")
message (STATUS "Project build dir = ${CMAKE_BINARY_DIR}")

option(APOPHNIAPP_BUILD_WITH_SSL "Build ssl support." ON)
option(APOPHNIAPP_BUILD_WITH_LMDB "Build backend lmdb." ON)
option(APOPHNIA_BUILD_STATIC "Build static library." OFF)

SET_PROPERTY(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
SET_PROPERTY(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)

find_package(Threads REQUIRED)
find_package(Boost 1.54.0 COMPONENTS system thread coroutine context date_time REQUIRED)
find_package(GLog REQUIRED)
find_package(ZeroMQ REQUIRED)
find_package(ProtobufPlugin REQUIRED)
find_package(ZQRPC REQUIRED)
find_package(VIPS REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GLOG_INCLUDE_DIRS})
include_directories(${ZeroMQ_INCLUDE_DIRS})
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${ZQRPC_INCLUDE_DIRS})
include_directories(${VIPS_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)

## libs
set(APOPHNIAPP_LIB_DEPS
	${CMAKE_THREAD_LIBS_INIT}
	${Boost_LIBRARIES}
	${GLOG_LIBRARIES}
	${ZeroMQ_LIBRARIES}
	${PROTOBUF_LIBRARY}
	${ZQRPC_LIBRARIES}
	${VIPS_LIBRARIES}
)

if (APOPHNIAPP_BUILD_WITH_SSL)
	find_package(OpenSSL REQUIRED)
	include_directories(${OPENSSL_INCLUDE_DIR})
	set(APOPHNIAPP_LIB_DEPS ${APOPHNIAPP_LIB_DEPS} ${OPENSSL_SSL_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARIES})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAPOPHNIAPP_BUILD_WITH_SSL=1")
endif()

if (APOPHNIAPP_BUILD_WITH_LMDB)
	find_package(Lmdb REQUIRED)
	include_directories(${LMDB_INCLUDE_DIRS})
	set(APOPHNIAPP_LIB_DEPS ${APOPHNIAPP_LIB_DEPS} ${LMDB_LIBRARIES})
endif()

add_definitions ("-std=c++11")
# Clang-specific options
if (CMAKE_COMPILER_IS_CLANG OR CMAKE_COMPILER_IS_APPLECLANG)
    #add_definitions ("-Wno-unused-function")
    #add_definitions ("-Wno-overloaded-virtual")
    #add_definitions ("-Wno-unneeded-internal-declaration")
    #add_definitions ("-Wno-unused-private-field")
    #add_definitions ("-Wno-tautological-compare")
    # disable warning about unused command line arguments
    #add_definitions ("-Qunused-arguments")
    # Don't warn if we ask it not to warn about warnings it doesn't know
    #add_definitions ("-Wunknown-warning-option")
    if (CLANG_VERSION_STRING VERSION_GREATER 3.5 OR APPLECLANG_VERSION_STRING VERSION_GREATER 6.0)
        add_definitions ("-Wno-unused-local-typedefs")
    endif ()
elseif(CMAKE_COMPILER_IS_GNUCC)
    if (NOT ${GCC_VERSION} VERSION_LESS 4.8)
        add_definitions("-Wno-error=unused-local-typedefs")
        add_definitions("-Wno-unused-local-typedefs")
    endif ()
endif ()

set(CMAKE_OSX_ARCHITECTURES x86_64)

add_subdirectory(src)
