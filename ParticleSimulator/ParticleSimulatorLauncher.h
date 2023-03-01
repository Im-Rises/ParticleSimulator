#ifndef PARTICLE_EMISSION_H
#define PARTICLE_EMISSION_H

#include <memory>
#include <string_view>
#include <glm/glm.hpp>

class Scene;
struct GLFWwindow;

constexpr const std::string_view PROJECT_NAME = "Particle Simulator 3D";
constexpr const std::string_view PROJECT_VERSION = "1.0.0";
constexpr const std::string_view PROJECT_GITHUB = "https://github.com/Im-Rises/ParticleSimulator";
constexpr const std::string_view PROJECT_AUTHOR = "Im-Rises (Quentin Morel)";

class ParticleSimulatorLauncher {
private:
    GLFWwindow* window;
    const int windowWidth = 1280, windowHeight = 720;
    int displayWidth, displayHeight;
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
    void updateViewport();

private:
    void calculateMouseMovement(const double& xMouse, const double& yMouse, double& xMovement, double& yMovement);
    glm::vec3 projectMouse(const double& xMouse, const double& yMouse);
    
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
