#ifndef CUBE_H
#define CUBE_H

#include <array>

#include "../Entity.h"
#include "../../../Shader/Shader.h"
#include <glad/glad.h>

class Cube : public Entity {
private:
    static const std::array<GLfloat, 108> Vertices;

    static const char* const VertexShaderSource;
    static const char* const FragmentShaderSource;

    GLuint VAO, VBO;

    Shader shader;

public:
    Cube();

    Cube(const Cube&) = delete;
    auto operator=(const Cube&) -> Cube& = delete;
    Cube(Cube&&) = delete;
    auto operator=(Cube&&) -> Cube& = delete;

    ~Cube() override;

public:
    void update(const float& deltaTime) override;

    void render(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjectionMatrix) override;
};


#endif // CUBE_H
