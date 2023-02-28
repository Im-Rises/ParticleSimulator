#include "InputManager.h"

#include "ParticleSimulatorLauncher.h"
#include <GLFW/glfw3.h>

void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto* engine = (ParticleSimulatorLauncher*)glfwGetWindowUserPointer(window);

    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        if (action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        break;
    case GLFW_KEY_F11:
        if (action == GLFW_PRESS)
        {
            engine->toggleFullscreen();
        }
        break;
    case GLFW_KEY_R:
        if (action == GLFW_PRESS)
        {
            engine->resetScene();
        }
        break;
    case GLFW_KEY_P:
        if (action == GLFW_PRESS)
        {
            engine->toggleScenePause();
        }
        break;
    }
}

bool InputManager::isForwardKeyPressed(GLFWwindow* window) {
    return glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
}

bool InputManager::isBackwardKeyPressed(GLFWwindow* window) {
    return glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
}

bool InputManager::isLeftKeyPressed(GLFWwindow* window) {
    return glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
}

bool InputManager::isRightKeyPressed(GLFWwindow* window) {
    return glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
}

bool InputManager::isUpKeyPressed(GLFWwindow* window) {
    return glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS; // GLFW_KEY_PAGE_UP
}

bool InputManager::isDownKeyPressed(GLFWwindow* window) {
    return glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS; // GLFW_KEY_PAGE_DOWN
}

bool InputManager::isKeyMouseMovementPressed(GLFWwindow* window) {
    return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
}

bool InputManager::isKeyMouseSetTargetPressed(GLFWwindow* window) {
    return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

void InputManager::getMousePosition(GLFWwindow* window, double& x, double& y) {
    glfwGetCursorPos(window, &x, &y);
}
