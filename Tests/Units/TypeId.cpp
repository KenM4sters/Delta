#include "../Includes.hpp"
#include <gtest/gtest.h>


TEST(ExampleTest, TypeID) 
{
    slv::TypeID id_1 = slv::TypeIDGenerator<Position>::GetNewID<Position>();
    slv::TypeID id_2 = slv::TypeIDGenerator<Position>::GetNewID<Velocity>();
    slv::TypeID id_3 = slv::TypeIDGenerator<Position>::GetNewID<Velocity>();
    slv::TypeID id_4 = slv::TypeIDGenerator<Position>::GetNewID<Texture>();

    std::cout << id_1 << std::endl;
    std::cout << id_2 << std::endl;
    std::cout << id_3 << std::endl;
    std::cout << id_4 << std::endl;

    EXPECT_NE(id_1, id_2);
    EXPECT_EQ(id_2, id_3);
    EXPECT_NE(id_3, id_4);
}
