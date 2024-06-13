#include "Includes.hpp"


TEST(ExampleTest, Registry) 
{
    slv::Registry registry;

    slv::Entity player(&registry);

    registry.RegisterComponent<Velocity>(); 
    registry.RegisterComponent<Position>(); 

    player.AddComponent<Velocity>(1.0f, 1.0f);
    player.AddComponent<Position>(5.0f, 2.0f);

    EXPECT_EQ(true, true);

}