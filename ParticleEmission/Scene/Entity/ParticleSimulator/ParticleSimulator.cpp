#include "ParticleSimulator.h"

ParticleSimulator::ParticleSimulator(int particleCount) : Entity("ParticleEmission/Shader/ParticlesSimulator.vert", "ParticleEmission/Shader/ParticlesSimulator.frag") {
    particles.resize(particleCount);

    // Init the particles as a cube
    for (int i = 0; i < particles.size(); i++) {
        particles[i].position = glm::vec3(
            (float)(rand() % 100) / 100.0f * 2.0f - 1.0f,
            (float)(rand() % 100) / 100.0f * 2.0f - 1.0f,
            (float)(rand() % 100) / 100.0f * 2.0f - 1.0f
        );
    }

    // Init the SSBO
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Particle) * particles.size(), particles.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    // Define the layout of the SSBO
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Init the VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, ssbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, velocity));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Init the VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * particles.size(), particles.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

ParticleSimulator::~ParticleSimulator() {
    glDeleteBuffers(1, &ssbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void ParticleSimulator::update(float deltaTime) {
    // Update the SSBO
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Particle) * particles.size(), particles.data());
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Update the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Particle) * particles.size(), particles.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSimulator::render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) {
    // Bind the shader
    shader.use();

    // Bind the VAO
    glBindVertexArray(vao);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Bind the SSBO
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

    // Set the uniforms
    shader.setMat4("u_view", cameraViewMatrix);
    shader.setMat4("u_projection", cameraProjectionMatrix);

    // Draw the particles
    glDrawArrays(GL_POINTS, 0, particles.size());

    // Unbind the SSBO
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind the VAO
    glBindVertexArray(0);

//    // Unbind the shader
//    shader->unuse();
}
