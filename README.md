# Silverback

Silverback is a flexible, header-only ECS framework that's designed to offer a comfortable balance between
peformance and ease of use. 

This is currently one of my main projects that I'm working on, so expect bugs and frequent changes.



## Features

- Template programming for a flexible, high peformance API
- Unit Tests
- CMake build system
- An intuitive API
- Header-only
- Centralised Resigstry class for ease of use
- Base class pointer system to support virtually any object as components without having 
  to use inheritance
- Flexible System class integration with custom user defined callbacks


## Installation

Currently only supporting MacOS, with windows support currently being worked on.

Clone and build with CMake.

```bash
git clone [link-to-repo]

mkdir -p build
cd build
cmake -S ../ -B .
make
cd ..

// To run the test tutorials...
build/Tests/Tutorial1/Tutorial1
```
    