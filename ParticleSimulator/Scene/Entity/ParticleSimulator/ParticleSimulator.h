#ifndef PARTICLE_SIMULATOR_H
#define PARTICLE_SIMULATOR_H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../Entity.h"

class ParticleSimulator : public Entity {
private:
    GLuint VAO;
    GLuint ssbo;

    // Can't use directly vec3 in SSBO (indexed by 4 bytes not 3): https://computergraphics.stackexchange.com/questions/5810/shader-storage-buffer-indexing-by-4-bytes-instead-of-3
    struct Particle {
        glm::vec3 position;
        float offset1;
        glm::vec3 velocity;
        float offset2;
        Particle() : position(glm::vec3(0.0f)), velocity(glm::vec3(0.0f)) {}
    };

    std::vector<Particle> particles;

    float deltaTime;
    glm::vec3 pointOfGravity;

    bool isPaused = false;

    float distanceToCamera = 10.0f;

public:
    explicit ParticleSimulator(int particleCount = 100000);
    ~ParticleSimulator();

public:
    void update(float deltaTime) override;
    void render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) override;

public:
    void randomizeParticles();
    void reset();

public:
    void setPointOfGravity(glm::vec3 cameraPos, glm::vec3 cameraFront);

public:
    [[nodiscard]] bool getIsPaused() const { return isPaused; }
    void setIsPaused(bool isPaused) { this->isPaused = isPaused; }

    [[nodiscard]] size_t getParticleCount() const { return particles.size(); }
};

#endif // PARTICLE_SIMULATOR_H
