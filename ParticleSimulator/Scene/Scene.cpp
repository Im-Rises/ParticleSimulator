#include "Scene.h"

Scene::Scene(int display_w, int display_h) : camera(display_w, display_h), particleSimulatorSsbo(1000000) {
}

void Scene::update(float deltaTime) {
    camera.update(deltaTime);
    if (isPaused)
    {
        return;
    }
    particleSimulatorSsbo.update(deltaTime);
}

void Scene::render() {
    particleSimulatorSsbo.render(camera.getViewMatrix(), camera.getProjectionMatrix());
}

void Scene::updateProjectionMatrix(int display_w, int display_h) {
    camera.updateProjectionMatrix(display_w, display_h);
}

void Scene::togglePause() {
    isPaused = !isPaused;
    particleSimulatorSsbo.setIsPaused(isPaused);
}

void Scene::reset() {
    camera.reset();
    particleSimulatorSsbo.reset();
}

auto Scene::getIsPaused() const -> bool {
    return isPaused;
}
