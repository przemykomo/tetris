#ifndef MODEL_HEADER
#define MODEL_HEADER
#include <glad/glad.h>
#include "shader.hpp"

class Model {
    public:
    Model() = default;
    ~Model();

    void init(float vertices[], unsigned int triangles);
    void bind();
    void draw();
    
    private:
    GLuint VAO;
    GLuint VBO;

    unsigned int m_triangles;
};

#endif
