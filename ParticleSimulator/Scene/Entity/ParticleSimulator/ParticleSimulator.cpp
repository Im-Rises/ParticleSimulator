#include "ParticleSimulator.h"

ParticleSimulator::ParticleSimulator(int particleCount) : Entity("shaders/ParticleSimulator.vert", "shaders/ParticleSimulator.frag") {
    particles.resize(particleCount);

    // Init the particles as a cube
    for (int i = 0; i < particles.size(); i++)
    {
        particles[i].position = glm::vec3(
            (float)(rand() % 100) / 100.0f * 2.0f - 1.0f,
            (float)(rand() % 100) / 100.0f * 2.0f - 1.0f,
            0);
    }

    // Generate the SSBO
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

ParticleSimulator::~ParticleSimulator() {
    glDeleteBuffers(1, &ssbo);
}

void ParticleSimulator::update(float deltaTime) {
    this->deltaTime = deltaTime;
}

void ParticleSimulator::render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) {
    // Bind the shader
    shader.use();

    // Bind the SSBO
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

    // Set the uniforms
    shader.setMat4("u_view", cameraViewMatrix);
    shader.setMat4("u_projection", cameraProjectionMatrix);
    shader.setFloat("u_deltaTime", deltaTime);
    shader.setVec3("u_pointOfGravity", pointOfGravity);

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
