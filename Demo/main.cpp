#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "Silverback.hpp"

struct Position 
{
    Position() {}
    Position(float x, float y) : x{x}, y{y} {}
    float x;
    float y;
};

struct Velocity 
{
    Velocity() {}
    Velocity(float x, float y) : x{x}, y{y} {}
    float x;
    float y;
};

struct Material
{
    Material() {}
    Material(const char* tex) : texture{tex} {}
    const char* texture = "";
};

static GLFWwindow* sWindow = nullptr;

static void InitContext();

static void WindowKeyCallback(GLFWwindow* window, int key, int mods, int scancode, int action);

int main(int argc, char** argv) 
{
    // Glad and GLFW context creation.
    InitContext();

    // In order to use the Silverback ECS, we need to initialize a Registry
    // instance, which will act as a mediator between us and the underlying logic. 
    // Entties, components and systems will be created from this registry instance.
    slv::Registry registry;

    // Create some entities.
    //
    auto player = registry.CreateEntity();
    auto enemy = registry.CreateEntity();

    // Add some components. Note that the templates here can quite literally
    // be any object that you come up with, so long as the provided arguments
    // can be used to construct them.
    //
    player->Add<Position>(5.0f, 3.0f);
    player->Add<Velocity>(9.0f, 9.0f);  
    player->Add<Material>("Test1.png");

    enemy->Add<Position>(8.0f, 6.0f);
    enemy->Add<Velocity>(10.0f, 15.0f);
    enemy->Add<Material>("Test2.png");

    // Create some systems. The parameter defines which "layer" the system
    // resides in, which will be set to 0 if it's not specified here.
    // The layer construct is a simple a way to ensure that any given system(s)
    // will be run after/before any other system(s).
    //
    auto system1 = registry.CreateSystem<Position>(1);
    auto system2 = registry.CreateSystem<Velocity>(1);
    auto system3 = registry.CreateSystem<Material>(1);

    // Define a function that will be called by registry.RunSystems().
    //
    system1->Action([](const float ts, const std::vector<slv::EntityID>& entities, Position* p) 
    {
        for(int i = 0; i < entities.size(); i++) 
        {
            std::cout << p[i].x << std::endl;
            std::cout << p[i].y << std::endl;
        }
    });

    system2->Action([](const float ts, const std::vector<slv::EntityID>& entities, Velocity* v) 
    {
        for(int i = 0; i < entities.size(); i++) 
        {
            std::cout << v[i].x << std::endl;
            std::cout << v[i].y << std::endl;
        }
    });

    system3->Action([](const float ts, const std::vector<slv::EntityID>& entities, Material* mat) 
    {
        for(int i = 0; i < entities.size(); i++) 
        {
            std::cout << mat[i].texture << std::endl;
        }
    }); 


    // Finally we run each system, which is when the previously defined "Actions" will
    // be called. This is where the aforementioned "layer" construct comes in handy,
    // since we can specfically only run the systems that belong to a specific layer.
    registry.RunSystems(1, 1);


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


