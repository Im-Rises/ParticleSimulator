#include <iostream>
#include "ParticleSimulatorLauncher.h"

int main(int argc, char* argv[]) {
    std::cout << PROJECT_NAME << " " << PROJECT_VERSION << std::endl
              << PROJECT_GITHUB << std::endl
              << PROJECT_AUTHOR << std::endl;
    ParticleSimulatorLauncher particleEmissionLauncher;
    particleEmissionLauncher.start();
    return 0;
}
