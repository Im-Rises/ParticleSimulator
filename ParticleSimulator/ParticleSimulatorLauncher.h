#ifndef PARTICLE_EMISSION_H
#define PARTICLE_EMISSION_H

#include <memory>
#include <string_view>
#include "Scene/Scene.h"

struct GLFWwindow;

constexpr const std::string_view PROJECT_NAME = "Particle Simulator 3D";
constexpr const std::string_view PROJECT_VERSION = "0.0.1";
constexpr const std::string_view PROJECT_GITHUB = "https://github.com/Im-Rises/ParticleSimulator";
constexpr const std::string_view PROJECT_AUTHOR = "Im-Rises (Quentin Morel)";

class ParticleSimulatorLauncher {
private:
    GLFWwindow* window;
    int display_w = 1280, display_h = 720;

    std::unique_ptr<Scene> scene;

    struct {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;
    } clear_color;

    float fixedUpdate = 50.0f;

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
    char* getOpenGLVersion();
    char* getGLSLVersion();
    char* getGLFWVersion();
    char* getGladVersion();
    char* getImGuiVersion();
    char* getGLMVersion();
};

#endif // PARTICLE_EMISSION_H
