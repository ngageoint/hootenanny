#
# Qt qmake integration with Google Protocol Buffers compiler protoc
# 
# To compile protocol buffers with qt qmake, specify PROTOS variable and
# include this file
#
# Example:
# PROTOS = a.proto b.proto
# include(protobuf.pri)
#
# By default protoc looks for .proto files (including the imported ones) in
# the current directory where protoc is run. If you need to include additional
# paths specify the PROTOPATH variable
#

PROTOPATH += .
PROTOPATHS = 
for(p, PROTOPATH):PROTOPATHS += --proto_path=$${p}

PROTO_TMP=src/main/cpp/hoot/core/proto/

protobuf_decl.name  = protobuf header
protobuf_decl.input = PROTOS
protobuf_decl.output  = $${PROTO_TMP}${QMAKE_FILE_BASE}.pb.h
protobuf_decl.commands = mkdir -p $${PROTO_TMP} && protoc --cpp_out=$${PROTO_TMP} $${PROTOPATHS} ${QMAKE_FILE_NAME}
protobuf_decl.variable_out = GENERATED_FILES 
QMAKE_EXTRA_COMPILERS += protobuf_decl 
#message($${protobuf_decl.output})

protobuf_impl.name  = protobuf implementation
protobuf_impl.input = PROTOS
protobuf_impl.output  = $${PROTO_TMP}${QMAKE_FILE_BASE}.pb.cc
protobuf_impl.depends  = $${PROTO_TMP}${QMAKE_FILE_BASE}.pb.h
protobuf_impl.commands = $$escape_expand(\\n)
protobuf_impl.variable_out = GENERATED_SOURCES
QMAKE_EXTRA_COMPILERS += protobuf_impl 
