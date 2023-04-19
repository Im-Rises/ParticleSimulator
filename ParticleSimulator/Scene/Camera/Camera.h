#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
    // Define the camera Up
    glm::vec3 cameraUp = glm::vec3(0.0F, 1.0F, 0.0F);

    // Matrix transformations
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

public:
    // Camera attributes
    float movementSpeed = 3.0F;
    float rotationSpeed = 0.1F;
    float fov = 90.0F;
    float nearPlane = 0.1F;
    float farPlane = 100.0F;

    // Position and rotation of the camera
    bool constrainPitch = true;
    float initialYaw = -90.0F;
    float yaw;
    float initialPitch = 0.0F;
    float pitch;
    glm::vec3 initPosition = glm::vec3(0.0F, 0.0F, 10.0F);
    glm::vec3 position;

    // Movement buffer to take into account the deltaTime
    glm::vec3 movementBuffer;
    glm::vec3 cameraFrontBuffer = glm::vec3(0.0F, 0.0F, -1.0F);

public:
    Camera(int display_w, int display_h);

    void reset();

public:
    void update(float deltaTime);

private:
    void updateViewMatrix();

public:
    void updateProjectionMatrix(int display_w, int display_h);

public:
    void moveForward();

    void moveBackward();

    void moveLeft();

    void moveRight();

    void moveUp();

    void moveDown();

    void processMouseMovement(float xMovement, float yMovement);


public:
    [[nodiscard]] auto getViewMatrix() const -> glm::mat4;

    [[nodiscard]] auto getProjectionMatrix() const -> glm::mat4;
};


#endif // CAMERA_H
