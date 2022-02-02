cmake_minimum_required(VERSION 3.10.0)
project(protobufexample)

list(APPEND CMAKE_PREFIX_PATH "/usr/local/lib/cmake/grpc")

# Cmake find modules
# list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake") 

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++17")

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf REQUIRED)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

find_package(gRPC CONFIG REQUIRED)

include_directories(include)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS DESCRIPTORS PROTO_DESCS proto/message.proto)
set(Message_LIBRARY message)
add_library(${Message_LIBRARY} OBJECT ${PROTO_SRCS} ${PROTO_HDRS})
set_property(TARGET ${Message_LIBRARY} PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library(${Message_LIBRARY}_shared SHARED $<TARGET_OBJECTS:${Message_LIBRARY}>)
set_target_properties(${Message_LIBRARY}_shared PROPERTIES OUTPUT_NAME ${Message_LIBRARY})

add_library(${Message_LIBRARY}_static STATIC $<TARGET_OBJECTS:${Message_LIBRARY}>)
set_target_properties(${Message_LIBRARY}_static PROPERTIES OUTPUT_NAME ${Message_LIBRARY})

# gRPC
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
# set(_GRPC_GRPCPP gRPC::grpc++)
# set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

# # Proto file
# get_filename_component(hw_proto "proto/encryptor.proto" ABSOLUTE)
# get_filename_component(hw_proto_path "${hw_proto}" PATH)
# # Generated sources
# set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/encryptor.pb.cc")
# set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/encryptor.pb.h")
# set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/encryptor.grpc.pb.cc")
# set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/encryptor.grpc.pb.h")
# add_custom_command(
#       OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
#       COMMAND ${_PROTOBUF_PROTOC}
#       ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
#         --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
#         -I "${hw_proto_path}"
#         --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
#         "${hw_proto}"
#       DEPENDS "${hw_proto}")

# include_directories("${CMAKE_CURRENT_BINARY_DIR}")

set(PROTO_FILES proto/encryptor.proto)
add_library(encryptorproto ${PROTO_FILES})
target_link_libraries(encryptorproto PUBLIC 
  protobuf::libprotobuf
  gRPC::grpc
  gRPC::grpc++)
target_include_directories(encryptorproto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
message("GRPC CPP PLUGIN: ${grpc_cpp_plugin_location}")
protobuf_generate(TARGET encryptorproto LANGUAGE cpp)
protobuf_generate(TARGET encryptorproto LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h PLUGIN protoc-gen-grpc=${grpc_cpp_plugin_location})

# protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto encryptor.proto)
# grpc_generate_cpp(GRPC_SRCS GRPC_HDRS proto encryptor.proto)


# Build encryptor as interface and link main target against it
find_package(Poco REQUIRED COMPONENTS Foundation Crypto)
set(Encryptor_LIBRARY encryptor)
add_library(${Encryptor_LIBRARY} INTERFACE)
SET(Poco_INCLUDE_DIRS "/usr/local/include/Poco")
MESSAGE( [Main] " Poco_INCLUDE_DIRS = ${Poco_INCLUDE_DIRS}")
MESSAGE( [Main] " Poco_LIBRARIES = ${Poco_LIBRARIES}")
target_link_libraries(${Encryptor_LIBRARY} INTERFACE ${Poco_LIBRARY} Poco::Crypto)
target_include_directories(${Encryptor_LIBRARY} INTERFACE include ${Poco_INCLUDE_DIRS})

add_subdirectory(src) 