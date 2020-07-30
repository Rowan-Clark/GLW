#ifndef _SHADER_PROGRAM_H_
#define _SHADER_PROGRAM_H_

#include <exception>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "AttributeLayout.h"
#include "CheckOpenGLError.h"

namespace GLW
{

    class ShaderProgram
    {
    public:
        ShaderProgram(const std::string& _vertexShaderPath, const std::string& _fragmentShaderPath);
        ~ShaderProgram();

        using ShaderProgramObj = std::unique_ptr <ShaderProgram>;
        static ShaderProgramObj Make(const std::string& _vertexShaderPath, const std::string& _fragmentShaderPath)
        {
            return std::make_unique< ShaderProgram>(_vertexShaderPath, _fragmentShaderPath);
        }

        void Use();

        void SetUniform(const std::string& _uniformKey, const int& _value);
        void SetUniform(const std::string& _uniformKey, const float& _value);
        void SetUniform(const std::string& _uniformKey, const glm::vec3& _value);
        void SetUniform(const std::string& _uniformKey, const glm::vec4& _value);
        void SetUniform(const std::string& _uniformKey, const glm::mat4& _value);

        void SpecifyAttributeLayout(const AttributeLayout& _attributeLayout);

        void BindToUniformBlock(const std::string& _uniformBlockName, unsigned int _bindingPoint);

    protected:

        GLuint shaderProgram;

        GLuint vertexShader;
        GLuint fragmentShader;

        std::map <const std::string, GLuint> uniformMap;
        std::map <const std::string, GLuint> uniformBlockMap;

        std::string LoadShaderFromFile(const std::string& _filePath);

        void CompileShader(GLuint& _shader, GLenum _shaderType, const std::string& _source);
    };

    using ShaderProgramObj = ShaderProgram::ShaderProgramObj;

}

#endif // _SHADER_PROGRAM_H_
