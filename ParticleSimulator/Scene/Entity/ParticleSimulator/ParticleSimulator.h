#ifndef PARTICLE_SIMULATOR_H
#define PARTICLE_SIMULATOR_H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../Entity.h"

class ParticleSimulator : public Entity {
private:
    static const char* const VertexShaderSource;
    static const char* const FragmentShaderSource;

    GLuint VAO, VBO;

    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;

        Particle() : position(glm::vec3(0.0F)), velocity(glm::vec3(1.0F, 1.0F, 1.0F)) {}
    };

    std::vector<Particle> particles;

    glm::vec3 attractorPosition;

    float isAttracting = 0.0F;

public:
    float spawnRadius = 2.0F;

    float damping = 0.99F;
    float particleMass = 50.0F;
    float attractorMass = 250.0F;
    float gravity = 1.0F;
    float distanceOffset = 10.0F;

public:
    explicit ParticleSimulator(int particleCount = 100000);

    ParticleSimulator(const ParticleSimulator&) = delete;
    auto operator=(const ParticleSimulator&) -> ParticleSimulator& = delete;
    ParticleSimulator(ParticleSimulator&&) = delete;
    auto operator=(ParticleSimulator&&) -> ParticleSimulator& = delete;

    ~ParticleSimulator() override;

public:
    void update(const float& deltaTime) override;

    void render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) override;

    void reset();

private:
    void randomizeParticles();

public:
    void setAttractorPosition(const glm::vec3& pos);

    void setIsAttracting(const bool& value);

    [[nodiscard]] auto getIsAttracting() const -> bool;

    void setParticlesCount(const size_t& count);

    [[nodiscard]] auto getParticlesCount() const -> size_t;
};

#endif // PARTICLE_SIMULATOR_H
