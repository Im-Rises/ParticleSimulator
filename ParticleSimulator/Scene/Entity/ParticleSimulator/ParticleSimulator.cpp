#include "ParticleSimulator.h"

#include <random>
#include <iostream>

ParticleSimulator::ParticleSimulator(int particleCount) : Entity("shaders/ParticleSimulator.vert", "shaders/ParticleSimulator.frag") {
    // Resize the particles vector
    particles.resize(particleCount);

    // Init the random engine
    std::mt19937 randomEngine;
    std::uniform_real_distribution<float> randomFloats(-1.0f, 1.0f);

    // Init the particles as a cube
    for (auto& particle : particles)
    {
        particle.position = glm::vec3(randomFloats(randomEngine),
                                randomFloats(randomEngine),
                                randomFloats(randomEngine)) +
                            position;
    }

    //    // Init the particles as a sphere
    //    for (auto & particle : particles)
    //    {
    //        particle.position = glm::vec3(randomFloats(randomEngine),
    //                                      randomFloats(randomEngine),
    //                                      randomFloats(randomEngine));
    //        particle.position = glm::normalize(particle.position) * 0.5f;
    //    }

    // Init the VAO and VBO
    glGenVertexArrays(1, &VAO);

    // Bind the VAO
    glBindVertexArray(VAO);

    // Generate the SSBO
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

    // Unbind the VAO
    glBindVertexArray(0);

    // Unbind the SSBO
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

ParticleSimulator::~ParticleSimulator() {
    glDeleteBuffers(1, &ssbo);
}

void ParticleSimulator::update(float deltaTime) {
    this->deltaTime = deltaTime;
}

void ParticleSimulator::render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) {
    // Bind the VAO
    glBindVertexArray(VAO);

    // Bind the SSBO
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

    // Bind the shader
    shader.use();

    // Set the uniforms
    shader.setMat4("u_view", cameraViewMatrix);
    shader.setMat4("u_projection", cameraProjectionMatrix);
    shader.setFloat("u_deltaTime", deltaTime);
    shader.setVec3("u_pointOfGravity", pointOfGravity);
    shader.setBool("u_isPaused", isPaused);

    // Draw the particles
    glDrawArrays(GL_POINTS, 0, particles.size());

    // Barrier
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    // Unbind the VAO
    glBindVertexArray(0);

    // Unbind the SSBO
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ParticleSimulator::setPointOfGravity(glm::vec3 cameraPos, glm::vec3 cameraFront) {
    pointOfGravity = cameraPos + cameraFront * 10.0f;
}
