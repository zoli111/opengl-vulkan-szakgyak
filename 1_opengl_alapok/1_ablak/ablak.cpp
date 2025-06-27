// https://learnopengl.com/Getting-started/OpenGL

// forditas: g++ program.cpp -lglfw -lGL

// glfw lesz a fo library, amit openglhez hasznalni fokunk
#include <GLFW/glfw3.h>

// ablak atmeretezesenel ez a fuggveny lesz meghivva, hogy a viewportot atmeretezze
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// input kezeles, ebben az esetben bezarja az ablakot esc lenyomasakor
void processInput(GLFWwindow *window);

int main() {
    int default_window_width = 600;
    int default_window_height = 800;

    // inicializalas
    glfwInit();
    // par parameter beallitasa: verzio, core profile hasznalata
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

    // ablak letrehozasa, fontos parameterek: szelesseg, magassag, cim, a tobbi lehet most NULL
    GLFWwindow* window = glfwCreateWindow(default_window_width, default_window_height, "Tesztablak (press space)", NULL, NULL);
    // kilepes, ha nem sikerult letrehozni
    if (window == NULL) {
        return -1;
    }
    // ablak hozzarendelese az opengl contexthez
    glfwMakeContextCurrent(window);

    // erre fogunk renderelni
    glViewport(0, 0, default_window_width, default_window_height);

    // mivel az ablak atmeretezheto, igy ha atmeretezzuk, ez a fuggveny automatikusan meg lesz hivva,
    // hogy hozzaigazitsa a viewportot
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // render loop
    // amig fut az alkalmazas minden frame-et ez rajzol ki
    while (!glfwWindowShouldClose(window)) {
        // input feldolgozas
        processInput(window);

        // itt megy vegbe a rendereles
        // a pelda szerint szinezzuk ki az ablakot
        // tegyunk bele egy csavart:
        // legyen piros, ha le van nyomva a szokoz, egyebkent kek
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            glClearColor(0.8, 0.2, 0.2, 1.0);
        } else {
            glClearColor(0.2, 0.2, 0.8, 1.0);
        }

        glClear(GL_COLOR_BUFFER_BIT);

        // kulso esemenyek feldolgozasa
        glfwPollEvents();
        // ujrarajzolas
        glfwSwapBuffers(window);
    }

    // kilepes elott osszetakaritja a glfw-s cuccokat
    glfwTerminate();
    return 0;
}

// viewport hozzaigazitasa az ablakmerethez, lasd deklaracio
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// ablak bezarasa esc-re
void processInput(GLFWwindow * window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
