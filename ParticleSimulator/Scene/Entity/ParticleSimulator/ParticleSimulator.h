#ifndef PARTICLE_SIMULATOR_H
#define PARTICLE_SIMULATOR_H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../Entity.h"

class ParticleSimulator : public Entity {
private:
    GLuint VAO; //,VBO;
    GLuint ssbo;

    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;
        Particle() : position(glm::vec3(0.0f)), velocity(glm::vec3(0.0f)) {}
    };

    std::vector<Particle> particles;

    float deltaTime;
    glm::vec3 pointOfGravity;

public:
    explicit ParticleSimulator(int particleCount = 100000);
    ~ParticleSimulator();

public:
    void update(float deltaTime) override;
    void render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) override;
    //    void setPointOfGravity(glm::vec3 cameraPos, glm::vec3 cameraFront) {
    //        pointOfGravity = cameraPos + cameraFront * 10.0f;
    //    }
};

#endif // PARTICLE_SIMULATOR_H
