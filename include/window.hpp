#ifndef H_WINDOW
#define H_WINDOW
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include "model.hpp"
#include "texture.hpp"

class Window {
    public:
    Window(int width, int height, const char* title);
    void loop();
    ~Window();

    private:
    GLFWwindow* glfwWindow;

    Model squareModel;
    Texture blockTexture;
    Shader blockShader;

    Model backgroundModel;
    Texture backgroundTexture;
    Shader backgroundShader;
};
#endif
