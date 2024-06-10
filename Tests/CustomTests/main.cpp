#include <Silverback.hpp>
#include <iostream>

struct Position 
{
    float val;
};

struct Velocity 
{
    float val;
};

int main(int argc, char** argv) 
{

    slv::TypeID id_1 = slv::TypeIDGenerator<Position>::GetNewID<Position>();
    slv::TypeID id_2 = slv::TypeIDGenerator<Velocity>::GetNewID<Velocity>();
    slv::TypeID id_3 = slv::TypeIDGenerator<Velocity>::GetNewID<Velocity>();
    slv::TypeID id_4 = slv::TypeIDGenerator<Velocity>::GetNewID<Velocity>();

    std::cout << id_1 << std::endl;
    std::cout << id_2 << std::endl;
    std::cout << id_3 << std::endl;
    std::cout << id_4 << std::endl;
}