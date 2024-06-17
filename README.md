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

## Example Code

```
struct Velocity { float x; float y; };
struct Posiiton { float x; float y; };
struct Material { float r; float g; float b; };
struct Renderable { GLuint vertexArray; GLuint shader; GLuint texture;  };

using namespace slv;

Registry registry;

auto player = registry.CreateEntity();

player.Add<Position>(0.0f, 0.0f);
player.Add<Velocity>({0.0f, 0.0f});
player.Add<Material>({1.0f, 0.5f, 0.0f});
player.Add<Renderable>(1, 1, 1);

auto physics = registry.CreateSystem<Position, Velocity>(0);

physics.Action([](const float ts, const std::vector<Entity>& entities, 
  Position* p Velocity* v) 
{
  for(size_t i = 0; i < entities.size(); i++) 
  {
    p[i] += v[i];
  }
});

bool running = true;

while(running) 
{
  registry.RunSystems(0.016, 0);
}

```


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
    