#pragma once

#include <Poco/DigestStream.h>
#include <Poco/MD5Engine.h>
#include <string>

namespace Message::Encryption {
class MD5Engine;
}

class Message::Encryption::MD5Engine {
public:
  MD5Engine(){};

  const std::string encrypt(const std::string &str) {
    Poco::DigestOutputStream ds(md5);
    ds << str;
    ds.close();
    return Poco::DigestEngine::digestToHex(md5.digest());
  }

private:
  Poco::MD5Engine md5;
};
