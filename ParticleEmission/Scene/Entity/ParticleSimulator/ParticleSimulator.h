#ifndef PARTICLE_EMISSION_H
#define PARTICLE_EMISSION_H

#include "../Entity.h"

class ParticleSimulator : public Entity {
private:
    int particleCount = 1000;
    unsigned int VAO, VBO;

public:
    ParticleSimulator();
    ~ParticleSimulator();

public:
    void update(float deltaTime) override;
    void render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) override;
};



#endif // PARTICLE_EMISSION_H
