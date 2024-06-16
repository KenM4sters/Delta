#include "Includes.hpp"


static GLFWwindow* sWindow = nullptr;

static constexpr float cubeVertices[288] {
      // back
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
      // front
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
      // left
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
      // right
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
      // bottom
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
      // top
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

int main(int argc, char** argv) 
{
    // Glad and GLFW context creation.
    InitContext();

    // Initialize a Registry instance.
    slv::Registry registry;

    // Camera
    glm::mat4 projection = glm::perspective(45.0f, 800/600.0f, 0.0f, 100.0f);

    // Create some entities.
    std::unique_ptr<slv::Entity> boxes[25];

    // Create a single vertex buffer for the boxes.
    GLuint vbo, vao;
    CreateVertexBuffer(&vbo, &vao);

    // Shader
    GLuint shaderProgram;

    const char* vertSrc = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoords;
        layout (location = 2) in vec3 aNormal;
        uniform mat4 uModel;
        uniform mat4 uProjection;
        void main() 
        {
            gl_Position = uProjection * uModel * vec4(aPos, 1.0);
        }
    )";

    const char* fragSrc = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec3 uColor;
        void main() 
        {
            FragColor = vec4(uColor, 1.0);
        }
    )";


    CreateShader(&shaderProgram, vertSrc, fragSrc);


    for(size_t i = 0; i < sizeof(boxes)/sizeof(boxes[0]); i++) 
    {
        boxes[i] = registry.CreateEntity();

        glm::vec3 positions[25];

        for(size_t j = 0; j < 5; j++) 
        {   
            for(size_t k = 0; k < 5; k++) 
            {
                float displacement = 1 / 5;
                float x = displacement * j;
                float y = displacement * k;
                positions[j + k] = glm::vec3(x, y, 0.0f); 
            }
        }


        // Add some components. 
        //
        boxes[i]->Add<Transform>(positions[i], glm::vec3(0.1f, 0.1f, 1.0f));
        boxes[i]->Add<Material>(glm::vec3(1.0f, 0.5f, 0.0f));
        boxes[i]->Add<Camera>(projection);
        boxes[i]->Add<Renderable>(vao, shaderProgram);
    }



    // Create some systems.
    auto renderer = registry.CreateSystem<Renderable, Transform, Material, Camera>(0);


    // Defines callback functions for each system.
    renderer->Action([](const float ts, const std::vector<slv::EntityID>& entities, Renderable* r, Transform* t, Material* m, Camera* c) 
    {
        for(size_t i = 0; i < entities.size(); i++) 
        {
            glm::mat4 modelMatrix{1.0f};
            modelMatrix = glm::scale(modelMatrix, t->scale);
            modelMatrix = glm::translate(modelMatrix, t->position);

            glm::mat4 projectionMatrix = c->projection;

            glUseProgram(r->shader);
            glUniform3fv(glGetUniformLocation(r->shader, "uColor"), 1, &m->color[0]);
            glUniformMatrix4fv(glGetUniformLocation(r->shader, "uModel"), 1, GL_FALSE, &modelMatrix[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(r->shader, "uProjection"), 1, GL_FALSE, &projectionMatrix[0][0]);

            glBindVertexArray(r->vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindVertexArray(0);
            glUseProgram(0);
        }
    });



    auto previousFrame = std::chrono::high_resolution_clock::now();
    while(!glfwWindowShouldClose(sWindow)) 
    {
        glClear(GL_COLOR_BUFFER_BIT); // Clears each pixel at the beginning of each frame.
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Sets the background color.

        // Calculate the time step for each frame.
        //
        auto currentFrame = std::chrono::high_resolution_clock::now();
        auto timeStep = std::chrono::duration<double>(currentFrame - previousFrame).count();


        // Run each system, which is when the previously defined "Actions" will
        // be called. This is where the aforementioned "layer" construct comes in handy,
        // since we can specfically only run the systems that belong to a specific layer.
        registry.RunSystems(0, timeStep); // layer 0, the default.


        glfwPollEvents(); // Listens to events (keyboard, mouse etc...)
        glfwSwapBuffers(sWindow); // Manages the swapchain.
    }

    glfwDestroyWindow(sWindow);
    glDeleteShader(shaderProgram);
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

void CreateShader(GLuint* shaderProgram, const char* vertSrc, const char* fragSrc) 
{
    GLuint s_vertex, s_fragment;

    s_vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(s_vertex, 1, &vertSrc, NULL);
    glCompileShader(s_vertex);
    CheckCompileErrors(s_vertex, "VERTEX");

    s_fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(s_fragment, 1, &fragSrc, NULL);
    glCompileShader(s_fragment);
    CheckCompileErrors(s_fragment, "FRAGMENT");

    *shaderProgram = glCreateProgram();
    glAttachShader(*shaderProgram, s_vertex);
    glAttachShader(*shaderProgram, s_fragment);
    glLinkProgram(*shaderProgram);
    CheckCompileErrors(*shaderProgram, "PROGRAM");

    // All shaders can be deleted now, since we've already linked them to our shader program.
    glDeleteShader(s_vertex);
    glDeleteShader(s_fragment);
}

void CreateVertexBuffer(GLuint* vbo, GLuint* vao) 
{
    glGenVertexArrays(1, vao);
    glGenBuffers(1, vbo);
    
    glBindVertexArray(*vao);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(5*sizeof(float)));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void CheckCompileErrors(GLuint object, const char* type)
{
    int success;
    char infoLog[1024];
    if(strcmp(type, "PROGRAM"))
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}