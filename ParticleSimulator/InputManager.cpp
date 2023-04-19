#include "InputManager.h"

#include "ParticleSimulatorLauncher.h"
#include <GLFW/glfw3.h>

void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)scancode;
    (void)mods;
    auto* engine = static_cast<ParticleSimulatorLauncher*>(glfwGetWindowUserPointer(window));

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

auto InputManager::isForwardKeyPressed(GLFWwindow* window) -> bool {
    return glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
}

auto InputManager::isBackwardKeyPressed(GLFWwindow* window) -> bool {
    return glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
}

auto InputManager::isLeftKeyPressed(GLFWwindow* window) -> bool {
    return glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
}

auto InputManager::isRightKeyPressed(GLFWwindow* window) -> bool {
    return glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
}

auto InputManager::isUpKeyPressed(GLFWwindow* window) -> bool {
    return glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS;
}

auto InputManager::isDownKeyPressed(GLFWwindow* window) -> bool {
    return glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS;
}

auto InputManager::isKeyMouseMovementPressed(GLFWwindow* window) -> bool {
    return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
}

auto InputManager::isKeyMouseSetAttractorPressed(GLFWwindow* window) -> bool {
    return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

void InputManager::getMousePosition(GLFWwindow* window, double& xPos, double& yPos) {
    glfwGetCursorPos(window, &xPos, &yPos);
}


#ifdef __EMSCRIPTEN__
InputManager::DragMovementData InputManager::dragMovementData = { 0.0F, 0.0F, false };

// Define emsccripten callback touch start
EM_BOOL InputManager::touchStart_callback(int eventType, const EmscriptenTouchEvent* touchEvent, void* userData) {
    DragMovementData* dragMoveDataBuffer = (DragMovementData*)userData;
    dragMoveDataBuffer->isUsingDrag = true;
    dragMoveDataBuffer->dragX = touchEvent->touches[0].targetX;
    dragMoveDataBuffer->dragY = touchEvent->touches[0].targetY;
    return EM_TRUE; // Return true to allow the event to propagate
}

// Define emsccripten callback touch move
EM_BOOL InputManager::touchMove_callback(int eventType, const EmscriptenTouchEvent* touchEvent, void* userData) {
    DragMovementData* dragMoveDataBuffer = (DragMovementData*)userData;
    dragMoveDataBuffer->dragX = touchEvent->touches[0].targetX;
    dragMoveDataBuffer->dragY = touchEvent->touches[0].targetY;
    return EM_TRUE; // Return true to allow the event to propagate
}

// Define emsccripten callback touch end
EM_BOOL InputManager::touchEnd_callback(int eventType, const EmscriptenTouchEvent* touchEvent, void* userData) {
    DragMovementData* dragMoveDataBuffer = (DragMovementData*)userData;
    dragMoveDataBuffer->isUsingDrag = false;
    return EM_TRUE; // Return true to allow the event to propagate
}
#endif
