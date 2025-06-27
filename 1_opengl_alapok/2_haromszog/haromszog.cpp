// https://learnopengl.com/Getting-started/Hello-Triangle

// forditas: g++ program.cpp -lglfw -lGL

#define GL_GLEXT_PROTOTYPES
#include <iostream>
#include <GL/glcorearb.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// shader forrasok
// kesobb ki lehet oket szervezni fajlokba
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
    "}\0";
const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

int main() {
    int default_window_width = 600;
    int default_window_height = 800;

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

    // -------------------------------------------
    // uj resz: shaderek

    // ezek kellenek a shader forditas ellenorzesehez
    int success;
    char infoLog[512];

    // vertex shader
    // haromszogekre fut le
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // ha nem sikerult a forditas, kiirjuk
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "vertex shader compilation failed: " << infoLog << std::endl;
    }

    // fragment shader
    // pixelekre fut le
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "fragment shader compilation failed: " << infoLog << std::endl;
    }

    // shaderek kombinalasa
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "shader program linking failed" << infoLog << std::endl;
    }
    // mivel a shaderek hozza lettek adva a shader programhoz, igy mar torolhetok
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // -----------------------------------------
    // haromszoges resz ide

    // haromszog pontjai
    // a 3. koordinata 0, mivel sikban vagyunk
    float vertices[] = {
        -0.5, -0.5, 0.0,
        0.5, 0.5, 0.0,
        0.0, 0.5, 0.0
    };

    // vertex array object
    unsigned int VAO;
    // vertex buffer object
    unsigned int VBO;

    // todo: ez micsoda pontosan?
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // buffer letrehozasa
    glGenBuffers(1, &VBO);
    // buffer tipusa
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // adatok bufferbe masolasa
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // ezzel mondjuk meg az opengl-nek, hogy a vertex shader hogyan ertelmezze a bufferben levo adatokat
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // volt a kodban, de ha kiveszem, akkor is mukodik
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // rendereles
        // ...
        glClearColor(0.1, 0.15, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        // haromszog rajzolasa
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // kilepes elott osszetakaritja a glfw-s cuccokat
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
