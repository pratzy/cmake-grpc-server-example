cmake_minimum_required(VERSION 3.10.0)
project(protobufexample)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++17")

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HRDS DESCRIPTORS PROTO_DESCS proto/message.proto)
add_library(message OBJECT ${PROTO_SRCS} ${PROTO_HDRS})
set_property(TARGET message PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library(message_shared SHARED $<TARGET_OBJECTS:message>)
set_target_properties(message_shared PROPERTIES OUTPUT_NAME message)

add_library(message_static STATIC $<TARGET_OBJECTS:message>)
set_target_properties(message_static PROPERTIES OUTPUT_NAME message)

add_subdirectory(src)