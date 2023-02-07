#ifndef PARTICLE_SIMULATOR_H
#define PARTICLE_SIMULATOR_H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../Entity.h"

class ParticleSimulator : public Entity {
private:
    GLuint ssbo;
    GLuint vao;
    GLuint vbo;

    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;
        Particle() : position(glm::vec3(0.0f)), velocity(glm::vec3(0.0f)) {}
    };

    std::vector<Particle> particles;
    
public:
    ParticleSimulator(int particleCount = 100000);
    ~ParticleSimulator();

public:
    void update(float deltaTime) override;
    void render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) override;
};

#endif // PARTICLE_SIMULATOR_H
