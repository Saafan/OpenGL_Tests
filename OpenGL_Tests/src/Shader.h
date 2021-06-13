#pragma once
#include "DefaultNamingConventions.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"


/**
A struct that combines shader sources into a single type.
*/
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
    std::string GeometrySource;
};

class Shader
{
private:
    std::string m_FilePath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
public:
    Shader(const std::string& filePath);
    ~Shader();

    glm::mat4* view = nullptr;
    glm::mat4* proj = nullptr;

    void Bind() const;
    void Unbind() const;

    unsigned int GetProgramID();

    // Set uniforms
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);

    void SetUniform3f(const std::string& name, float v0, float v1, float v2);
    void SetUniform3f(const std::string& name, glm::vec3& vector);

    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniform4f(const std::string& name, glm::vec4& vector);

    void SetUniformMat4f(const std::string& name, glm::mat4& matrix);

    void SetMVPMatrix(glm::mat4& model, glm::mat4& view, glm::mat4& proj);
    void SetModelMatrix(glm::mat4& model);
    void SetViewMatrix(glm::mat4& view);
    void SetProjMatrix(glm::mat4& proj);


    unsigned int GetUniformLocation(const std::string& name);
private:

    /**
        Parse a single shader file to a shader source.

        @param filePath Path to the shader file
        @return ShaderProgramSource The combined shader sources in as ShaderProgramSource
    */
    ShaderProgramSource ParseShader(const std::string filePath);

    /**
        Links the given shaders into a single shader so that it can be bound.

        @param vertexShader Source of the vertexshader as string
        @param fragmentShader Source of the fragmentshader as string
        @return unsigned int An identifier for the newly created shader
    */
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geomeretShader);

    /**
        Compiles a single shader so it can be attached.

        @param type The type of the shader
        @param source The source code of the shader as string
        @return unsigned int An identifier for the compiled shader
    */
    unsigned int CompileShader(unsigned int type, const std::string& source);

    /**
        Return the location of the shader based on uniform name

        @param name The name of the uniform
        @return The location
    */
};