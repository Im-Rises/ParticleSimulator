#include "Shader.h"

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>

Shader::Shader(const char* vertexPath, const char* fragmentPath, bool isPath) {
    if (isPath)
    {
        compileFromFiles(vertexPath, fragmentPath);
    }
    else
    {
        compile(vertexPath, fragmentPath);
    }
}

Shader::Shader(const char* vertexSource, const char* fragmentSource, const std::vector<std::string>& varyings, bool isPath) {
    if (isPath)
    {
        compileFromFiles(vertexSource, fragmentSource, varyings);
    }
    else
    {
        compile(vertexSource, fragmentSource, varyings);
    }
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

void Shader::compileFromFiles(const char* vertexPath, const char* fragmentPath) {
    // Retrieve the vertex/fragment source code from filePath
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // ensure stream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream;
        std::stringstream fShaderStream;

        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // close file handlers
        vShaderFile.close();
        fShaderFile.close();

        // convert stream into string and create shader
        std::string const vertexCodeStr = vShaderStream.str();
        const char* vertexCode = vertexCodeStr.c_str();

        std::string const fragmentCodeStr = fShaderStream.str();
        const char* fragmentCode = fragmentCodeStr.c_str();

        compile(vertexCode, fragmentCode);
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
}

void Shader::compileFromFiles(const char* vertexPath, const char* fragmentPath, const std::vector<std::string>& varyings) {
    // Retrieve the vertex/fragment source code from filePath
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // ensure stream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream;
        std::stringstream fShaderStream;

        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // close file handlers
        vShaderFile.close();
        fShaderFile.close();

        // convert stream into string and create shader
        std::string const vertexCodeStr = vShaderStream.str();
        const char* vertexCode = vertexCodeStr.c_str();

        std::string const fragmentCodeStr = fShaderStream.str();
        const char* fragmentCode = fragmentCodeStr.c_str();

        compile(vertexCode, fragmentCode, varyings);
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
}

void Shader::compile(const char* vertexSource, const char* fragmentSource) {
    const GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    const GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::compile(const char* vertexSource, const char* fragmentSource, const std::vector<std::string>& varyings) {
    std::vector<const char*> varyingsCStr;
    for (const std::string& varying : varyings)
    {
        varyingsCStr.push_back(varying.c_str());
    }

    const GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    const GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glTransformFeedbackVaryings(ID, static_cast<GLsizei>(varyingsCStr.size()), varyingsCStr.data(), GL_INTERLEAVED_ATTRIBS);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::checkCompileErrors(unsigned int shader, const std::string& type) {
    int success = 0;
    std::array<char, 1024> infoLog{};
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success == 0)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog.data());
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                      << infoLog.data()
                      << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (success == 0)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog.data());
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                      << infoLog.data()
                      << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void Shader::use() const {
    glUseProgram(ID);
}

[[maybe_unused]] auto Shader::getID() const -> unsigned int {
    return ID;
}

[[maybe_unused]] void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

[[maybe_unused]] void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

[[maybe_unused]] void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

[[maybe_unused]] void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

[[maybe_unused]] void Shader::setVec2(const std::string& name, float x, float y) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

[[maybe_unused]] void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

[[maybe_unused]] void Shader::setVec3(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

[[maybe_unused]] void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

[[maybe_unused]] void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

[[maybe_unused]] void Shader::setMat2(const std::string& name, const glm::mat2& mat) const {
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

[[maybe_unused]] void Shader::setMat3(const std::string& name, const glm::mat3& mat) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

[[maybe_unused]] void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
