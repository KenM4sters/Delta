#ifndef SILVERBACK_UNIT_TESTS_COMPONENTS_HPP
#define SILVERBACK_UNIT_TESTS_COMPONENTS_HPP

#include <Silverback.hpp>

struct Position 
{
    Position() {}
    Position(float x, float y) : x{x}, y{y} {}
    float x;
    float y;
};

struct Velocity 
{
    Velocity() {}
    Velocity(float x, float y) : x{x}, y{y} {}
    float x;
    float y;
};

struct BoundingBox 
{
    BoundingBox() {}
    BoundingBox(float x, float y) : x{x}, y{y} {}
    float x;
    float y;
};

struct Texture
{
    Texture() {}
    Texture(const char* tex) : texture{tex} {}
    const char* texture = "";
};

#endif