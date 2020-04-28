#include "shader.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

void Shader::compile(const char* vertexPath, const char* fragmentPath) {
    
    std::string vertexCode, fragmentCode;

    try {
        std::ifstream vertexFile(vertexPath), fragmentFile(fragmentPath);
        std::stringstream vertexStream, fragmentStream;

        vertexStream << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();

        vertexFile.close();
        fragmentFile.close();

        vertexCode = vertexStream.str();
        fragmentCode = fragmentStream.str();
    } catch (std::exception e) {
        std::cerr << "Failed to read shader files!\n";
    }

    const GLchar* vertexSource{ vertexCode.c_str() };
    const GLchar* fragmentSource{ fragmentCode.c_str() };

    GLuint vertexID{ glCreateShader(GL_VERTEX_SHADER) };
    GLuint fragmentID{ glCreateShader(GL_FRAGMENT_SHADER) };

    glShaderSource(vertexID, 1, &vertexSource, NULL);
    glCompileShader(vertexID);
    checkCompileErrors(vertexID, false);

    glShaderSource(fragmentID, 1, &fragmentSource, NULL);
    glCompileShader(fragmentID);
    checkCompileErrors(fragmentID, false);

    ID = glCreateProgram();
    glAttachShader(ID, vertexID);
    glAttachShader(ID, fragmentID);

    glLinkProgram(ID);
    checkCompileErrors(ID, true);

    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
}

void Shader::checkCompileErrors(GLuint ID, bool program) {
    GLint success;
    if(program) {
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if(!success) {
            GLchar infoLog[1024];
            glGetProgramInfoLog(ID, 1024, NULL, infoLog);
            std::cout << infoLog;
        }
    } else {
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
        if(!success) {
            GLchar infoLog[1024];
            glGetShaderInfoLog(ID, 1024, NULL, infoLog);
            std::cout << infoLog;
        }
    }
}

void Shader::setMatrix4(const char* name, const glm::mat4 &matrix) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec3(const char* name, const glm::vec3 &vector) {
    glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(vector));
}

void Shader::setInt(const char* name, int value) {
    glUniform1i(glGetUniformLocation(ID, name), value);
}
