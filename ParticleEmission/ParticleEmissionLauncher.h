#ifndef PARTICLE_EMISSION_H
#define PARTICLE_EMISSION_H

#include <memory>
#include <string_view>
#include "Scene/Scene.h"

struct GLFWwindow;

constexpr const std::string_view PROJECT_NAME = "Particle Emission 3D";
constexpr const std::string_view PROJECT_VERSION = "1.0.0";
constexpr const std::string_view PROJECT_GITHUB = "https://github.com/Im-Rises/ParticleEmission";
constexpr const std::string_view PROJECT_AUTHOR = "Im-Rises (Quentin Morel)";

class ParticleEmissionLauncher {
private:
    GLFWwindow* window;
    int display_w = 1280, display_h = 720;

    std::unique_ptr<Scene> scene;

    struct {
        float x = 0.45f;
        float y = 0.55f;
        float z = 0.60f;
        float w = 1.0f;
    } clear_color;

public:
    explicit ParticleEmissionLauncher();
    ~ParticleEmissionLauncher();
    void start();

private:
    void handleInputs();
    void handleUi(float deltaTime);
    void updateGame(float deltaTime);
    void updateScreen();
};

#endif // PARTICLE_EMISSION_H
