#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-vararg"

#include "ParticleSimulatorLauncher.h"

#include "InputManager.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <cstdlib>

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include "Scene/Scene.h"
#include <iostream>
#include <chrono>
#include <thread>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#ifdef __EMSCRIPTEN__
#include "imgui/libs/emscripten/emscripten_mainloop_stub.h"
#include <emscripten/html5.h>
#endif

static void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

ParticleSimulatorLauncher::ParticleSimulatorLauncher() {
    glfwSetErrorCallback(glfw_error_callback);
    if (glfwInit() == 0)
        exit(1);

// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    const char* glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // 3.0+ only
#endif

    // Set display size
#ifdef __EMSCRIPTEN__
    // According to canvas
    emscripten_get_canvas_element_size("#canvas", &displayWidth, &displayHeight);
#else
    // According to init windowSize
    displayWidth = windowWidth;
    displayHeight = windowHeight;
#endif

    // Create window with graphics context
    window = glfwCreateWindow(displayWidth, displayHeight, PROJECT_NAME.data(), nullptr, nullptr);
    if (window == nullptr)
        exit(1);
    glfwMakeContextCurrent(window);
    //    glfwSwapInterval(1); // Enable vsync
    glfwSwapInterval(0); // Disable vsync
    //    glfwWindowHint(GLFW_REFRESH_RATE, 0); // Disable refresh rate
    //    glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE); // Disable refresh rate
    //    glfwWindowHint(GLFW_REFRESH_RATE, 60);

    // Callbacks
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, InputManager::key_callback);

#ifdef __EMSCRIPTEN__
    // Initialize OpenGL loader
    if (gladLoadGLES2Loader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
        exit(1);
#else
    // Initialize OpenGL loader
    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
        exit(1);
#endif

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0)
    {
        style.WindowRounding = 0.0F;
        style.Colors[ImGuiCol_WindowBg].w = 1.0F;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

#ifdef __EMSCRIPTEN__
    // Register emscripten callbacks
    emscripten_set_touchstart_callback("#canvas", (void*)&InputManager::dragMovementData, true, InputManager::touchStart_callback);
    emscripten_set_touchmove_callback("#canvas", (void*)&InputManager::dragMovementData, true, InputManager::touchMove_callback);
    emscripten_set_touchend_callback("#canvas", (void*)&InputManager::dragMovementData, true, InputManager::touchEnd_callback);
#endif

#ifndef __EMSCRIPTEN__
    centerWindow();
#endif

    // Setup OpenGL state
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //    glEnable(GL_MULTISAMPLE);
    //    glEnable(GL_POINT_SMOOTH); // Deprecated
    //    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    //    glPointSize(1.0f); // Not working in OpenGL ES 3.0

    // Same line as above but with C++ string
    std::cout << "OpenGL vendor: " << getOpenGLVendor() << std::endl
              << "OpenGL version: " << getOpenGLVersion() << std::endl
              << "GLSL version: " << getGLSLVersion() << std::endl
              << "GLFW version: " << getGLFWVersion() << std::endl
              << "Glad version: " << getGladVersion() << std::endl
              << "ImGui version: " << getImGuiVersion() << std::endl
              << "GLM version: " << getGLMVersion() << std::endl;
}

ParticleSimulatorLauncher::~ParticleSimulatorLauncher() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void ParticleSimulatorLauncher::start() {
    // Create the scene
    scene = std::make_unique<Scene>(displayWidth, displayHeight);

#ifdef _WIN32
    timeBeginPeriod(1);
#endif

    // Variables for the main loop
    std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
    float deltaTime = 0.0F;

#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (glfwWindowShouldClose(window) == 0)
#endif
    {
        auto currentTime = std::chrono::high_resolution_clock::now();

        deltaTime = std::chrono::duration<float>(currentTime - previousTime).count();

        auto startMs = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());

        handleInputs();

        handleUi(deltaTime);

        updateGame(fixedDeltaTime);

        updateScreen();

        auto endMs = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
        auto delayMs = fixedDeltaTime - std::chrono::duration_cast<std::chrono::duration<float>>(endMs - startMs).count();
        if (delayMs > 0.0F)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delayMs * 1000.0F)));
        }

        previousTime = currentTime;
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

