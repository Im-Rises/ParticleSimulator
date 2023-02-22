#include "Scene.h"

Scene::Scene(int display_w, int display_h) : camera(display_w, display_h), particleSimulator(10000000) {
}

Scene::~Scene() {
}

void Scene::update(float deltaTime) {
    camera.update(deltaTime);
    if (isPaused)
    {
        return;
    }
    particleSimulator.update(deltaTime);
}

// void Scene::fixedUpdate(float fixedDeltaTime) {
//     if (isPaused)
//     {
//         return;
//     }
//     particleSimulator.fixedUpdate(fixedDeltaTime);
//}

void Scene::render() {
    particleSimulator.render(camera.getViewMatrix(), camera.getProjectionMatrix());
}

void Scene::updateProjectionMatrix(int display_w, int display_h) {
    camera.updateProjectionMatrix(display_w, display_h);
}
void Scene::togglePause() {
    isPaused = !isPaused;
    particleSimulator.setIsPaused(isPaused);
}
