#include "ParticleSimulatorLauncher.h"

#include <glad/glad.h>

#include "Scene/Scene.h"
#include "InputManager.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <cstdio>
#include <cstdlib>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <iostream>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

ParticleSimulatorLauncher::ParticleSimulatorLauncher() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        exit(1);

// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    const char* glsl_version = "#version 310";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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

    // Create window with graphics context
    window = glfwCreateWindow(display_w, display_h, PROJECT_NAME.data(), NULL, NULL);
    if (window == NULL)
        exit(1);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Callbacks
    glfwSetKeyCallback(window, InputManager::key_callback);

    // Center window
    centerWindow();

    // Initialize OpenGL loader
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        exit(1);

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
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Print OpenGL version
    printf("OpenGL vendor: %s\nOpenGL version: %s\nGLSL version: %s\nGLFW version: %s\n"
           "Glad version: %s\nImGui version: %s\nGLM version: %s\n",
        getOpenGLVendor().data(),
        getOpenGLVersion().data(),
        getGLSLVersion().data(),
        getGLFWVersion().data(),
        getGladVersion().data(),
        getImGuiVersion().data(),
        getGLMVersion().data());

    // Setup OpenGL state
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //    glEnable(GL_POINT_SMOOTH); // Deprecated
    glPointSize(1.0f);
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
    scene = std::make_unique<Scene>(display_w, display_h);

    // Variables for the main loop
    float deltaTime;

#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = NULL;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        deltaTime = ImGui::GetIO().DeltaTime;

        handleInputs();

        handleUi(deltaTime);

        updateGame(deltaTime);

        updateScreen();
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif
}

void ParticleSimulatorLauncher::handleInputs() {
    glfwPollEvents();

    if (InputManager::isFullscreenKeyPressed(window))
        toggleFullscreen();

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

    if (InputManager::isPauseKeyPressed(window))
        scene->togglePause();

    if (InputManager::isResetKeyPressed(window))
        scene->reset();

    /* Get mouse position*/
    double mouseX = 0, mouseY = 0;
    InputManager::getMousePosition(window, mouseX, mouseY);

    /* Get mouse delta */
    double mouseDeltaX = 0, mouseDeltaY = 0;
    calculateMouseMovement(mouseX, mouseY, mouseDeltaX, mouseDeltaY);

    // Read mouse inputs and update camera
    if (InputManager::isKeyMouseMovementPressed(window))
    {
        scene->camera.processMouseMovement((float)mouseDeltaX, (float)mouseDeltaY);
    }

    // Read mouse inputs and update particle simulator target
    bool isTargeting = InputManager::isKeyMouseSetTargetPressed(window);
    scene->particleSimulator.setIsTargeting(isTargeting);
    mousePositionWorld = projectMouse(mouseX, mouseY);
    scene->particleSimulator.setTarget(mousePositionWorld);
}

void ParticleSimulatorLauncher::handleUi(float deltaTime) {
    if (isFullscreen || isWindowMinimized())
        return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("Window info");
        ImGui::Text("%.3f ms/frame (%.1f FPS)", deltaTime, 1.0f / deltaTime);
        ImGui::Text("Window width: %d", display_w);
        ImGui::Text("Window height: %d", display_h);
        ImGui::Text("GPU: %s", getOpenGLVendor().data());
        ImGui::Text("OpenGL version: %s", getOpenGLVersion().data());
        ImGui::Text("GLSL version: %s", getGLSLVersion().data());
        ImGui::End();
    }

    {
        ImGui::Begin("Camera settings");

        static bool wireframe = false;
        ImGui::TextColored(ImVec4(1.0F, 0.0F, 1.0F, 1.0F), "View settings");
        ImGui::Checkbox("Wireframe", &wireframe);
        if (wireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        ImGui::NewLine();

        ImGui::TextColored(ImVec4(1.0F, 0.0F, 1.0F, 1.0F), "Camera settings");

        ImGui::Text("Position:");
        ImGui::DragFloat3("##position", (float*)&scene->camera.position);

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
        ImGui::DragFloat("##sensitivity", &scene->camera.rotationSpeed, 0.1f);

        ImGui::End();
    }

    {
        ImGui::Begin("Particle simulator settings");

        ImGui::Text("Particle count: %s", std::to_string(scene->particleSimulator.getParticleCount()).c_str());
        ImGui::NewLine();

        ImGui::TextColored(ImVec4(1.0F, 0.0F, 1.0F, 1.0F), "Particle settings");

        ImGui::Text("Fixed update frequency:");
        ImGui::DragFloat("##fixedUpdate", &fixedUpdate, 1.0f, 1.0f, 1000.0f);

        ImGui::Text("Reset simulation:");
        ImGui::SameLine();
        ImGui::Button("Reset##ResetBtn");
        if (ImGui::IsItemClicked())
        {
            scene->reset();
        }

        ImGui::Text("Spawn position:");
        ImGui::DragFloat3("##spawnPosition", (float*)&scene->particleSimulator.position);

//        ImGui::Text("Toggle pause:");
//        ImGui::SameLine();
//        ImGui::Button(scene->getIsPaused() ? "Resume##TogglePAuseBtn" : "Pause##TogglePAuseBtn");
//        if (ImGui::IsItemClicked())
//        {
//            scene->togglePause();
//        }

        ImGui::End();
    }

    {
        ImGui::Begin("Mouse controls");

        ImGui::Text("Is targeting: %s", scene->particleSimulator.getIsTargeting() ? "true" : "false");

        ImGui::Text("Mouse position world:");
        ImGui::Text("X: %f", mousePositionWorld.x);
        ImGui::SameLine();
        ImGui::Text("Y: %f", mousePositionWorld.y);
        ImGui::SameLine();
        ImGui::Text("Z: %f", mousePositionWorld.z);

        ImGui::Text("Target distance:");
        ImGui::DragFloat("##targetDistance", &targetDistance, 0.1f, 0.0f, 100.0f);

        ImGui::End();
    }

    ImGui::Render();
}

