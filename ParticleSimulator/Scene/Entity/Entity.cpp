#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>

// Entity::Entity() {
//     // Should be called in the constructor of the derived class
//     //    updateModelMatrix();
// }

// Entity::Entity(const std::string& vertexSource, const std::string& fragmentSource) : shader(
//                                                                                          vertexSource.c_str(),
//                                                                                          fragmentSource.c_str()) {
//     // Should be called in the constructor of the derived class
//     //    updateModelMatrix();
// }

// Entity::~Entity() {
// }

void Entity::updateModelMatrix() {
    modelMatrix = glm::mat4(1.0F);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0F, 0.0F, 0.0F));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0F, 1.0F, 0.0F));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0F, 0.0F, 1.0F));
    modelMatrix = glm::scale(modelMatrix, scale);
}

void Entity::setIsPaused(bool isPaused) {
    this->isPaused = isPaused;
}

[[maybe_unused]] auto Entity::getIsPaused() const -> bool {
    return isPaused;
}
