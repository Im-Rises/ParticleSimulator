#ifndef PARTICLE_EMISSION_H
#define PARTICLE_EMISSION_H

#include <memory>
#include <string_view>
#include "Scene/Scene.h"

struct GLFWwindow;

constexpr const std::string_view PROJECT_NAME = "Particle Simulator 3D";
constexpr const std::string_view PROJECT_VERSION = "0.1.0";
constexpr const std::string_view PROJECT_GITHUB = "https://github.com/Im-Rises/ParticleSimulator";
constexpr const std::string_view PROJECT_AUTHOR = "Im-Rises (Quentin Morel)";

class ParticleSimulatorLauncher {
private:
    GLFWwindow* window;
    int display_w = 1280, display_h = 720;
    bool isFullscreen = false;

    std::unique_ptr<Scene> scene;
    float fixedUpdate = 50.0f;

    struct {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;
    } clear_color;

public:
    explicit ParticleSimulatorLauncher();
    ~ParticleSimulatorLauncher();
    void start();

private:
    void handleInputs();
    void handleUi(float deltaTime);
    void updateGame(float deltaTime);
    void updateScreen();

private:
    void centerWindow();
    void toggleFullscreen();

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
