#include <gtest/gtest.h>
#include <Silverback.hpp>
#include "Components.hpp"

TEST(ExampleTest, TypeID) 
{
    slv::TypeID id_1 = slv::TypeIDGenerator<Position>::GetNewID<Position>();
    slv::TypeID id_2 = slv::TypeIDGenerator<Velocity>::GetNewID<Velocity>();
    slv::TypeID id_3 = slv::TypeIDGenerator<Velocity>::GetNewID<Velocity>();
    slv::TypeID id_4 = slv::TypeIDGenerator<Velocity>::GetNewID<Velocity>();

    std::cout << id_1 << std::endl;
    std::cout << id_2 << std::endl;
    std::cout << id_3 << std::endl;
    std::cout << id_4 << std::endl;

    EXPECT_EQ(id_1, id_2);
    EXPECT_NE(id_1, id_3);
}
