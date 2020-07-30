// GlWrap.h
//
// This class can be inherited to provide a derived class with OpenGL functionality
// enabling a user to avoid making direct OpenGL hardware calls whilst managing resources on the graphics card
// and providing an Object Orientated interface through which to access the OpenGL functionality

#ifndef _GLWRAP_H_
#define _GLWRAP_H_

// Standard library includes
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <memory>

// Other library includes
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/matrix_major_storage.hpp>
#include <SOIL2/SOIL2.h>

// Project includes
#include "AttributeLayout.h"
#include "CheckOpenGLError.h"
#include "ShaderProgram.h"
#include "VertexArray.h"

namespace GLW
{
	class GlWrap
	{
	protected:
		GlWrap();
		~GlWrap();

		// -------- Basic OpenGL Functions
		// 
		void SetClearColor(float _red, float _green, float _blue, float _alpha);
		void ClearFramebuffer();

		/*********************************
		************* Texture ************
		*********************************/
		// Generate a texture from an image file which can be referenced by a texture key string
		void LoadTexture(const std::string& _textureKey, const std::string& _imagePath);
		// Set texture to be used for draw calls
		void SetActiveTexture(const std::string& _textureKey);
		// Used to send multiple textures to a shader program
		void SetTextureUnit(int _unit);

		/*********************************
		********** Vertex Array **********
		*********************************/
		// Create a vertex array and add it to the internal vertexArrayMap
		void CreateVertexArray(const std::string& _vertexArrayKey,
			const std::vector<float>& _vertices,
			const std::vector<unsigned int>& _elements,
			const AttributeLayout& _attributeLayout);

		void BindVertexArray(const std::string& _vertexArrayKey);

		void RenderVertexArray(const std::string& _vertexArrayKey);

		/*********************************
		********* Uniform Buffer *********
		*********************************/
		void CreateUniformBuffer(const std::string& _uniformBufferName, unsigned int _size, std::vector<std::string> _shaderKeys);

		void SetUniformBuffer(const std::string& _uniformBufferName, unsigned int _offset, glm::mat4 _value);

		/**************************
		********* Shader **********
		**************************/
		void CreateShader(const std::string& _shaderKey, const std::string& _vertPath, const std::string& _fragPath);
		void UseShader(const std::string& _shaderKey);
		void SpecifyAttributeLayout(const std::string& _shaderKey, const std::string& _vertexArryObjectKey);

		void SetUniform(const std::string& _shaderKey, const std::string& _uniformKey, const int& _value);
		void SetUniform(const std::string& _shaderKey, const std::string& _uniformKey, const float& _value);
		void SetUniform(const std::string& _shaderKey, const std::string& _uniformKey, const glm::vec3& _value);
		void SetUniform(const std::string& _shaderKey, const std::string& _uniformKey, const glm::vec4& _value);
		void SetUniform(const std::string& _shaderKey, const std::string& _uniformKey, const glm::mat4& _value);

	private:

		std::map <const std::string, GLuint> textureMap;
		std::map <const std::string, ShaderProgramObj> shaderMap;
		std::map <const std::string, VertexArrayObj> vertexArrayMap;
		std::map <const std::string, GLuint> uniformBufferMap;
	};

} // namespace GLW

#endif // _GLWRAP_H_
