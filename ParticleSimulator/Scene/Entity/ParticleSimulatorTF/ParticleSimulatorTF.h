#ifndef PARTICLE_SIMULATOR_TF_H
#define PARTICLE_SIMULATOR_TF_H

#include <array>

#include <glad/glad.h>

#include "../Entity.h"

class ParticleSimulatorTF : public Entity {
    static const char* const VertexShaderSource;
    static const char* const FragmentShaderSource;

    Shader shader;

    GLuint VAO[2];
    GLuint TFBO[2];
    GLuint VBO[2];

    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;

        Particle() : position(glm::vec3(0.0F)), velocity(glm::vec3(0.0F, 0.0F, 0.0F)) {}
    };

    GLuint currentVAO;
    GLuint currentTFBO;

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
    float softening = 10.0F;

public:
    explicit ParticleSimulatorTF(int particlesCount = 100000);

    ParticleSimulatorTF(const ParticleSimulatorTF&) = delete;
    auto operator=(const ParticleSimulatorTF&) -> ParticleSimulatorTF& = delete;
    ParticleSimulatorTF(ParticleSimulatorTF&&) = delete;
    auto operator=(ParticleSimulatorTF&&) -> ParticleSimulatorTF& = delete;

    ~ParticleSimulatorTF() override;

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


#endif // PARTICLE_SIMULATOR_TF_H
