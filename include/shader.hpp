#ifndef H_SHADER
#define H_SHADER
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Shader {
    public:
        void compile(const char* vertexPath, const char* fragmentPath);
        void setMatrix4(const char* name, const glm::mat4 &matrix);
        void setVec3(const char* name, const glm::vec3 &vector);
        void setInt(const char* name, int value);

        inline void use() {
            glUseProgram(ID);
        }

    private:
        GLuint ID;
        void checkCompileErrors(GLuint ID, bool program);
};

#endif //H_SHADER
