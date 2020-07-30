#include "GLW/ShaderProgram.h"

namespace GLW
{

    ShaderProgram::ShaderProgram(const std::string& _vertexShaderPath, const std::string& _fragmentShaderPath)
    {

        GL_CHECK(shaderProgram = glCreateProgram());

        // Create and compile the vertex shader
        CompileShader(vertexShader, GL_VERTEX_SHADER, LoadShaderFromFile(_vertexShaderPath));

        // Create and compile the fragment shader
        CompileShader(fragmentShader, GL_FRAGMENT_SHADER, LoadShaderFromFile(_fragmentShaderPath));

        // Link the vertex and fragment shader into a shader program
        GL_CHECK(glAttachShader(shaderProgram, vertexShader));
        GL_CHECK(glAttachShader(shaderProgram, fragmentShader));
        GL_CHECK(glBindFragDataLocation(shaderProgram, 0, "outColor"));
        GL_CHECK(glLinkProgram(shaderProgram));
        GL_CHECK(glUseProgram(shaderProgram));
    }

    ShaderProgram::~ShaderProgram()
    {
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteProgram(shaderProgram);
    }

    void ShaderProgram::Use()
    {
        GL_CHECK(glUseProgram(shaderProgram));
    }

    void ShaderProgram::SetUniform(const std::string& _uniformKey, const int& _value)
    {
        // Look for uniform in the uniform map
        if (uniformMap.find(_uniformKey) == uniformMap.end())
        {
            // If uniform is not in map create it
            GLuint uniformLocation = GL_CHECK(glGetUniformLocation(shaderProgram, _uniformKey.c_str()));

            if (uniformLocation == -1)
            {
                std::cerr << "Uniform key " << _uniformKey << " not found in shader program";
                throw std::runtime_error("Shader error");
            }
            
            uniformMap.insert(std::make_pair(_uniformKey, uniformLocation));
        }

        GL_CHECK(glUniform1i(uniformMap[_uniformKey], _value));
    }

    void ShaderProgram::SetUniform(const std::string& _uniformKey, const float& _value)
    {
        // Look for uniform in the uniform map
        if (uniformMap.find(_uniformKey) == uniformMap.end())
        {
            // If uniform is not in map create it
            GLuint uniformLocation = GL_CHECK(glGetUniformLocation(shaderProgram, _uniformKey.c_str()));

            if (uniformLocation == -1)
            {
                std::cerr << "Uniform key " << _uniformKey << " not found in shader program";
                throw std::runtime_error("Shader error");
            }
            
            uniformMap.insert(std::make_pair(_uniformKey, uniformLocation));
        }

        GL_CHECK(glUniform1f(uniformMap[_uniformKey], _value));
    }

    void ShaderProgram::SetUniform(const std::string& _uniformKey, const glm::vec3& _value)
    {
        // Look for uniform in the uniform map
        if (uniformMap.find(_uniformKey) == uniformMap.end())
        {
            // If uniform is not in map create it
            GLuint uniformLocation = GL_CHECK(glGetUniformLocation(shaderProgram, _uniformKey.c_str()));

            if (uniformLocation == -1)
            {
                std::cerr << "Uniform key " << _uniformKey << " not found in shader program";
                throw std::runtime_error("Shader error");
            }

            uniformMap.insert(std::make_pair(_uniformKey, uniformLocation));
        }

        GL_CHECK(glUniform3fv(uniformMap[_uniformKey], 1, glm::value_ptr(_value)));
    }

    void ShaderProgram::SetUniform(const std::string& _uniformKey, const glm::vec4& _value)
    {
        // Look for uniform in the uniform map
        if (uniformMap.find(_uniformKey) == uniformMap.end())
        {
            // If uniform is not in map create it
            GLuint uniformLocation = GL_CHECK(glGetUniformLocation(shaderProgram, _uniformKey.c_str()));

            if (uniformLocation == -1)
            {
                std::cerr << "Uniform key " << _uniformKey << " not found in shader program";
                throw std::runtime_error("Shader error");
            }
            
            uniformMap.insert(std::make_pair(_uniformKey, uniformLocation));
        }

        GL_CHECK(glUniform4fv(uniformMap[_uniformKey], 1, glm::value_ptr(_value)));
    }

    void ShaderProgram::SetUniform(const std::string& _uniformKey, const glm::mat4& _value)
    {
        // Look for uniform in the uniform map
        if (uniformMap.find(_uniformKey) == uniformMap.end())
        {
            // If uniform is not in map create it
            GLuint uniformLocation = GL_CHECK(glGetUniformLocation(shaderProgram, _uniformKey.c_str()));

            if (uniformLocation == -1)
            {
                std::cerr << "Uniform key " << _uniformKey << " not found in shader program";
                throw std::runtime_error("Shader error");
            }
            
            uniformMap.insert(std::make_pair(_uniformKey, uniformLocation));
        }

        GL_CHECK(glUniformMatrix4fv(uniformMap[_uniformKey], 1, GL_FALSE, glm::value_ptr(_value)));
    }

    void ShaderProgram::SpecifyAttributeLayout(const AttributeLayout& _attributeLayout)
    {
        int layoutSize = 0;
        for (auto attributeAndNumValues : _attributeLayout)
        {
            layoutSize += std::get<int>(attributeAndNumValues);
        }

        int currentPosition = 0;
        for (auto attributeAndNumValues : _attributeLayout)
        {
            const char* name = std::get<std::string>(attributeAndNumValues).c_str();
            GLint attribLocation = GL_CHECK(glGetAttribLocation(shaderProgram, name));
            if (attribLocation != -1)
            {
                GL_CHECK(glEnableVertexAttribArray(attribLocation));
                GL_CHECK(glVertexAttribPointer(attribLocation,
                    std::get<int>(attributeAndNumValues),
                    GL_FLOAT, GL_FALSE,
                    layoutSize * sizeof(GLfloat),
                    (void*)(currentPosition * sizeof(GLfloat))));
            }
            currentPosition += std::get<int>(attributeAndNumValues);
        }
    }

    void ShaderProgram::BindToUniformBlock(const std::string& _uniformBlockName, unsigned int _bindingPoint)
    {
        unsigned int uniformBlockIndex = glGetUniformBlockIndex(shaderProgram, _uniformBlockName.c_str());
        glUniformBlockBinding(shaderProgram, uniformBlockIndex, _bindingPoint);
    }

    std::string ShaderProgram::LoadShaderFromFile(const std::string& _filename)
    {
        std::ifstream file;
        file.open(_filename.c_str());

        if (!file)
        {
            std::cerr << "Could not find file: " << _filename << std::endl;
            throw std::runtime_error("Could not load shader file");
        }

        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        return stream.str();
    }

    void ShaderProgram::CompileShader(GLuint& _shader, GLenum _shaderType, const std::string& _source)
    {
        // Create and compile the shader
        _shader = glCreateShader(_shaderType);
        const char* cStr = _source.c_str();
        glShaderSource(_shader, 1, &cStr, nullptr);
        glCompileShader(_shader);

        // Check for compilation errors
        GLint isCompiled = 0;
        glGetShaderiv(_shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            // Get the length of the error strings
            GLint maxLength = 0;
            glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &maxLength);

            // Load the error strings into vector
            std::vector<GLchar> errorLog(maxLength);
            glGetShaderInfoLog(_shader, maxLength, &maxLength, &errorLog[0]);

            // Output the errors
            std::cerr << &errorLog[0];

            // Exit with failure.
            glDeleteShader(_shader); // Don't leak the shader.

            throw std::runtime_error("Error compiling shader");
        }
    }

} // namespace GLW