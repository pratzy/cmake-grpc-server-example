cmake_minimum_required(VERSION 3.10.0)
project(protobufexample)

# Cmake find modules
include(cmake/grpc.cmake)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++17")

find_package(Protobuf REQUIRED)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

find_package(gRPC CONFIG REQUIRED)

include_directories(include)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})


# Build encryptor as interface and link main target against it
find_package(Poco REQUIRED COMPONENTS Foundation Crypto)
set(Encryptor_LIBRARY encryptor)
add_library(${Encryptor_LIBRARY} INTERFACE)
SET(Poco_INCLUDE_DIRS "/usr/local/include/Poco")
MESSAGE( [Main] " Poco_INCLUDE_DIRS = ${Poco_INCLUDE_DIRS}")
MESSAGE( [Main] " Poco_LIBRARIES = ${Poco_LIBRARIES}")
target_link_libraries(${Encryptor_LIBRARY} INTERFACE ${Poco_LIBRARY} Poco::Crypto)
target_include_directories(${Encryptor_LIBRARY} INTERFACE include ${Poco_INCLUDE_DIRS})


# Proto file
get_filename_component(encryptor_service_proto "proto/encryptorservice.proto" ABSOLUTE)
get_filename_component(encryptor_service_proto_path "${encryptor_service_proto}" PATH)

# Generated sources
set(encryptor_service_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/encryptorservice.pb.cc")
set(encryptor_service_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/encryptorservice.pb.h")
set(encryptor_service_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/encryptorservice.grpc.pb.cc")
set(encryptor_service_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/encryptorservice.grpc.pb.h")
add_custom_command(
      OUTPUT "${encryptor_service_proto_srcs}" "${encryptor_service_proto_hdrs}" "${encryptor_service_grpc_srcs}" "${encryptor_service_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${encryptor_service_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${encryptor_service_proto}"
      DEPENDS "${encryptor_service_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# encryptor_service_grpc_proto
add_library(encryptor_service_grpc_proto
  ${encryptor_service_grpc_srcs}
  ${encryptor_service_grpc_hdrs}
  ${encryptor_service_proto_srcs}
  ${encryptor_service_proto_hdrs})
target_link_libraries(encryptor_service_grpc_proto PUBLIC
  ${Encryptor_LIBRARY} 
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})


add_subdirectory(src) 