#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

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
    static bool isForwardKeyPressed(GLFWwindow* window);
    static bool isBackwardKeyPressed(GLFWwindow* window);
    static bool isLeftKeyPressed(GLFWwindow* window);
    static bool isRightKeyPressed(GLFWwindow* window);
    static bool isUpKeyPressed(GLFWwindow* window);
    static bool isDownKeyPressed(GLFWwindow* window);

    /*
     * Functions for mouse movement (used for each game loop iteration)
     */
    static void getMousePosition(GLFWwindow* window, double& x, double& y);
    static bool isKeyMouseMovementPressed(GLFWwindow* window);
    static bool isKeyMouseSetTargetPressed(GLFWwindow* window);
};

#endif // INPUT_MANAGER_H
