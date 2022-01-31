set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Build encryption-server
foreach(_target encryption-server encryption-client)
add_executable(${_target} "${_target}.cpp")
target_link_libraries(${_target} PRIVATE
encryptor_service_grpc_proto
)
endforeach()
