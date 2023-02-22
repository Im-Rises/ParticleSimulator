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
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    auto xPos = (mode->width - display_w) / 2;
    auto yPos = (mode->height - display_h) / 2;
    glfwSetWindowPos(window, xPos, yPos);

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
    printf("OpenGL version: %s\n", getOpenGLVersion());
    printf("GLSL version: %s\n", getGLSLVersion());
    printf("GLFW version: %s\n", getGLFWVersion());
    printf("Glad version: %s\n", getGladVersion());
    printf("ImGui version: %s\n", getImGuiVersion());
    printf("GLM version: %s\n", getGLMVersion());


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

    /* Get mouse position*/
    double x = 0, y = 0;
    InputManager::getMouseMovement(window, x, y);

    // Read mouse inputs and update camera
    if (InputManager::isKeyMouseMovementPressed(window))
    {
        scene->camera.processMouseMovement((float)x, (float)y);
    }

    // Read mouse inputs and update simulator target
    if (InputManager::isKeyMouseSetTargetPressed(window))
    {
        scene->particleSimulator.setPointOfGravity(scene->camera.position, scene->camera.cameraFrontBuffer);
    }
}

void ParticleSimulatorLauncher::handleUi(float deltaTime) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("Window info");
        ImGui::Text("%.3f ms/frame (%.1f FPS)", deltaTime, 1.0f / deltaTime);
        ImGui::Text("Window width: %d", display_w);
        ImGui::Text("Window height: %d", display_h);
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

        ImGui::TextColored(ImVec4(1.0F, 0.0F, 1.0F, 1.0F), "Particle settings");

        ImGui::Text("Fixed update frequency:");
        ImGui::DragFloat("##fixedUpdate", &fixedUpdate, 1.0f, 1.0f, 100.0f);

        ImGui::End();
    }
}

void ParticleSimulatorLauncher::updateGame(float deltaTime) {
    const float fixedDeltaTime = 1.0f / fixedUpdate;
    static float accumulator = 0.0f;
    accumulator += deltaTime;
    while (accumulator >= fixedDeltaTime)
    {
        scene->update(fixedDeltaTime);
        accumulator -= fixedDeltaTime;
    }

    //    scene->update(deltaTime);
}

void ParticleSimulatorLauncher::updateScreen() {
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    scene->updateProjectionMatrix(display_w, display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene->render();
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

    glfwSwapBuffers(window);
}

char* ParticleSimulatorLauncher::getOpenGLVersion() {
    return (char*)glGetString(GL_VERSION);
}

char* ParticleSimulatorLauncher::getGLSLVersion() {
    return (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
}

char* ParticleSimulatorLauncher::getGLFWVersion() {
    char version[10];
    sprintf(version, "%d.%d.%d", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);
    return version;
}

char* ParticleSimulatorLauncher::getGladVersion() {
    return "0.1.36";
}

char* ParticleSimulatorLauncher::getImGuiVersion() {
    return IMGUI_VERSION;
}

char* ParticleSimulatorLauncher::getGLMVersion() {
    char version[10];
    sprintf(version, "%d.%d.%d", GLM_VERSION_MAJOR, GLM_VERSION_MINOR, GLM_VERSION_PATCH);
    return version;
}
