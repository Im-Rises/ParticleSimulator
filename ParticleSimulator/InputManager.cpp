#include "InputManager.h"

double InputManager::lastMouseX = 0.0;
double InputManager::lastMouseY = 0.0;

void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
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

bool InputManager::isPauseKeyPressed(GLFWwindow* window) {
    static bool isPauseKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        if (!isPauseKeyPressed)
        {
            isPauseKeyPressed = true;
            return true;
        }
    }
    else
    {
        isPauseKeyPressed = false;
    }
    return false;
}

bool InputManager::isKeyMouseMovementPressed(GLFWwindow* window) {
    return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
}

bool InputManager::isKeyMouseSetTargetPressed(GLFWwindow* window) {
    return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

bool InputManager::isFullscreenKeyPressed(GLFWwindow* window) {
    static bool isFullscreenKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS)
    {
        if (!isFullscreenKeyPressed)
        {
            isFullscreenKeyPressed = true;
            return true;
        }
    }
    else
    {
        isFullscreenKeyPressed = false;
    }
    return false;
}

bool InputManager::isResetKeyPressed(GLFWwindow* window) {
    static bool isResetKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (!isResetKeyPressed)
        {
            isResetKeyPressed = true;
            return true;
        }
    }
    else
    {
        isResetKeyPressed = false;
    }
    return false;
}

void InputManager::getMouseMovement(GLFWwindow* window, double& xMovement, double& yMovement) {
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    xMovement = x - lastMouseX;
    yMovement = lastMouseY - y;

    lastMouseX = x;
    lastMouseY = y;
}