void ParticleSimulatorLauncher::updateGame(float deltaTime) {
    //    const float fixedDeltaTime = 1.0f / fixedUpdate;
    //    static float accumulator = 0.0f;
    //    accumulator += deltaTime;
    //    while (accumulator >= fixedDeltaTime)
    //    {
    //        scene->update(fixedDeltaTime);
    //        accumulator -= fixedDeltaTime;
    //    }
    scene->update(deltaTime);
}

void ParticleSimulatorLauncher::updateScreen() {
    if (!isWindowMinimized())
    {
        int screenWidth, screenHeight;
        glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
        scene->updateProjectionMatrix(screenWidth, screenHeight);
        glViewport(0, 0, screenWidth, screenHeight);

        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene->render();

        if (!isFullscreen)
        {
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            ImGuiIO& io = ImGui::GetIO();
            (void)io;
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }
        }
    }
    glfwSwapBuffers(window);
}

void ParticleSimulatorLauncher::centerWindow() {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    auto xPos = (mode->width - display_w) / 2;
    auto yPos = (mode->height - display_h) / 2;
    glfwSetWindowPos(window, xPos, yPos);
}

void ParticleSimulatorLauncher::toggleFullscreen() {
    if (isFullscreen)
    {
        glfwSetWindowMonitor(window, NULL, 0, 0, 1280, 720, 0);
        centerWindow();
        isFullscreen = false;
    }
    else
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        isFullscreen = true;
    }
}

bool ParticleSimulatorLauncher::isWindowMinimized() {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return width == 0 || height == 0;
}

void ParticleSimulatorLauncher::calculateMouseMovement(const double& xMouse, const double& yMouse, double& xMovement, double& yMovement) {
    static double lastMouseX = 0.0;
    static double lastMouseY = 0.0;

    xMovement = xMouse - lastMouseX;
    yMovement = lastMouseY - yMouse;

    lastMouseX = xMouse;
    lastMouseY = yMouse;
}

glm::vec3 ParticleSimulatorLauncher::projectMouse(const double& xMouse, const double& yMouse) {
    glm::vec2 windowSpaceCoords = glm::vec2(xMouse, yMouse);
    windowSpaceCoords = glm::vec2(windowSpaceCoords.x, display_h - windowSpaceCoords.y);
    glm::vec2 normalizedDeviceCoords = (windowSpaceCoords / glm::vec2(display_w, display_h)) * 2.0f - glm::vec2(1.0f);
    glm::vec3 dir = calculateWorldSpaceRay(glm::inverse(scene->camera.getProjectionMatrix()), glm::inverse(scene->camera.getViewMatrix()), normalizedDeviceCoords);
    return scene->camera.position + dir * targetDistance;
}

glm::vec3 ParticleSimulatorLauncher::calculateWorldSpaceRay(glm::mat4 inverseProjection, glm::mat4 inverseView, glm::vec2 normalizedDeviceCoords) {
    glm::vec4 rayEye = glm::vec4(normalizedDeviceCoords, -1.0f, 1.0f) * inverseProjection;
    rayEye.z = -1.0f;
    rayEye.w = 0.0f;
    return glm::normalize(glm::vec3(rayEye * inverseView));
}


std::string_view ParticleSimulatorLauncher::getOpenGLVendor() {
    return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
}

std::string_view ParticleSimulatorLauncher::getOpenGLVersion() {
    return reinterpret_cast<const char*>(glGetString(GL_VERSION));
}

std::string_view ParticleSimulatorLauncher::getGLSLVersion() {
    return reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
}

std::string ParticleSimulatorLauncher::getGLFWVersion() {
    char version[10];
    sprintf(version, "%d.%d.%d", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);
    return { version };
}

std::string_view ParticleSimulatorLauncher::getGladVersion() {
    return "0.1.36";
}

std::string ParticleSimulatorLauncher::getImGuiVersion() {
    return IMGUI_VERSION;
}

std::string ParticleSimulatorLauncher::getGLMVersion() {
    char version[10];
    sprintf(version, "%d.%d.%d", GLM_VERSION_MAJOR, GLM_VERSION_MINOR, GLM_VERSION_PATCH);
    return { version };
}
