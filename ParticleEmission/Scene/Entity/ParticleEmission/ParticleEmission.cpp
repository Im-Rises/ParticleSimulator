#include "ParticleEmission.h"

ParticleEmission::ParticleEmission() : Entity("ParticleEmission/Shader/ParticlesEmission.vert", "ParticleEmission/Shader/ParticlesEmission.frag") {
}

ParticleEmission::~ParticleEmission() {
}

void ParticleEmission::update(float deltaTime) {
}

void ParticleEmission::render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) {
}
