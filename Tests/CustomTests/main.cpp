#include <Entity.hpp>
#include <iostream>

int main(int argc, char** argv) 
{
    class X {float val;};
    class Y {float val;};
    class Z {float val;};
    class W {float val;};

    dt::TypeID id_1 = dt::TypeIDGenerator<X>::GetNewID<X>();
    dt::TypeID id_2 = dt::TypeIDGenerator<X>::GetNewID<X>();
    dt::TypeID id_3 = dt::TypeIDGenerator<X>::GetNewID<X>();
    dt::TypeID id_4 = dt::TypeIDGenerator<X>::GetNewID<X>();

    std::cout << id_1 << std::endl;
    std::cout << id_2 << std::endl;
    std::cout << id_3 << std::endl;
    std::cout << id_4 << std::endl;
}