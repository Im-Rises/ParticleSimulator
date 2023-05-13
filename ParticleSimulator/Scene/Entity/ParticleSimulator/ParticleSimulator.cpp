#include "ParticleSimulator.h"

#include <random>
#include <iostream>

#include "../../../Utility/piDeclaration.h"

const char* const ParticleSimulator::VertexShaderSource =
    R"(#version 300 es

precision highp float;

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_velocity;

uniform mat4 u_mvp;

out vec3 v_velocity;

void main()
{
    gl_Position = u_mvp * vec4(a_position, 1.0);
    v_velocity = a_velocity;
    gl_PointSize = 1.0f;
}
)";

const char* const ParticleSimulator::FragmentShaderSource =
    R"(#version 300 es

precision highp float;

in vec3 v_velocity;

out vec4 o_fragColor;

void main() {
    vec3 v_color = vec3(min(v_velocity.y, 0.8f), max(v_velocity.x, 0.5f), min(v_velocity.z, 0.5f));
    o_fragColor = vec4(v_color, 1.0f);
}
)";

ParticleSimulator::ParticleSimulator(int particleCount) : shader(VertexShaderSource, FragmentShaderSource, false) {
    position = glm::vec3(6.0F, 0.0F, 0.0F);

    // Resize the particles vector
    particles.resize(particleCount);

    // Init the particles
    randomizeParticles();

    // Init the VAO
    glGenVertexArrays(1, &VAO);

    // Init the VBO
    glGenBuffers(1, &VBO);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Bind the VAO
    glBindVertexArray(VAO);

    // Set the VBO data
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(particles.size() * sizeof(Particle)), particles.data(), GL_STATIC_DRAW);

    // Set the VAO attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, velocity));
    glEnableVertexAttribArray(1);

    // Unbind the VAO
    glBindVertexArray(0);

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

ParticleSimulator::~ParticleSimulator() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void ParticleSimulator::update(const float& deltaTime) {
    if (isPaused)
        return;

    for (auto& particle : particles)
    {
        // Calculate the distance between the particle and the point of gravity
        const glm::vec3 r = attractorPosition - particle.position;
        const float rSquared = glm::dot(r, r) + distanceOffset;

        // Calculate the force
        const glm::vec3 force = ((gravity * particleMass * attractorMass * glm::normalize(r)) / rSquared) * isAttracting;

        // Calculate the acceleration
        const glm::vec3 acceleration = force / particleMass;

        // Calculate the position
        particle.position += particle.velocity * deltaTime + 0.5F * acceleration * deltaTime * deltaTime;

        // Calculate the velocity
        particle.velocity += acceleration * deltaTime;

        // Damp the velocity
        particle.velocity *= damping;
    }
}

void ParticleSimulator::render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) {
    // Bind the VAO
    glBindVertexArray(VAO);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Set the VBO data
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(particles.size() * sizeof(Particle)), particles.data(), GL_STATIC_DRAW);

    // Bind the shader
    shader.use();

    // Set the uniform variables
    shader.setMat4("u_mvp", cameraProjectionMatrix * cameraViewMatrix);

    // Draw the particles
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(particles.size()));

    // Unbind the VAO
    glBindVertexArray(0);

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSimulator::reset() {
    randomizeParticles();
}

void ParticleSimulator::randomizeParticles() {
    // Init the random engine
    std::mt19937 randomEngine;
    std::uniform_real_distribution<float> randomFloats(0.0F, 2.0F * M_PI);
    std::uniform_real_distribution<float> const randomFloats2(-1.0F, 1.0F);

    // Init the particles as a sphere
    for (auto& particle : particles)
    {
        const float angle1 = randomFloats(randomEngine);
        const float angle2 = randomFloats(randomEngine);
        const float x = spawnRadius * std::sin(angle1) * std::cos(angle2);
        const float y = spawnRadius * std::sin(angle1) * std::sin(angle2);
        const float z = spawnRadius * std::cos(angle1);
        particle.position = glm::vec3(x, y, z) + position;
        particle.velocity = glm::vec3(0.0F, 0.0F, 0.0F);
    }
}

void ParticleSimulator::setAttractorPosition(const glm::vec3& pos) {
    attractorPosition = pos;
}

void ParticleSimulator::setIsAttracting(const bool& value) {
    isAttracting = value ? 1.0F : 0.0F;
}

auto ParticleSimulator::getIsAttracting() const -> bool {
    return isAttracting == 1.0F;
}

void ParticleSimulator::setParticlesCount(const size_t& count) {
    particles.resize(count);
    randomizeParticles();
}

auto ParticleSimulator::getParticlesCount() const -> size_t {
    return particles.size();
}
