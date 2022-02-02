set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(PkgConfig REQUIRED)
pkg_search_module(GRPC REQUIRED grpc grpc++)


add_executable(main main.cpp)
target_link_libraries(main PUBLIC 
${Message_LIBRARY} 
${Encryptor_LIBRARY} 
${PROTOBUF_LIBRARY})
target_link_options(main PUBLIC "-pthread")
#  ${CMAKE_BINARY_DIR}/proto/encryptor.grpc.pb.cc ${CMAKE_BINARY_DIR}/proto/encryptor.pb.cc
add_executable(encryption-server encryption-server.cpp)
target_link_libraries(encryption-server PUBLIC 
${Encryptor_LIBRARY} 
${PROTOBUF_LIBRARY}
grpc++)