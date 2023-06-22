#ifndef PARTICLE_SIMULATOR_LAUNCHER_H
#define PARTICLE_SIMULATOR_LAUNCHER_H

#include <memory>
#include <string_view>
#include <glm/glm.hpp>

class Scene;

struct GLFWwindow;

class ParticleSimulatorLauncher {
public:
    static constexpr std::string_view PROJECT_NAME = "Particle Simulator 3D";
    static constexpr std::string_view PROJECT_VERSION = "2.2.0";
    static constexpr std::string_view PROJECT_LINK = "https://github.com/Im-Rises/ParticleSimulator";
    static constexpr std::string_view PROJECT_AUTHOR = "Im-Rises (Quentin Morel)";

private:
    GLFWwindow* window;

    int windowWidth = 1280;
    int windowHeight = 720;

    int displayWidth, displayHeight;
    int windowPosX, windowPosY;

    float fixedDeltaTime = 1.0F / 60.0F;

    std::unique_ptr<Scene> scene;

    struct {
        float x = 0.0F;
        float y = 0.0F;
        float z = 0.0F;
        float w = 1.0F;
    } clear_color;

    float attractorDistance = 10.0F;
    glm::vec3 mousePositionWorld;

    static constexpr int MAX_PARTICLES_COUNT = 10000000;


#ifndef __EMSCRIPTEN__
    float pointSize = 1.0F;
    bool isFullscreen = false;
#endif
    bool isUiVisible = true;

public:
    ParticleSimulatorLauncher();

    ParticleSimulatorLauncher(const ParticleSimulatorLauncher&) = delete;

    auto operator=(const ParticleSimulatorLauncher&) -> ParticleSimulatorLauncher& = delete;

    ParticleSimulatorLauncher(ParticleSimulatorLauncher&&) = delete;

    auto operator=(ParticleSimulatorLauncher&&) -> ParticleSimulatorLauncher& = delete;

    ~ParticleSimulatorLauncher();

public:
    void start();

private:
    void handleInputs();

    void handleUi(float deltaTime);

    void updateGame(float deltaTime);

    void updateScreen();

public:
    void resetScene();

    void toggleScenePause();

    void updateViewport();

    void centerWindow();

    void toggleFullscreen();

    void toggleUiVisibility();

    void clearScreen() const;

    [[nodiscard]] auto isMinimized() const -> bool;

private:
    static void calculateMouseMovement(const double& xMouse, const double& yMouse, double& xMovement, double& yMovement);

    auto projectMouse(const double& xMouse, const double& yMouse) -> glm::vec3;

private:
    static auto getOpenGLVendor() -> std::string_view;

    static auto getOpenGLVersion() -> std::string_view;

    static auto getGLSLVersion() -> std::string_view;

    static auto getGLFWVersion() -> std::string;

    static auto getGladVersion() -> std::string_view;

    static auto getImGuiVersion() -> std::string;

    static auto getGLMVersion() -> std::string;
};

#endif // PARTICLE_SIMULATOR_LAUNCHER_H
