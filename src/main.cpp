#include "message.pb.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>
#include <iostream>

int main() {
  message::Message m;
  m.set_id(10);
  m.set_text("This is the text from message");

  if (std::string s; google::protobuf::TextFormat::PrintToString(m, &s)) {
    std::cout << "Your message: " << s;
  } else {
    std::cerr << "Message not valid (partial content: " << m.ShortDebugString()
              << ")\n";
  }

  std::string s2;
  m.SerializeToString(&s2);

  std::cout << "Message as string: " << s2 << std::endl;

  std::string s3;
  google::protobuf::util::MessageToJsonString(m, &s3);
  std::cout << "Message as json: " << s3 << std::endl;
}