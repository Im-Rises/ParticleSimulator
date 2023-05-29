#ifndef SCENE_H
#define SCENE_H

#include "Camera/Camera.h"

#ifdef __EMSCRIPTEN__
#include "Entity/ParticleSimulatorTF/ParticleSimulatorTF.h"
#else
#include "Entity/ParticleSimulatorSSBO/ParticleSimulatorSSBO.h"
#endif

class Scene {
private:
    bool isPaused = false;

public:
    Camera camera;

#ifdef __EMSCRIPTEN__
    ParticleSimulatorTF particleSimulator;
#else
    ParticleSimulatorSSBO particleSimulator;
#endif

    //    std::vector<Entity*> entities;

public:
    Scene(int display_w, int display_h);

    //    void fixedUpdate(float deltaTime);
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
