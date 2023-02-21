#include "ParticleSimulator.h"

#include <random>
#include <iostream>

ParticleSimulator::ParticleSimulator(int particleCount) : Entity("shaders/ParticleSimulator.vert", "shaders/ParticleSimulator.frag") {
    position = glm::vec3(5.0f, 0.0f, 0.0f);

    // Resize the particles vector
    particles.resize(particleCount);

    // Init the random engine
    std::mt19937 randomEngine;
    std::uniform_real_distribution<float> randomFloats(-1.0f, 1.0f);

    // Init the particles as a cube
//    for (auto & particle : particles)
//    {
//        particle.position = glm::vec3(randomFloats(randomEngine),
//                                      randomFloats(randomEngine),
//                                      randomFloats(randomEngine)) +position;
//        particle.velocity = glm::vec3(0.0f);
//    }

    for (int i = 0; i < particles.size(); i++) {
        particles[i].position = glm::vec3(i);
    }

    // Generate the SSBO
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
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
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

    // Set the uniforms
    shader.setMat4("u_view", cameraViewMatrix);
    shader.setMat4("u_projection", cameraProjectionMatrix);
    shader.setFloat("u_deltaTime", deltaTime);
    shader.setVec3("u_pointOfGravity", pointOfGravity);
    shader.setBool("u_isPaused", false);

    // Draw the particles
    glDrawArrays(GL_POINTS, 0, particles.size());

    // Barrier
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    // Unbind the SSBO
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
