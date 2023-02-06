#ifndef PARTICLE_EMISSION_H
#define PARTICLE_EMISSION_H

#include "../Entity.h"

class ParticleEmission : public Entity {
private:
    int particleCount = 1000;
    unsigned int VAO, VBO;

public:
    ParticleEmission();
    ~ParticleEmission();

public:
    void update(float deltaTime) override;
    void render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) override;
};



#endif // PARTICLE_EMISSION_H
