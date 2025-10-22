# Build & Run unit tests

Ensure you have gcc or any c++ compiler added into your system path. Then, run the following commands on Windows for example:

Run directly from project root directory:
```
cmake -S . -B build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

# Build & Run application tests
This application allows playing manually with the alarm system.
It requires `sudo apt-get install sox` to play the audio beeps on Linux




