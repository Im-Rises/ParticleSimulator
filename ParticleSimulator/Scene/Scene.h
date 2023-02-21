#ifndef SCENE_H
#define SCENE_H

#include "Entity/Cube/Cube.h"
#include "Camera/Camera.h"
#include "Entity/ParticleSimulator/ParticleSimulator.h"

class Scene {
private:
    bool isPaused = false;

public:
    Camera camera;

private:
//    Cube cube;
    ParticleSimulator particleSimulator;

public:
    Scene(int display_w, int display_h);
    ~Scene();
    void update(float deltaTime);
    void render();

public:
    void updateProjectionMatrix(int display_w, int display_h);
    void togglePause();
};

#endif // SCENE_H
