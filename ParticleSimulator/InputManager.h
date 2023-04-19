#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

struct GLFWwindow;

class InputManager {
public:
    /*
     * Callback function for key window events
     */
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
    /*
     * Functions to check if a key is pressed (used for each game loop iteration)
     */
    static auto isForwardKeyPressed(GLFWwindow* window) -> bool;

    static auto isBackwardKeyPressed(GLFWwindow* window) -> bool;

    static auto isLeftKeyPressed(GLFWwindow* window) -> bool;

    static auto isRightKeyPressed(GLFWwindow* window) -> bool;

    static auto isUpKeyPressed(GLFWwindow* window) -> bool;

    static auto isDownKeyPressed(GLFWwindow* window) -> bool;

    /*
     * Functions for mouse movement (used for each game loop iteration)
     */
    static void getMousePosition(GLFWwindow* window, double& xPos, double& yPos);

    static auto isKeyMouseMovementPressed(GLFWwindow* window) -> bool;

    static auto isKeyMouseSetAttractorPressed(GLFWwindow* window) -> bool;

public:
    /*
     * Functions for Emscripten drag
     */

#ifdef __EMSCRIPTEN__
    struct DragMovementData {
        float dragX = 0.0F;
        float dragY = 0.0F;
        bool isUsingDrag = false;
    };

    static InputManager::DragMovementData dragMovementData;

    static EM_BOOL touchEnd_callback(int eventType, const EmscriptenTouchEvent* touchEvent, void* userData);

    static EM_BOOL touchStart_callback(int eventType, const EmscriptenTouchEvent* touchEvent, void* userData);

    static EM_BOOL touchMove_callback(int eventType, const EmscriptenTouchEvent* touchEvent, void* userData);
#endif
};

#endif // INPUT_MANAGER_H
