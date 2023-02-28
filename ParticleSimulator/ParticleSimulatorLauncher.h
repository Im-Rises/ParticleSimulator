#ifndef PARTICLE_EMISSION_H
#define PARTICLE_EMISSION_H

#include <memory>
#include <string_view>
#include "Scene/Scene.h"

struct GLFWwindow;

constexpr const std::string_view PROJECT_NAME = "Particle Simulator 3D";
constexpr const std::string_view PROJECT_VERSION = "0.1.3";
constexpr const std::string_view PROJECT_GITHUB = "https://github.com/Im-Rises/ParticleSimulator";
constexpr const std::string_view PROJECT_AUTHOR = "Im-Rises (Quentin Morel)";

class ParticleSimulatorLauncher {
private:
    GLFWwindow* window;
    int display_w = 1280, display_h = 720;
    bool isFullscreen = false;

    std::unique_ptr<Scene> scene;
    float fixedUpdate = 60.0f;

    struct {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;
    } clear_color;

    float targetDistance = 10.0f;
    glm::vec3 mousePositionWorld;

public:
    explicit ParticleSimulatorLauncher();
    ~ParticleSimulatorLauncher();
    void start();

private:
    void handleInputs();
    void handleUi(float deltaTime);
    void updateGame(float deltaTime);
    void updateScreen();

public:
    void toggleFullscreen();
    void resetScene();
    void toggleScenePause();

private:
    void centerWindow();
    bool isWindowMinimized();

private:
    void calculateMouseMovement(const double& xMouse, const double& yMouse, double& xMovement, double& yMovement);
    glm::vec3 projectMouse(const double& xMouse, const double& yMouse);
    glm::vec3 calculateWorldSpaceRay(glm::mat4 inverseProjection, glm::mat4 inverseView, glm::vec2 normalizedDeviceCoords);

private:
    std::string_view getOpenGLVendor();
    std::string_view getOpenGLVersion();
    std::string_view getGLSLVersion();
    std::string getGLFWVersion();
    std::string_view getGladVersion();
    std::string getImGuiVersion();
    std::string getGLMVersion();
};

#endif // PARTICLE_EMISSION_H
