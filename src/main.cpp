#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "Shader.h"
#include <filesystem>
#include "stb_image.h"
#include "imageLoad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"camera.h"
#include"particle.h"
#include<vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void drawParticle(const Particle&, const Shader&);
float randFloat(float min, float max);

const unsigned int SCR_WIDTH = 2560;
const unsigned int SCR_HEIGHT = 1440;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "simple_gl", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    glEnable(GL_DEPTH_TEST);

    // cube
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
    };


    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    Shader ourShader("shaders/3.3.shader.vs", "shaders/3.3.shader.fs");

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // says: attribute index 0 reads 3 floats per-vertex from the currently bound VBO, tightly packed.
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Render loop
    std::vector<Particle> particles;
    int numParticles = 10;
    Particle p;
    p.acceleration = glm::vec3(0.0f, -9.8f, 0.0f);
    p.position = glm::vec3(0.0f, 0.0f, 0.0f);

    // create multiple particle objects
    for (int i = 0; i < numParticles; i++) {
        particles.push_back(Particle());
    }

    for (auto& p : particles) {
        p.acceleration = glm::vec3(0.0f, -9.8f, 0.0f);
        p.position = glm::vec3(randFloat(-1, 1), randFloat(-1, 1), randFloat(-1, 1));
    }

    while (!glfwWindowShouldClose(window)) {

        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            p.position = glm::vec3(0.0f, 0.0f, 0.0f);
            p.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
            for (auto& p : particles) {
                p.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
                // may want to find a way later to store initial values for each and
                // reset them to that
                p.position = glm::vec3(randFloat(-1, 1), randFloat(-1, 1), randFloat(-1, 1));
            }
        }

        p.updateParticle(p, deltaTime);
        p.velocity *= 0.999f;

        for (auto& p2 : particles) {
            p2.updateParticle(p2, deltaTime);
            p2.velocity *= 0.999f;
        }

        glClearColor(0.1f,0.1f,0.12f,1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ourShader.use();
    
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        glBindVertexArray(vao);        
        // draw the container box
        glm::mat4 boxModel = glm::mat4(1.0f);
        boxModel = glm::scale(boxModel, glm::vec3(2.0f)); // matches bounds [-1, 1]
        ourShader.setMat4("model", boxModel);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        for (auto& p : particles) {
            drawParticle(p, ourShader);
        }

        drawParticle(p, ourShader);
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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void drawParticle(const Particle& p, const Shader& s) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, p.position);
    model = glm::scale(model, glm::vec3(0.05f));

    s.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

// random float func for randomized position of particles
float randFloat(float min, float max) {
    return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
}