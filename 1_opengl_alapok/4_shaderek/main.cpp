// https://https://learnopengl.com/Getting-started/Shaders

#define GL_GLEXT_PROTOTYPES
#include <GL/glcorearb.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const int default_window_width = 600;
const int default_window_height = 800;

// a working direcory-ban kell lennie az adott fajloknak
const char* vertex_path = "vertex.glsl";
const char* fragment_path = "fragment.glsl";

int main() {

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);

    // force xorg
    #if (GLFW_VERSION_MAJOR >= 3) && (GLFW_VERSION_MINOR >= 4)
        if (glfwPlatformSupported(GLFW_PLATFORM_X11)) {
            glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
        }
    #endif

    GLFWwindow* window = glfwCreateWindow(default_window_width, default_window_height, "Haromszog", NULL, NULL);
    if (window == NULL) {
        return -1;
    }
    glfwMakeContextCurrent(window);
    glViewport(0, 0, default_window_width, default_window_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // shader letrehozasa
    Shader shader = Shader(vertex_path, fragment_path);

    float vertices[] = {
        -0.5, -0.5, 0.0,
        0.5, 0.5, 0.0,
        0.0, 0.5, 0.0
    };

    unsigned int VAO;

    unsigned int VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.1, 0.15, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // es csak ennyi hasznalni a shadert
        shader.use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow * window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
