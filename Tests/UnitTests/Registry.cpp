#include <gtest/gtest.h>
#include <Silverback.hpp>
#include "Components.hpp"


TEST(ExampleTest, Registry) 
{
    slv::Registry registry;

    slv::Entity player(&registry);

    Position position{1.0f};

    player.AddComponent<Position>(1.0f);

}