#ifdef _WIN32
    timeEndPeriod(1);
#endif
}

void ParticleSimulatorLauncher::handleInputs() {
    glfwPollEvents();

    /* Read keyboard inputs and update states (buffers) */
    if (InputManager::isLeftKeyPressed(window))
        scene->camera.moveLeft();

    if (InputManager::isRightKeyPressed(window))
        scene->camera.moveRight();

    if (InputManager::isForwardKeyPressed(window))
        scene->camera.moveForward();

    if (InputManager::isBackwardKeyPressed(window))
        scene->camera.moveBackward();

    if (InputManager::isUpKeyPressed(window))
        scene->camera.moveUp();

    if (InputManager::isDownKeyPressed(window))
        scene->camera.moveDown();

    /* Read and update mouse controls */
    // Get mouse position or drag position
    double posX = 0, posY = 0;
#ifdef __EMSCRIPTEN__
    if (InputManager::dragMovementData.isUsingDrag)
    {
        posX = InputManager::dragMovementData.dragX;
        posY = InputManager::dragMovementData.dragY;
    }
    else
    {
#endif
        InputManager::getMousePosition(window, posX, posY);
#ifdef __EMSCRIPTEN__
    }
#endif

    // Get movement delta
    double mouseDeltaX = 0, mouseDeltaY = 0;
    calculateMouseMovement(posX, posY, mouseDeltaX, mouseDeltaY);

    // Read mouse inputs and update camera
    if (InputManager::isKeyMouseMovementPressed(window))
    {
        scene->camera.processMouseMovement(static_cast<float>(mouseDeltaX), static_cast<float>(mouseDeltaY));
    }

    // Update particle simulator attractor if mouse is pressed or dragging
    bool const isAttracting = InputManager::isKeyMouseSetAttractorPressed(window);
    scene->particleSimulator.setIsAttracting(isAttracting);
    mousePositionWorld = projectMouse(posX, posY);
    scene->particleSimulator.setAttractorPosition(mousePositionWorld);
}

