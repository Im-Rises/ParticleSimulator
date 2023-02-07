#include "ParticleSimulator.h"

ParticleSimulator::ParticleSimulator() : Entity("ParticleEmission/Shader/ParticlesEmission.vert", "ParticleEmission/Shader/ParticlesEmission.frag") {
}

ParticleSimulator::~ParticleSimulator() {
}

void ParticleSimulator::update(float deltaTime) {
}

void ParticleSimulator::render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) {
}
