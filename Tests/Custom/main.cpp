#include "../Includes.hpp"

int main(int argc, char** argv) 
{

    slv::Registry registry;

    slv::Entity player(&registry);
    slv::Entity enemy(&registry);

    registry.RegisterComponent<Position>(); 
    registry.RegisterComponent<Velocity>(); 
    registry.RegisterComponent<Texture>(); 

    player.AddComponent<Position>(5.0f, 3.0f);
    player.AddComponent<Velocity>(9.0f, 9.0f);
 
    enemy.AddComponent<Position>(8.0f, 6.0f);
    enemy.AddComponent<Velocity>(10.0f, 15.0f);


    slv::System<Position> system(registry, 1);
    slv::System<Velocity> system2(registry, 1);

    system.Action([](const float ts, const std::vector<slv::EntityID>& entities, Position* p) 
    {
        for(const auto& entity : entities) 
        {
            std::cout << p->x << std::endl;
            std::cout << p->y << std::endl;
        }
    });

    system2.Action([](const float ts, const std::vector<slv::EntityID>& entities, Velocity* v) 
    {
        for(const auto& entity : entities) 
        {
            std::cout << v->x << std::endl;
            std::cout << v->y << std::endl;
        }
    });

    registry.RunSystems(1, 1);
}