void ParticleSimulatorLauncher::handleUi(float deltaTime) {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

#ifndef __EMSCRIPTEN__
    if (!isFullscreen)
    {
#endif
        {
#ifdef __EMSCRIPTEN__
            static bool isCollapsed = true;
            ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiCond_Once);
            ImGui::SetNextWindowCollapsed(isCollapsed, ImGuiCond_Once);
#endif
            ImGui::Begin("Information");
            ImGui::Text("Author: %s", PROJECT_AUTHOR.data());
            ImGui::Text("Project: %s", PROJECT_NAME.data());
            ImGui::Text("Version: %s", PROJECT_VERSION.data());
            ImGui::Text("Author: %s", PROJECT_AUTHOR.data());
            ImGui::Text("%.3f ms/frame (%.1f FPS)", deltaTime, 1.0F / deltaTime);
            ImGui::Text("Window width: %d", displayWidth);
            ImGui::Text("Window height: %d", displayHeight);
            ImGui::Text("GPU: %s", getOpenGLVendor().data());
            ImGui::Text("OpenGL version: %s", getOpenGLVersion().data());
            ImGui::Text("GLSL version: %s", getGLSLVersion().data());
            ImGui::End();
        }

        // NOT WORKING
        //        {
        //            ImGui::Begin("View settings");
        // #ifndef __EMSCRIPTEN__
        //            static bool wireframe = false;
        //            ImGui::Checkbox("Wireframe", &wireframe);
        //            if (wireframe)
        //            {
        //                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //            }
        //            else
        //            {
        //                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //            }
        //            ImGui::NewLine();
        // #endif
        //            ImGui::NewLine();
        //            static bool useVsync = false;
        //            static float fps = 60.0F;
        //            ImGui::Checkbox("Use VSync", &useVsync);
        //            if (ImGui::IsItemClicked())
        //            {
        //                if (useVsync)
        //                {
        //                    glfwSwapInterval(1); // Enable vsync
        //                }
        //                else
        //                {
        //                    glfwSwapInterval(0); // Disable vsync
        //                    glfwWindowHint(GLFW_REFRESH_RATE, fps);
        //                }
        //            }
        //            if (!useVsync)
        //            {
        //                ImGui::DragFloat("Frame per second", &fps, 0.1F, 1.0F, 60.0F);
        //                ImGui::Button("Validate");
        //                if (ImGui::IsItemClicked())
        //                {
        //                    glfwWindowHint(GLFW_REFRESH_RATE, fps);
        //                }
        //            }
        //            ImGui::End();
        //        }

        {
#ifdef __EMSCRIPTEN__
            static bool isCollapsed = true;
            ImGui::SetNextWindowPos(ImVec2(5, 25), ImGuiCond_Once);
            ImGui::SetNextWindowCollapsed(isCollapsed, ImGuiCond_Once);
#endif
            ImGui::Begin("Camera settings");
            //        #ifndef __EMSCRIPTEN__
            //        ImGui::TextColored(ImVec4(1.0F, 0.0F, 1.0F, 1.0F), "View settings");
            //        static bool wireframe = false;
            //        ImGui::Checkbox("Wireframe", &wireframe);
            //        if (wireframe)
            //        {
            //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            //        }
            //        else
            //        {
            //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            //        }
            //        ImGui::NewLine();
            //        #endif

            ImGui::Text("Position:");
            ImGui::DragFloat3("##position", reinterpret_cast<float*>(&scene->camera.position));

            ImGui::Text("Reset position:");
            ImGui::DragFloat3("##resetPosition", reinterpret_cast<float*>(&scene->camera.initPosition));
            ImGui::Button("Reset position");
            if (ImGui::IsItemClicked())
            {
                scene->camera.reset();
            }

            ImGui::NewLine();
            ImGui::Text("Pitch:");
            ImGui::Checkbox("Pitch constrained", &scene->camera.constrainPitch);
            ImGui::DragFloat("##pitch", &scene->camera.pitch);

            ImGui::Text("Yaw:");
            ImGui::DragFloat("##yaw", &scene->camera.yaw);

            ImGui::NewLine();
            ImGui::Text("FOV:");
            ImGui::DragFloat("##fov", &scene->camera.fov);

            ImGui::NewLine();
            ImGui::Text("Near plane:");
            ImGui::DragFloat("##near", &scene->camera.nearPlane);

            ImGui::Text("Far plane:");
            ImGui::DragFloat("##far", &scene->camera.farPlane);

            ImGui::NewLine();
            ImGui::Text("Speed:");
            ImGui::DragFloat("##speed", &scene->camera.movementSpeed);

            ImGui::Text("Sensitivity: ");
            ImGui::DragFloat("##sensitivity", &scene->camera.rotationSpeed, 0.1F);

            ImGui::End();
        }

        {
#ifdef __EMSCRIPTEN__
            static bool isCollapsed = true;
            ImGui::SetNextWindowPos(ImVec2(5, 45), ImGuiCond_Once);
            ImGui::SetNextWindowCollapsed(isCollapsed, ImGuiCond_Once);
#endif
            ImGui::Begin("Particle simulator settings");

            ImGui::Text("Particle count: %s", std::to_string(scene->particleSimulator.getParticlesCount()).c_str());
            static int particlesCount = static_cast<int>(scene->particleSimulator.getParticlesCount());
            ImGui::DragInt("##particlesCount", &particlesCount, 1, 1, MAX_PARTICLES_COUNT);
            ImGui::Button("Validate##ParticlesCountSetterButton");
            if (ImGui::IsItemClicked())
            {
                scene->particleSimulator.setParticlesCount(particlesCount);
            }
            ImGui::NewLine();

            ImGui::Text("Reset simulation:");
            ImGui::Button("Reset##ResetBtn");
            if (ImGui::IsItemClicked())
            {
                resetScene();
            }
            ImGui::NewLine();

            ImGui::Text("Toggle pause:");
            ImGui::Button(scene->getIsPaused() ? "Resume##TogglePAuseBtn" : "Pause##TogglePAuseBtn");
            if (ImGui::IsItemClicked())
            {
                scene->togglePause();
            }
            ImGui::NewLine();

            ImGui::Text("Spawn position:");
            ImGui::DragFloat3("##spawnPosition", reinterpret_cast<float*>(&scene->particleSimulator.position));
            ImGui::NewLine();

            ImGui::Text("Spawn radius:");
            ImGui::DragFloat("##spawnRadius", &scene->particleSimulator.spawnRadius, 0.1F, 0.1F, 100.0F);
            ImGui::NewLine();

            ImGui::Text("Particle mass:");
            ImGui::DragFloat("##particleMass", &scene->particleSimulator.particleMass, 0.1F, 0.1F, 100.0F);
            ImGui::NewLine();

            ImGui::Text("Attractor mass:");
            ImGui::DragFloat("##attractorMass", &scene->particleSimulator.attractorMass, 0.1F, 0.1F, 100.0F);
            ImGui::NewLine();

            ImGui::Text("Gravity:");
            ImGui::DragFloat("##gravity", &scene->particleSimulator.gravity, 0.1F, 0.1F, 100.0F);
            ImGui::NewLine();

            ImGui::Text("Softening:");
            ImGui::DragFloat("##softening", &scene->particleSimulator.softening, 0.1F, 0.1F, 100.0F);
            ImGui::NewLine();

            ImGui::Text("Damping:");
            ImGui::DragFloat("##damping", &scene->particleSimulator.damping, 0.0F, 0.0F, 1.0F);

            ImGui::End();
        }

        {
#ifdef __EMSCRIPTEN__
            static bool isCollapsed = true;
            ImGui::SetNextWindowPos(ImVec2(5, 65), ImGuiCond_Once);
            ImGui::SetNextWindowCollapsed(isCollapsed, ImGuiCond_Once);
#endif
            ImGui::Begin("Mouse controls");

            ImGui::Text("Is attracting: %s", scene->particleSimulator.getIsAttracting() ? "true" : "false");

            ImGui::Text("Mouse position world:");
            ImGui::Text("X: %f", mousePositionWorld.x);
            ImGui::SameLine();
            ImGui::Text("Y: %f", mousePositionWorld.y);
            ImGui::SameLine();
            ImGui::Text("Z: %f", mousePositionWorld.z);

            ImGui::Text("Attractor distance from camera:");
            ImGui::DragFloat("##attractorDistance", &attractorDistance, 0.1F, 0.0F, 100.0F);

            ImGui::End();
        }
#ifndef __EMSCRIPTEN__
    }
