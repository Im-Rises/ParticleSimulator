#include "Camera.h"

Camera::Camera(int display_w, int display_h) {
    updateViewMatrix();
    updateProjectionMatrix(display_w, display_h);
}

Camera::~Camera() {
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
    projectionMatrix = glm::perspective(glm::radians(fov / 2), (float)display_w / (float)display_h, nearPlane,
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
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }
    else
    {
        if (pitch > 360.0f)
            pitch -= 360.0f;
        if (pitch < -360.0f)
            pitch += 360.0f;
    }

    if (yaw > 360.0f)
        yaw -= 360.0f;
    if (yaw < -360.0f)
        yaw += 360.0f;

    cameraFrontBuffer.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFrontBuffer.y = sin(glm::radians(pitch));
    cameraFrontBuffer.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
}

glm::mat4 Camera::getViewMatrix() const {
    return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() const {
    return projectionMatrix;
}
