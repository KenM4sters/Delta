#include "Includes.hpp"


TEST(ExampleTest, Registry) 
{
    slv::Registry registry;

    slv::Entity player(&registry);

    registry.RegisterComponent<Velocity>(); 

    registry.AddComponent<Velocity>(1, 1.0f, 2.0f);

    EXPECT_EQ(true, true);

}