#include "Cube.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

Cube::Cube() : Entity("shaders/transform.vert", "shaders/transform.frag") {
    create();
    position = glm::vec3(-1.0F, 0.0F, 0.0F);
    updateModelMatrix();
}

void Cube::create() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

Cube::~Cube() {
    destroy();
}

void Cube::destroy() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Cube::update(float deltaTime) {
    // No component to update
}

void Cube::render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) {
    // Shader
    shader.use();
    shader.setMat4("u_view", cameraViewMatrix);
    shader.setMat4("u_projection", cameraProjectionMatrix);
    shader.setMat4("u_model", modelMatrix);

    // Draw
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());
}
