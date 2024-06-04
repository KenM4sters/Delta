#include <ECS.hpp>
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

    dt::TypeID id_1 = dt::TypeIDGenerator<Position>::GetNewID<Position>();
    dt::TypeID id_2 = dt::TypeIDGenerator<Velocity>::GetNewID<Velocity>();

    std::cout << id_1 << std::endl;
    std::cout << id_2 << std::endl;

    

}