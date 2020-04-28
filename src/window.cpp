#include "window.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <thread>
#include <chrono>
#include <cstdlib>
#include <iostream>

const glm::mat4 c_model{ glm::scale(glm::identity<glm::mat4>(), glm::vec3(30.0f)) };

inline void setBlockPos(Shader &shader, float x, float y) {
    glm::mat4 model = glm::translate(c_model, glm::vec3(x, y, 0.0f));
    shader.setMatrix4("model", model);
}

Window::Window(int width, int height, const char* title) {
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
    if(!glfwWindow) {
        glfwTerminate();
    }

    glfwMakeContextCurrent(glfwWindow);
    //callback
    glfwSwapInterval(1);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glViewport(0, 0, width, height);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    backgroundShader.compile("assets/backgroundVertex.glsl", "assets/backgroundFragment.glsl");

    {
        float vertices[]{
            -1.0f, -1.0f,
            +1.0f, -1.0f,
            -1.0f, +1.0f,
            +1.0f, -1.0f,
            -1.0f, +1.0f,
            +1.0f, +1.0f
        };

        backgroundModel.init(vertices, 2);
    }

    blockShader.compile("assets/vertex.glsl", "assets/fragment.glsl");
    blockShader.use();

    {
        float vertices[]{
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 1.0f
        };

        squareModel.init(vertices, 2);
    }

    blockShader.setMatrix4("model", c_model);

    glm::mat4 projection{ glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f) };
    blockShader.setMatrix4("projection", projection);

    glActiveTexture(GL_TEXTURE0);
    backgroundTexture.load("assets/background.png");
    backgroundTexture.bind();
    backgroundShader.setInt("tex", 0);

    glActiveTexture(GL_TEXTURE1);
    blockTexture.load("assets/block.png");
    blockTexture.bind();
    blockShader.setInt("tex", 1);
}

struct Point {int x, y;} a[4], b[4];

constexpr int fieldHeight{20};
constexpr int fieldWidth{10};
int field[fieldHeight][fieldWidth]{0};

int figures[7][4]{
    1, 3, 5, 7, // I
    2, 4, 5, 7, // Z
    3, 5, 4, 6, // S
    3, 5, 4, 7, // T
    2, 3, 5, 7, // L
    3, 5, 7, 6, // J
    2, 3, 4, 5  // O
};

inline bool doesCollide() {
    for(int i{0}; i < 4; ++i) {
        if(a[i].x < 0 || a[i].x >= fieldWidth || a[i].y >= fieldHeight) return true;
        if(field[a[i].y][a[i].x]) return true;
    }
    return false;
}

double lastMovementUpdate{};
double lastFallUpdate{};

int currentFigure{1};

void Window::loop() {

    for(int i{0}; i < 4; ++i) {
        a[i].x = figures[currentFigure-1][i] % 2;
        a[i].y = figures[currentFigure-1][i] / 2;
    }
    blockShader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
    
    bool renderUpdate{true};
    
    while(!glfwWindowShouldClose(glfwWindow)) {
        double currentFrame{ glfwGetTime() };

        if(currentFrame - lastMovementUpdate > 0.1) {
            lastMovementUpdate = currentFrame;
            // Movement
            int dx{0};
            if(glfwGetKey(glfwWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                dx = 1;
            } else if(glfwGetKey(glfwWindow, GLFW_KEY_LEFT) == GLFW_PRESS) {
                dx = -1;
            }

            if(dx) {
                for(int i{0}; i < 4; ++i) {
                    b[i] = a[i];
                    a[i].x += dx;
                }
                if(doesCollide()) {
                    for(int i{0}; i < 4; ++i) {
                        a[i] = b[i];
                    }
                }
            }

            // Rotation
            if(glfwGetKey(glfwWindow, GLFW_KEY_UP) == GLFW_PRESS) {
                Point center{ a[1] };
                for(int i{0}; i < 4; ++i) {
                    b[i] = a[i];
                    int x{ a[i].y - center.y };
                    int y{ a[i].x - center.x };
                    a[i].x = center.x - x;
                    a[i].y = center.y + y;
                }
                if(doesCollide()) {
                    for(int i{0}; i < 4; ++i) {
                        a[i] = b[i];
                    }
                }
            }


            renderUpdate = true;
        }

        if(currentFrame - lastFallUpdate > 0.3 || (glfwGetKey(glfwWindow, GLFW_KEY_DOWN) == GLFW_PRESS && currentFrame - lastFallUpdate > 0.1)) {
            lastFallUpdate = currentFrame;
            
            for(int i{0}; i < 4; ++i) {
                b[i] = a[i];
                a[i].y += 1;
            }

            if(doesCollide()) {
                for(int i{0}; i < 4; ++i) {
                    field[b[i].y][b[i].x] = currentFigure;
                    a[i].x = figures[currentFigure-1][i] % 2 + 5;
                    a[i].y = figures[currentFigure-1][i] / 2;
                }

                currentFigure = std::rand() % 7 + 1;
                switch(currentFigure) {
                }
            }

            renderUpdate = true;
        }

        if(renderUpdate) {
            renderUpdate = false;

            backgroundShader.use();
            backgroundModel.bind();
            backgroundModel.draw();

            blockShader.use();
            squareModel.bind();

            for(int i{0}; i < fieldHeight; ++i) {
                for(int j{0}; j < fieldWidth; ++j) {
                    if(field[i][j]) {
                        switch(field[i][j]) {
                            case 1:
                                blockShader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
                                break;
                            case 2:
                                blockShader.setVec3("color", glm::vec3(0.6f, 0.6f, 0.6f));
                                break;
                            case 3:
                                blockShader.setVec3("color", glm::vec3(0.0f, 1.0f, 1.0f));
                                break;
                            case 4:
                                blockShader.setVec3("color", glm::vec3(1.0f, 1.0f, 0.0f));
                                break;
                            case 5:
                                blockShader.setVec3("color", glm::vec3(1.0f, 0.0f, 1.0f));
                                break;
                            case 6:
                                blockShader.setVec3("color", glm::vec3(0.0f, 0.0f, 1.0f));
                                break;
                            case 7:
                                blockShader.setVec3("color", glm::vec3(0.0f, 1.0f, 0.0f));
                                break;
                        }
                        setBlockPos(blockShader, j, i);
                        squareModel.draw();
                    }
                }
            }

            for(int i{0}; i < 4; ++i) {
                switch(currentFigure) {
                    case 1:
                        blockShader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
                        break;
                    case 2:
                        blockShader.setVec3("color", glm::vec3(0.6f, 0.6f, 0.6f));
                        break;
                    case 3:
                        blockShader.setVec3("color", glm::vec3(0.0f, 1.0f, 1.0f));
                        break;
                    case 4:
                        blockShader.setVec3("color", glm::vec3(1.0f, 1.0f, 0.0f));
                        break;
                    case 5:
                        blockShader.setVec3("color", glm::vec3(1.0f, 0.0f, 1.0f));
                        break;
                    case 6:
                        blockShader.setVec3("color", glm::vec3(0.0f, 0.0f, 1.0f));
                        break;
                    case 7:
                        blockShader.setVec3("color", glm::vec3(0.0f, 1.0f, 0.0f));
                        break;
                }
                setBlockPos(blockShader, a[i].x, a[i].y);
                squareModel.draw();
            }

            glfwSwapBuffers(glfwWindow);
        }
        glfwPollEvents();
    }
}

Window::~Window() {
    glfwTerminate();
}
