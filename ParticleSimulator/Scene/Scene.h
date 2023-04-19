#ifndef SCENE_H
#define SCENE_H

#include "Camera/Camera.h"
#include "Entity/ParticleSimulatorSSBO/ParticleSimulatorSSBO.h"

class Scene {
private:
    bool isPaused = false;

public:
    Camera camera;
    ParticleSimulatorSSBO particleSimulatorSsbo;

    //    std::vector<Entity*> entities;

public:
    Scene(int display_w, int display_h);

    void update(float deltaTime);

    void render();

public:
    void updateProjectionMatrix(int display_w, int display_h);

    void togglePause();

    void reset();

public:
    [[nodiscard]] auto getIsPaused() const -> bool;
};

#endif // SCENE_H
