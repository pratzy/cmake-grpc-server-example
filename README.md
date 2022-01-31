Requirements

Build and install following from source:

1. grpc (also installs protobuf as dependency)
2. Poco (Make sure latest release tag is used, else 'Crypto' package may not be included)

```
rm -rf build && mkdir build && cd build
cmake ..
cmake --build .

# In build directory
./bin/main
```
