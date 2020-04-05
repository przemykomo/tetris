#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

constexpr int WIDTH{640};
constexpr int HEIGHT{480};

int main() {
    if(!glfwInit()) {
        std::cout << "Can't initialize GLFW!\n";
        return -1;
    }

    GLFWwindow* window{ glfwCreateWindow(WIDTH, HEIGHT, "Opengl sample", NULL, NULL) };
    if(!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Can't initialize GLAD!\n";
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
