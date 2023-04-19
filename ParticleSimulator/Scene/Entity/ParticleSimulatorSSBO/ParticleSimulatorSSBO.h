#ifndef PARTICLE_SIMULATOR_H
#define PARTICLE_SIMULATOR_H

#include <array>

#include <glad/glad.h>
#include <vector>
#include "../Entity.h"


class ParticleSimulatorSSBO : public Entity {
private:
    GLuint VAO;
    GLuint ssbo;

    // Can't use directly vec3 in SSBO (indexed by 4 bytes not 3): https://computergraphics.stackexchange.com/questions/5810/shader-storage-buffer-indexing-by-4-bytes-instead-of-3
    struct Particle {
        glm::vec3 position;
        float offset1{};
        glm::vec3 velocity;
        float offset2{};

        Particle() : position(glm::vec3(0.0F)), velocity(glm::vec3(0.0F)) {}
    };

    int particlesCount;

    float deltaTime = 0.0F;
    glm::vec3 attractorPosition = glm::vec3(0.0F, 0.0F, 0.0F);
    float isAttracting = 0.0F;

public:
    float spawnRadius = 2.0F;

    float damping = 0.99F;
    float particleMass = 50.0F;
    float attractorMass = 250.0F;
    float gravity = 1.0F;
    float distanceOffset = 10.0F;


public:
    explicit ParticleSimulatorSSBO(int particlesCount = 1000000);

    ParticleSimulatorSSBO(const ParticleSimulatorSSBO&) = delete;
    auto operator=(const ParticleSimulatorSSBO&) -> ParticleSimulatorSSBO& = delete;
    ParticleSimulatorSSBO(ParticleSimulatorSSBO&&) = delete;
    auto operator=(ParticleSimulatorSSBO&&) -> ParticleSimulatorSSBO& = delete;

    ~ParticleSimulatorSSBO() override;

public:
    void update(const float& deltaTime) override;

    void render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) override;

    void reset();

private:
    void randomizeParticles(std::vector<Particle>& particles);

public:
    void setAttractorPosition(const glm::vec3& pos);

    void setIsAttracting(const bool& value);

    [[nodiscard]] auto getIsAttracting() const -> bool;

    void setParticlesCount(const int& value);

    [[nodiscard]] auto getParticlesCount() const -> size_t;
};

#endif // PARTICLE_SIMULATOR_H
