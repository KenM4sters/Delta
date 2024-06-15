#include "Includes.hpp"
#include "glad/gl.h"
#include "GLFW/glfw3.h"


static GLFWwindow* sWindow = nullptr;

static void InitContext();

static void WindowKeyCallback(GLFWwindow* window, int key, int mods, int scancode, int action);

int main(int argc, char** argv) 
{

    InitContext();

    slv::Registry registry;

    slv::Entity player(&registry);
    slv::Entity enemy(&registry);

    registry.RegisterComponent<Position>(); 
    registry.RegisterComponent<Velocity>(); 
    registry.RegisterComponent<Texture>(); 
    registry.RegisterComponent<BoundingBox>(); 

    player.AddComponent<Position>(5.0f, 3.0f);
    player.AddComponent<Velocity>(9.0f, 9.0f);
    player.AddComponent<Texture>("Test1.png");
    player.AddComponent<BoundingBox>(2.0f, 2.0f);
 
    enemy.AddComponent<Position>(8.0f, 6.0f);
    enemy.AddComponent<Velocity>(10.0f, 15.0f);
    enemy.AddComponent<Texture>("Test2.png");
    enemy.AddComponent<BoundingBox>(4.0f, 4.0f);

    
    slv::System<Position> system(registry, 1);
    slv::System<Velocity> system2(registry, 1);
    slv::System<Texture> system3(registry, 2);

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

    system3.Action([](const float ts, const std::vector<slv::EntityID>& entities, Texture* t) 
    {
        for(int i = 0; i < entities.size(); i++) 
        {
            std::cout << t[i].texture << std::endl;
        }
    });

    registry.RunSystems(1, 1);
    registry.RunSystems(2, 1);



    while(!glfwWindowShouldClose(sWindow)) 
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        glfwPollEvents();
        glfwSwapBuffers(sWindow);
    }
}


void InitContext() 
{
    glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    sWindow = glfwCreateWindow(800, 600, "window", NULL, NULL);
    glfwMakeContextCurrent(sWindow);

    glfwSetKeyCallback(sWindow, WindowKeyCallback);

	gladLoadGL(glfwGetProcAddress);
}


void WindowKeyCallback(GLFWwindow* window, int key, int mods, int scancode, int action) 
{
    if(key == GLFW_KEY_ESCAPE) 
    {
        glfwSetWindowShouldClose(sWindow, true);
    }
}


