find_program(PROTOC_GEN_CPP_ZQRPC NAMES protoc-gen-cpp_zqrpc)

function(PROTOBUF_GENERATE_ZQRPC_ONLY SRCS HDRS)
  PROTOBUF_GENERATE_MULTI(PLUGIN "cpp_zqrpc" PROTOS ${ARGN}
                          OUTPUT_STRUCT "_SRCS:.zqrpc.cc;_HDRS:.zqrpc.h"
                          FLAGS "--plugin=protoc-gen-cpp_zqrpc=${PROTOC_GEN_CPP_ZQRPC}")
  set(${SRCS} ${_SRCS} PARENT_SCOPE)
  set(${HDRS} ${_HDRS} PARENT_SCOPE)
endfunction()

function(PROTOBUF_GENERATE_ZQRPC SRCS HDRS)
    PROTOBUF_GENERATE_CPP(_SRCS_PB2 _HDRS_PB2 ${ARGN})
    PROTOBUF_GENERATE_ZQRPC_ONLY(_SRCS_ZQRPC _HDRS_ZQRPC ${ARGN})
    set(${SRCS} ${_SRCS_PB2} ${_SRCS_ZQRPC} PARENT_SCOPE)
    set(${HDRS} ${_HDRS_PB2} ${_HDRS_ZQRPC} PARENT_SCOPE)
endfunction()
