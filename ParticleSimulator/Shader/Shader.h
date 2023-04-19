#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <glm/glm.hpp>

class Shader {
private:
    unsigned int ID;

public:
    Shader(const char *vertexPath, const char *fragmentPath);

    Shader(const Shader &) = delete;
    auto operator=(const Shader &) -> Shader & = delete;
    Shader(Shader &&) = delete;
    auto operator=(Shader &&) -> Shader & = delete;

    virtual ~Shader();

    static void checkCompileErrors(unsigned int shader, const std::string &type);

public:
    void use() const;

    [[maybe_unused]] [[nodiscard]] auto getID() const -> unsigned int;

public:
    [[maybe_unused]] void setBool(const std::string &name, bool value) const;

    [[maybe_unused]] void setInt(const std::string &name, int value) const;

    [[maybe_unused]] void setFloat(const std::string &name, float value) const;

    [[maybe_unused]] void setVec2(const std::string &name, float x, float y) const;

    [[maybe_unused]] void setVec3(const std::string &name, float x, float y, float z) const;

    [[maybe_unused]] void setVec4(const std::string &name, float x, float y, float z, float w) const;

    [[maybe_unused]] void setVec2(const std::string &name, const glm::vec2 &value) const;

    [[maybe_unused]] void setVec3(const std::string &name, const glm::vec3 &value) const;

    [[maybe_unused]] void setVec4(const std::string &name, const glm::vec4 &value) const;

    [[maybe_unused]] void setMat2(const std::string &name, const glm::mat2 &mat) const;

    [[maybe_unused]] void setMat3(const std::string &name, const glm::mat3 &mat) const;

    [[maybe_unused]] void setMat4(const std::string &name, const glm::mat4 &mat) const;
};


#endif //SHADER_H
