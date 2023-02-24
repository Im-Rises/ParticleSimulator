#include <iostream>
#include "ParticleSimulatorLauncher.h"

// This is a workaround for the Nvidia and AMD drivers to force the use of the GPU instead of the integrated GPU
// This is not a good practice, but it's the only way to force the use of the GPU
// https://stackoverflow.com/questions/68469954/how-to-choose-specific-gpu-when-create-opengl-context

#ifdef _WIN32
#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) uint32_t NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#ifdef __cplusplus
}
#endif
#endif


int main(int argc, char* argv[]) {
    std::cout << PROJECT_NAME << " " << PROJECT_VERSION << std::endl
              << PROJECT_GITHUB << std::endl
              << PROJECT_AUTHOR << "\n"
              << std::endl;
    ParticleSimulatorLauncher particleEmissionLauncher;
    particleEmissionLauncher.start();
    return 0;
}
