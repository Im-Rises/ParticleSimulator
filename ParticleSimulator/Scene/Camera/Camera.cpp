#include "Camera.h"

Camera::Camera(int display_w, int display_h) {
    position = initPosition;
    yaw = initialYaw;
    pitch = initialPitch;
    updateViewMatrix();
    updateProjectionMatrix(display_w, display_h);
}

void Camera::reset() {
    position = initPosition;
    yaw = initialYaw;
    pitch = initialPitch;
    processMouseMovement(0.0F, 0.0F);
}


void Camera::update(float deltaTime) {
    position += movementBuffer * movementSpeed * deltaTime;
    updateViewMatrix();
    movementBuffer = glm::vec3(0.0F);
}

void Camera::updateViewMatrix() {
    viewMatrix = glm::lookAt(position, position + cameraFrontBuffer, cameraUp);
}

void Camera::updateProjectionMatrix(int display_w, int display_h) {
    /*
     * Update the projection matrix when the window is resized
     */
    projectionMatrix = glm::perspective(glm::radians(fov / 2),
        static_cast<float>(display_w) / static_cast<float>(display_h), nearPlane,
        farPlane);
}

void Camera::moveForward() {
    movementBuffer += cameraFrontBuffer;
}

void Camera::moveBackward() {
    movementBuffer -= cameraFrontBuffer;
}

void Camera::moveLeft() {
    movementBuffer -= glm::normalize(glm::cross(cameraFrontBuffer, cameraUp));
}

void Camera::moveRight() {
    movementBuffer += glm::normalize(glm::cross(cameraFrontBuffer, cameraUp));
}

void Camera::moveUp() {
    movementBuffer += cameraUp;
}

void Camera::moveDown() {
    movementBuffer -= cameraUp;
}

void Camera::processMouseMovement(float xMovement, float yMovement) {
    yaw += xMovement * rotationSpeed;
    pitch += yMovement * rotationSpeed;

    if (constrainPitch)
    {
        if (pitch > 89.0F)
            pitch = 89.0F;
        if (pitch < -89.0F)
            pitch = -89.0F;
    }
    else
    {
        if (pitch > 360.0F)
            pitch -= 360.0F;
        if (pitch < -360.0F)
            pitch += 360.0F;
    }

    if (yaw > 360.0F)
        yaw -= 360.0F;
    if (yaw < -360.0F)
        yaw += 360.0F;


    cameraFrontBuffer.x = static_cast<float>(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
    cameraFrontBuffer.y = static_cast<float>(sin(glm::radians(pitch)));
    cameraFrontBuffer.z = static_cast<float>(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
}

auto Camera::getViewMatrix() const -> glm::mat4 {
    return viewMatrix;
}

auto Camera::getProjectionMatrix() const -> glm::mat4 {
    return projectionMatrix;
}