#endif

    ImGui::Render();

    // Prevent ImGui from stealing focus on start
    static bool disableImGuiFocusOnStart = true;
    if (disableImGuiFocusOnStart)
    {
        ImGui::SetWindowFocus(nullptr);
        disableImGuiFocusOnStart = false;
    }
}

// void ParticleSimulatorLauncher::fixedUpdateGame(float deltaTime) {
//     scene->fixedUpdate(deltaTime);
// }

void ParticleSimulatorLauncher::updateGame(float deltaTime) {
    scene->update(deltaTime);
}

void ParticleSimulatorLauncher::updateScreen() {
    if (!isMinimized())
        updateViewport();

    clearScreen();
    scene->render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    const ImGuiIO& io = ImGui::GetIO();
    if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(window);
}

void ParticleSimulatorLauncher::resetScene() {
    scene->reset();
}

void ParticleSimulatorLauncher::toggleScenePause() {
    scene->togglePause();
}

void ParticleSimulatorLauncher::updateViewport() {
#ifdef __EMSCRIPTEN__
    emscripten_get_canvas_element_size("#canvas", &displayWidth, &displayHeight);
#else
    glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
    if (!isFullscreen)
    {
        windowWidth = displayWidth;
        windowHeight = displayHeight;
    }
#endif
    scene->updateProjectionMatrix(displayWidth, displayHeight);

    glViewport(0, 0, displayWidth, displayHeight);
}


