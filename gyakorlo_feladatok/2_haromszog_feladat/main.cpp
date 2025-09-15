#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow *window);

int defaultWindowWidth = 800;
int defaultWindowHeight = 600;

// A shaderek olyan programok, amik a GPU-n futnak.
// A vertex shader minden háromszögre lefut
// Míg a fragment shader minden pixelre

// A shadder forráskódját egy karaktertömbben tároljuk
// (C++ 11 óta van raw string literal, amivel lehet többsoros szöveget megadni R"(szoveg)" formátumban)
// Később természetesen ezeket lehet külön fájlból is betölteni
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
	# FELADAT: az aPos változóban kapjuk meg a vertex koordinátáit
	# Viszont a gl_Position vec4-et vár
	# legyen a 4. koordináta 1.0, a többit az aPosból szedjük ki
    #gl_Position =
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main()
{
	# Feladat: Egy 4 elemű vektorban adhatunk neki RGBA színt
    #FragColor =
}
)";

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(defaultWindowWidth, defaultWindowHeight, "Haromszog kirajzolasa", NULL, NULL);
    glfwMakeContextCurrent(window);

    glViewport(0, 0, defaultWindowWidth, defaultWindowHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Új rész

    // Shaderek létrehozása

    // Shader azonosító
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Shader kód csatolása a shaderhez
    // Paraméterek:
    // 1. Shader azonosító, amihez hozzá lesz rendelve
    // 2. Darabszám (most csak 1 shadert akarunk)
    // 3. Shader kódja
    // 4. Hány bájt hosszú (NULL-on hagyva nullterminált sztringet feltételez)
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Na most ugyanazt a fragment shaderrel is
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // A shadereket ezután összekombináljuk egy shader programba
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //
    glUseProgram(shaderProgram);

    // Ha linkeltük a shadereket, törölhetők
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    // Háromszög csúcspontjai
    // A rajzterületet egy -1-től 1-ig tartó Descartes-féle koordinátarendszerként képzelhetjük el
    // A (0;0)-s pont a középpont
    // 3 koordinátát kell megadnunk, de mivel 2 dimenzióban rajzolunk még, így a Z 0 lesz
    float vertices[] = {
    	//  X    Y    Z
    	-0.5f, -0.5f, 0.0f,
     	0.5f, -0.5f, 0.0f,
       	0.0f,  0.5f, 0.0f
    };

    // Az adatok valamilyen módon fel kell tölteni a GPU memóriájába
    // Mert onnan sokkal gyorsabban tudja kirajzolni
    // Erre buffereket használunk
    // A Vertex Buffer Objectben a vertexek adatai szoktak lenni
    // A buffer azonosítója lesz
    unsigned int VBO;

    // TODO: mit is csinál a VAO?
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // VBO hozzárendelése az azonosítóhoz
    glGenBuffers(1, &VBO);

    // TODO: VAO?
    glBindVertexArray(VAO);

    // VBO aktiválása
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Adatok VBO-ba másolása
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Ezzel mondjuk meg az OpenLG-nek, hogy hogyan értelmezze a vertex adatokat
    // Paraméterek:
    // 1. Vertex attribútum indexe
    // 2. Hány darab
    // 3. Típus
    // 4. Normalizált-e
    // 5. Lépésköz 2 vertex adatai között (bájtban, tipp: sizeof())
    // 6. Offset, hogy milyen pozíción van az adat a bufferben
    // void* típust vár, ezért típust kell kényszeríteni
    glVertexAttribPointer(/*FELADAT*/);
    glEnableVertexAttribArray(0);

    // VBO unbindelése
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VAO unbindelése
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
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
