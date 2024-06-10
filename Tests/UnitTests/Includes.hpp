#ifndef SILVERBACK_UNIT_TESTS_COMPONENTS_HPP
#define SILVERBACK_UNIT_TESTS_COMPONENTS_HPP

#include <gtest/gtest.h>
#include <Silverback.hpp>

struct Position 
{
    float x;
};

struct Velocity 
{
    Velocity() {}
    Velocity(float x, float y) : x{x}, y{y} {}
    float x;
    float y;
};

#endif