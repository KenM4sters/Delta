#ifndef SILVERBACK_DEMO_INCLUDES_HPP
#define SILVERBACK_DEMO_INCLUDES_HPP

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Silverback.hpp"

#include <chrono>

struct Transform 
{
    Transform() {}
    Transform(glm::vec3 pos, glm::vec3 scale) : position{pos}, scale{scale} {}

    glm::vec3 position;
    glm::vec3 scale;
}; 

struct Material
{
    Material() {}
    Material(glm::vec3 color) : color{color} {}

    glm::vec3 color;
};

struct Camera 
{
    Camera() {}
    Camera(glm::mat4 projection) : projection{projection} {}

    glm::mat4 projection;
};

struct Renderable 
{
    Renderable() {}
    Renderable(GLuint vao, GLuint shader) 
        : vao{vao}, shader{shader} {}

    GLuint vao;
    GLuint shader;
};

void InitContext();
void WindowKeyCallback(GLFWwindow* window, int key, int mods, int scancode, int action);
void CreateShader(GLuint* shaderProgram, const char* vertSrc, const char* fragSrc);
void CheckCompileErrors(GLuint object, const char* type); 
void CreateVertexBuffer(GLuint* vbo, GLuint* vao);

#endif