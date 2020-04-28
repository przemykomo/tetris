#include "model.hpp"

void Model::init(float vertices[], unsigned int triangles) {
    m_triangles = triangles;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, triangles * 3 * 2 * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
}

Model::~Model() {
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
}

void Model::bind() {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void Model::draw() {
    glDrawArrays(GL_TRIANGLES, 0, m_triangles * 3);
}
