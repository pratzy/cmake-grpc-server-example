1. Compile message.proto to C++ objects
2. Create a static library libmessage cpmposed of message source and header files
3. Use the symbols from libmessage in main.cpp - create a message object, set some fields and display it as a string

```
rm -rf build && mkdir build && cd build
cmake ..
cmake --build .

# In build directory
./bin/main
```
