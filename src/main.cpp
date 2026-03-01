#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <C:\Users\mason\simple_gl\src\Shader.h>
#include <filesystem>

int main() {
    if (!glfwInit()) {
        std::cerr << "glfwInit failed\n";
        return -1;
    }

    // Inittializes stuff
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creates a window and context
    GLFWwindow* window = glfwCreateWindow(2560, 1440, "simple_gl", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initializes GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }
    // Defines three 3D positions of the triangle
    float verts[] = {
        -0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
        -0.0f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f
    };

    // verts of 2nd triangle. UNUSED
    float verts2[] {
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.0f, 0.0f, 0.0f 
    };

    // Rectangle stuff. UNUSED
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    Shader ourShader("C:/Users/mason/simple_gl/shaders/3.3.shader.vs", "C:/Users/mason/simple_gl/shaders/3.3.shader.fs");
    std::cout << std::filesystem::current_path() << std::endl;

    GLuint vao, vbo, ebo, vao2, vbo2;
    // Creates VAO, stores the vertex attribute config
    glGenVertexArrays(1, &vao);
    // generates vao 2 for triangle 2
    // glGenVertexArrays(1,&vao2);

    // Creates VBO, GPU memory to store the vertex data
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // creates VBO and memory for VBO
    // glGenBuffers(1, &vbo2);

    // all uploads verts to GPU
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    // says: attribute index 0 reads 3 floats per-vertex from the currently bound VBO, tightly packed.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    
    // triangle 2. Here vao2 is bounded, so everything under here is bound to vao2
    // glBindVertexArray(vao2);

    // "this is the vbo im refering to"
    // glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    // initializes memory and stores the data in that memory
    // glBufferData(GL_ARRAY_BUFFER, sizeof(verts2), verts2, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f,0.1f,0.12f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    //glDeleteBuffers(1, &vbo2);
    // glDeleteVertexArrays(1, &vao2);    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}