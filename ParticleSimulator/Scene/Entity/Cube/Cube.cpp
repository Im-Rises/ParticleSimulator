#include "Cube.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

const std::array<float, 108> Cube::Vertices = {
    -0.5F,
    -0.5F,
    -0.5F,
    0.5F,
    -0.5F,
    -0.5F,
    0.5F,
    0.5F,
    -0.5F,
    0.5F,
    0.5F,
    -0.5F,
    -0.5F,
    0.5F,
    -0.5F,
    -0.5F,
    -0.5F,
    -0.5F,

    -0.5F,
    -0.5F,
    0.5F,
    0.5F,
    -0.5F,
    0.5F,
    0.5F,
    0.5F,
    0.5F,
    0.5F,
    0.5F,
    0.5F,
    -0.5F,
    0.5F,
    0.5F,
    -0.5F,
    -0.5F,
    0.5F,

    -0.5F,
    0.5F,
    0.5F,
    -0.5F,
    0.5F,
    -0.5F,
    -0.5F,
    -0.5F,
    -0.5F,
    -0.5F,
    -0.5F,
    -0.5F,
    -0.5F,
    -0.5F,
    0.5F,
    -0.5F,
    0.5F,
    0.5F,

    0.5F,
    0.5F,
    0.5F,
    0.5F,
    0.5F,
    -0.5F,
    0.5F,
    -0.5F,
    -0.5F,
    0.5F,
    -0.5F,
    -0.5F,
    0.5F,
    -0.5F,
    0.5F,
    0.5F,
    0.5F,
    0.5F,

    -0.5F,
    -0.5F,
    -0.5F,
    0.5F,
    -0.5F,
    -0.5F,
    0.5F,
    -0.5F,
    0.5F,
    0.5F,
    -0.5F,
    0.5F,
    -0.5F,
    -0.5F,
    0.5F,
    -0.5F,
    -0.5F,
    -0.5F,

    -0.5F,
    0.5F,
    -0.5F,
    0.5F,
    0.5F,
    -0.5F,
    0.5F,
    0.5F,
    0.5F,
    0.5F,
    0.5F,
    0.5F,
    -0.5F,
    0.5F,
    0.5F,
    -0.5F,
    0.5F,
    -0.5F,
};


const char* const Cube::VertexShaderSource =
    R"(#version 300 es

        precision highp float;

        layout (location = 0) in vec3 a_vertex;

        uniform mat4 u_mvp;

        void main()
        {
            gl_Position = u_mvp * vec4(a_vertex, 1.0);
        }
)";

const char* const Cube::FragmentShaderSource =
    R"(#version 300 es

        precision highp float;

        out vec4 o_fragColor;

        void main()
        {
            o_fragColor = vec4(1.0, 0.5, 0.2, 1.0);
        }
)";

Cube::Cube() : shader(VertexShaderSource, FragmentShaderSource, false) {
    // Set initial position
    updateModelMatrix();

    // Create VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create VBO and bind it
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copy vertices to VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Vertices.size(), Vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Cube::~Cube() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Cube::update(const float& deltaTime) {
    // No component to update
}

void Cube::render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) {
    glBindVertexArray(VAO);

    shader.use();
    shader.setMat4("u_mvp", cameraProjectionMatrix * cameraViewMatrix * modelMatrix);

    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(Vertices.size()));

    glBindVertexArray(0);
}
