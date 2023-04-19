#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include "../../Shader/Shader.h"
#include <glm/glm.hpp>

class Entity {
public:
    glm::vec3 position = glm::vec3(0.0F, 0.0F, 0.0F);
    glm::vec3 rotation = glm::vec3(0.0F, 0.0F, 0.0F);
    glm::vec3 scale = glm::vec3(1.0F, 1.0F, 1.0F);

protected:
    glm::mat4 modelMatrix;
    Shader shader;

protected:
    bool isPaused = false;

public:
    Entity(const std::string& vertexSource, const std::string& fragmentSource);

    Entity(const Entity&) = delete;
    auto operator=(const Entity&) -> Entity& = delete;
    Entity(Entity&&) = delete;
    auto operator=(Entity&&) -> Entity& = delete;

    virtual ~Entity() = default;

public:
    virtual void update(const float& deltaTime) = 0;

    //    virtual void fixedUpdate(float fixedDeltaTime) = 0;

    virtual void render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) = 0;

    void updateModelMatrix();

    void setIsPaused(bool isPaused);

    [[nodiscard]] auto getIsPaused() const -> bool;
};


#endif // ENTITY_H
