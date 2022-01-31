#include <grpcpp/grpcpp.h>

#include <string>

#include "encryptorservice.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using encryptorservice::MD5Encrypt;
using encryptorservice::RequestMessage;
using encryptorservice::ResponseMessage;

// Client Implementation
class EncryptorClient {
public:
  EncryptorClient(std::shared_ptr<Channel> channel)
      : stub_(MD5Encrypt::NewStub(channel)) {}

  std::string sendRequest(std::string a) {
    RequestMessage request;
    request.set_message(a);

    ResponseMessage response;

    ClientContext context;

    Status status = stub_->sendRequest(&context, request, &response);

    if (status.ok()) {
      return response.encrypted();
    } else {
      std::cerr << status.error_code() << ":" << status.error_message()
                << std::endl;
      return "RPC Failed";
    }
  }

private:
  std::unique_ptr<MD5Encrypt::Stub> stub_;
};

void run_client(const std::string &a) {
  std::string target_address("0.0.0.0:50051");

  EncryptorClient client(
      // Channel from which RPCs are made - endpoint is the target_address
      grpc::CreateChannel(target_address,
                          // Indicate when channel is not authenticated
                          grpc::InsecureChannelCredentials()));
  std::string response;

  response = client.sendRequest(a);

  std::cout << "Original string: " << a << std::endl;
  std::cout << "Encrypted md5 string: " << response << std::endl;
}

int main(int argc, char *argv[]) {
  run_client(argv[1]);

  return 0;
}