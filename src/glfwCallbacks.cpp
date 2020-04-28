#include "glfwCallbacks.hpp"

namespace callbacks {
    void framebufferSize(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
}
