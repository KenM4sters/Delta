#include "../Includes.hpp"

int main(int argc, char** argv) 
{

    slv::Registry registry;

    slv::Entity player(&registry);
    slv::Entity enemy(&registry);

    registry.RegisterComponent<Position>(); 
    registry.RegisterComponent<Velocity>(); 
    registry.RegisterComponent<Texture>(); 
    registry.RegisterComponent<BoundingBox>(); 

    player.AddComponent<Position>(5.0f, 3.0f);
    player.AddComponent<Velocity>(9.0f, 9.0f);
    player.AddComponent<Texture>("");
    player.AddComponent<BoundingBox>(2.0f, 2.0f);
 
    enemy.AddComponent<Position>(8.0f, 6.0f);
    enemy.AddComponent<Velocity>(10.0f, 15.0f);
    enemy.AddComponent<Texture>("");
    enemy.AddComponent<BoundingBox>(4.0f, 4.0f);

     
    slv::System<Position> system(registry, 1);
    slv::System<Velocity> system2(registry, 1);

    system.Action([](const float ts, const std::vector<slv::EntityID>& entities, Position* p) 
    {
        for(int i = 0; i < entities.size(); i++) 
        {
            std::cout << p[i].x << std::endl;
            std::cout << p[i].y << std::endl;
        }
    });

    system2.Action([](const float ts, const std::vector<slv::EntityID>& entities, Velocity* v) 
    {
        for(int i = 0; i < entities.size(); i++) 
        {
            std::cout << v[i].x << std::endl;
            std::cout << v[i].y << std::endl;
        }
    });

    registry.RunSystems(1, 1);
}