void ParticleSimulatorLauncher::centerWindow() {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    windowPosX = (mode->width - displayWidth) / 2;
    windowPosY = (mode->height - displayHeight) / 2;
    glfwSetWindowPos(window, windowPosX, windowPosY);
}

void ParticleSimulatorLauncher::toggleFullscreen() {
#ifndef __EMSCRIPTEN__
    if (isFullscreen)
    {
        glfwSetWindowMonitor(window, nullptr, 0, 0, windowWidth, windowHeight, 0);
        displayWidth = windowWidth;
        displayHeight = windowHeight;
        glfwSetWindowPos(window, windowPosX, windowPosY);
        isFullscreen = false;
    }
    else
    {
        glfwGetWindowPos(window, &windowPosX, &windowPosY);
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        isFullscreen = true;
    }
#endif
}


void ParticleSimulatorLauncher::clearScreen() const {
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
        clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

auto ParticleSimulatorLauncher::isMinimized() const -> bool {
    return glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0;
}

void ParticleSimulatorLauncher::calculateMouseMovement(const double& xMouse, const double& yMouse, double& xMovement,
    double& yMovement) {
    static double lastMouseX = 0.0;
    static double lastMouseY = 0.0;

    xMovement = xMouse - lastMouseX;
    yMovement = lastMouseY - yMouse;

    lastMouseX = xMouse;
    lastMouseY = yMouse;
}

auto ParticleSimulatorLauncher::projectMouse(const double& xMouse, const double& yMouse) -> glm::vec3 {
    // Convert the mouse coordinates from screen space to NDC space
    float const normalized_x = (2.0F * static_cast<float>(xMouse)) / static_cast<float>(displayWidth) - 1.0F;
    float const normalized_y = 1.0F - (2.0F * static_cast<float>(yMouse)) / static_cast<float>(displayHeight);

    // Create a vector representing the mouse coordinates in NDC space
    glm::vec4 const mouse_ndc(normalized_x, normalized_y, -1.0F, 1.0F);

    // Convert the mouse coordinates from NDC space to world space
    glm::mat4 const inverse_projection = glm::inverse(scene->camera.getProjectionMatrix());
    glm::mat4 const inverse_view = glm::inverse(scene->camera.getViewMatrix());
    glm::vec4 mouse_world = inverse_projection * mouse_ndc;
    mouse_world = mouse_world / mouse_world.w;
    mouse_world = inverse_view * mouse_world;

    // Calculate the direction from the camera position to the mouse position
    glm::vec3 const camera_to_mouse = glm::normalize(glm::vec3(mouse_world) - scene->camera.position);

    // Use the direction to update the position of an object in the 3D environment
    return scene->camera.position + camera_to_mouse * attractorDistance;
}

auto ParticleSimulatorLauncher::getOpenGLVendor() -> std::string_view {
    return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
}

auto ParticleSimulatorLauncher::getOpenGLVersion() -> std::string_view {
    return reinterpret_cast<const char*>(glGetString(GL_VERSION));
}

auto ParticleSimulatorLauncher::getGLSLVersion() -> std::string_view {
    return reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
}

auto ParticleSimulatorLauncher::getGLFWVersion() -> std::string {
    return std::to_string(GLFW_VERSION_MAJOR) + "." + std::to_string(GLFW_VERSION_MINOR) + "." +
           std::to_string(GLFW_VERSION_REVISION);
}

auto ParticleSimulatorLauncher::getGladVersion() -> std::string_view {
    return "0.1.36";
}

auto ParticleSimulatorLauncher::getImGuiVersion() -> std::string {
    return IMGUI_VERSION;
}

auto ParticleSimulatorLauncher::getGLMVersion() -> std::string {
    return std::to_string(GLM_VERSION_MAJOR) + "." + std::to_string(GLM_VERSION_MINOR) + "." +
           std::to_string(GLM_VERSION_PATCH);
}

#pragma clang diagnostic pop
