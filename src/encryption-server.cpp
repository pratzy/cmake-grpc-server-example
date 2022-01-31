#include "encryptor.hpp"
#include "encryptorservice.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <string>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using encryptorservice::MD5Encrypt;
using encryptorservice::RequestMessage;
using encryptorservice::ResponseMessage;

// Server Implementation
class EncryptionServiceImpl final : public MD5Encrypt::Service {
  Status sendRequest(ServerContext *context, const RequestMessage *request,
                     ResponseMessage *response) override {
    Message::Encryption::MD5Engine md5encryptor;
    response->set_encrypted(md5encryptor.encrypt(request->message()));
    return Status::OK;
  }
};

void run_server() {
  std::string server_address("0.0.0.0:50051");
  EncryptionServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  // Assembling the server
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on port: " << server_address << std::endl;

  server->Wait();
}

int main() {
  run_server();
  return 0;
}