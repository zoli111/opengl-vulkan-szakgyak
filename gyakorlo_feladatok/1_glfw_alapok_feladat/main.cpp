// A kiegészítendő részek FELADAT-ként vannak jelezve
// Használd az IDE keresés funkcióját

// https://learnopengl.com/Getting-started/OpenGL
// ^ Ezen alapuló feladatok lesznek, innen érdemes lehet lesni

// FONTOS: még a glfw inclue-olása előtt kell definiálni, mert különben sok használt függvény nem lenne elérhető
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

// Ezt a függvényt hívjuk meg, ha átméretezzük az ablakot
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Ebben a függvényben dolgozzuk fel a bemenetet például a billentyűzetről
void processInput(GLFWwindow *window);

int defaultWindowWidth = 800;
int defaultWindowHeight = 600;

int main() {

	// A glfw függvénykönyvárat inicializálni kell, mielőtt bármilyen glfw-s függvényt használnánk
    glfwInit();
    // Megmondjuk, hogy a 3.3-as OpenGL verziót használja
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // OpenGL Core Profile használata, a kurzus alatt ezt használjuk
    // Ez lehetővé teszi, hogy a modernebb, gyorsabb funkciókat kihasználjuk
    // Cserébe kicsit bonyolultabb használni, és nem rejt el semmit a működéséből
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Ablak létrehozása
    // Az elő három paraméter a szélesség, magasság, és a cím
    GLFWwindow* window = glfwCreateWindow(defaultWindowWidth, defaultWindowHeight, "Ez egy ablak", NULL, NULL);
    // Kiválasztjuk, hogy ezt az ablakot fogjuk használni
    glfwMakeContextCurrent(window);

    // Ez kvázi a rajzterület
    // A paraméterek mondják meg, hogy az ablakon belül hova kerüljön
    // Jelenleg kitölti az ablakot, mert (0;0)-ás pozícióban van a bal felső sarka, és (800;600)-nál a jobb alsó
    glViewport(0, 0, defaultWindowWidth, defaultWindowHeight);
    // Ez a függvény hívódik meg, ha átméretezzük az ablakot
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Van egy függvény, a glfwWindowShouldClose(), aminek egy paramétere van: az adott ablak
    // Fusson ez a ciklus, amíg hamisat ad vissza
    while (/*Itt egészítsd ki*/) {
    	// Bemenetek feldolgozása
     	// Lásd lentebb
        processInput(window);

        // Kitöltési szín beállítása
        // 4 float paramétere van: RGBA, a színek intenzitása a [0;1] intervallumon belül mozoghat, illetve az alfa legyen 1
        glClearColor(/*FELADAT 1*/);
        // Kitöltés
        glClear(GL_COLOR_BUFFER_BIT);

        // Glfw események lekérdezése
        glfwPollEvents();
        // Végleges kirajzolás
        glfwSwapBuffers(window);
    }

    // a glfwInit() párja
    glfwTerminate();
    return 0;
}

// Az ablak átméretezésekor meghívott függvény
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// Vajon melyik függvény kell, amivel a rajzterületet átméretezzük, hogy illeszkedjen az ablakhoz?
	// FELADAT 2
}

void processInput(GLFWwindow * window) {
	// Escape lenyomására zárjuk be a programot
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

// Ha kész vagy, lefordíthatod ezzel a paranccsal:
// `g++ main.cpp -o program -lGL -lglfw`
// Elvileg most kaptál egy "program" nevű futtatható fájlt

// Hogy ne kelljen mindig ezekre a paraméterekre emlékezni, ez automatizálható
// Most olvasd el a CMakeLists.txt fájlt
