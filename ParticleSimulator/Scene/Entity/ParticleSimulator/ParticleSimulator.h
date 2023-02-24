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

    //    float isRunning = 1.0f;
    float isTargeting = 0.0f;

public:
    explicit ParticleSimulator(int particleCount = 100000);
    ~ParticleSimulator();

public:
    void update(const float& deltaTime) override;
    void render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) override;

public:
    void randomizeParticles();
    void reset();
    void setTarget(const glm::vec3& target) { this->pointOfGravity = target; }

public:
    void setIsTargeting(const bool& value) { this->isTargeting = (float)value; }
    bool getIsTargeting() const { return (bool)this->isTargeting; }
    //    void setIsRunning(const bool& value) { this->isRunning = (float)value; }

    [[nodiscard]] size_t getParticleCount() const { return particles.size(); }
};

#endif // PARTICLE_SIMULATOR_H
