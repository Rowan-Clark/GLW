#include "GLW/GlWrap.h"

namespace GLW
{

	GlWrap::GlWrap()
	{

	}

	GlWrap::~GlWrap()
	{
		// Delete every texture in the texture map
		for (auto const& texture : textureMap)
		{
			glDeleteTextures(1, &texture.second);
		}
	}

	void GlWrap::SetClearColor(float _red, float _green, float _blue, float _alpha)
	{
		GL_CHECK(glClearColor(_red, _green, _blue, _alpha));
	}

	void GlWrap::ClearFramebuffer()
	{
		GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void GlWrap::LoadTexture(const std::string& _textureKey, const std::string& _imagePath)
	{
		auto result = textureMap.insert(std::make_pair(_textureKey, 0));
		if (!result.second)
		{
			std::cerr << "Key already in use: " << result.first->first << std::endl;
			throw std::runtime_error("Failed to load texture");
		}

		std::cout << "Loading image: " << _imagePath << std::endl;

		// Load texture
		GL_CHECK((glGenTextures(1, &textureMap[_textureKey])));
		GL_CHECK((glBindTexture(GL_TEXTURE_2D, textureMap[_textureKey])));

		int width, height;
		unsigned char* image = SOIL_load_image(_imagePath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

		if (!image)
		{
			std::cerr << "Could not load image: " << _imagePath << std::endl;
			std::cerr << "SOIL error: " << SOIL_last_result() << std::endl;
			throw std::runtime_error("Error loading image");
		}

		GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image));

		SOIL_free_image_data(image);

		GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));

		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	}

	void GlWrap::SetActiveTexture(const std::string& _textureKey)
	{
		// Check if textureKey is valid
		if (textureMap.find(_textureKey) == textureMap.end())
		{
			std::cerr << "Texture key '" << _textureKey << "' not found in map" << std::endl;
			throw std::runtime_error("GlWrap Error");
		}

		// Make texture active
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, textureMap[_textureKey]));
	}

	void GlWrap::SetTextureUnit(int _unit)
	{
		switch (_unit)
		{
		case 0: GL_CHECK(glActiveTexture(GL_TEXTURE0)); break;
		case 1: GL_CHECK(glActiveTexture(GL_TEXTURE1)); break;
		case 2: GL_CHECK(glActiveTexture(GL_TEXTURE2)); break;
		case 3: GL_CHECK(glActiveTexture(GL_TEXTURE3)); break;
		default:
			std::cerr << "Texture unit out of range" << std::endl;
			throw std::runtime_error("GlWrap Error");
		}
	}

	void GlWrap::CreateVertexArray(const std::string& _VertexArrayKey,
		const std::vector<float>& _vertices,
		const std::vector<unsigned int>& _elements,
		const AttributeLayout& _attributeLayout)
	{
		auto result = vertexArrayMap.insert(
			std::make_pair(_VertexArrayKey, VertexArray::Make(_vertices, _elements, _attributeLayout)));
		if (!result.second)
		{
			std::cerr << "VertexArray key already in use: " << result.first->first << std::endl;
			throw std::runtime_error("GlWrap Error");
		}
	}

	void GlWrap::BindVertexArray(const std::string& _vertexArrayKey)
	{
		if (vertexArrayMap.find(_vertexArrayKey) == vertexArrayMap.end())
		{
			std::cerr << "Vertex array key " << _vertexArrayKey << " not found in uniform map";
			throw std::runtime_error("GlWrap error");
		}

		vertexArrayMap[_vertexArrayKey]->Bind();
	}

	void GlWrap::RenderVertexArray(const std::string& _vertexArrayKey)
	{
		if (vertexArrayMap.find(_vertexArrayKey) == vertexArrayMap.end())
		{
			std::cerr << "Vertex array key " << _vertexArrayKey << " not found in uniform map";
			throw std::runtime_error("GlWrap error");
		}

		vertexArrayMap[_vertexArrayKey]->Render();
	}

	void GlWrap::CreateUniformBuffer(const std::string& _uniformBufferName, unsigned int size, std::vector<std::string> _shaderKeyVector)
	{
		for (const auto& _shaderKey : _shaderKeyVector)
		{
			shaderMap[_shaderKey]->Use();
			shaderMap[_shaderKey]->BindToUniformBlock(_uniformBufferName, 0);
		}

		auto result = uniformBufferMap.insert(std::make_pair(_uniformBufferName, 0));
		if (!result.second)
		{
			std::cerr << "Uniform buffer key already in use: " << result.first->first << std::endl;
			throw std::runtime_error("GlWrap Error");
		}

		GL_CHECK(glGenBuffers(1, &uniformBufferMap[_uniformBufferName]));

		GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferMap[_uniformBufferName]));
		GL_CHECK(glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW));
		GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, 0));

		GL_CHECK(glBindBufferRange(GL_UNIFORM_BUFFER, 0, uniformBufferMap[_uniformBufferName], 0, size));
	}

	void GlWrap::SetUniformBuffer(const std::string& _uniformBufferName, unsigned int _offset, glm::mat4 _value)
	{
		GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferMap[_uniformBufferName]));
		GL_CHECK(glBufferSubData(GL_UNIFORM_BUFFER, _offset, sizeof(glm::mat4), glm::value_ptr(_value)));
		GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, 0));
	}

	void GlWrap::CreateShader(const std::string& _shaderKey, const std::string& _vertPath, const std::string& _fragPath)
	{
		// Create a shader and insert it into the shader map
		auto result = shaderMap.insert(std::make_pair(_shaderKey, ShaderProgram::Make(_vertPath, _fragPath)));
		if (!result.second)
		{
			std::cerr << "Shader key already in use: " << result.first->first << std::endl;
			throw std::runtime_error("GlWrap Error");
		}
	}

	void GlWrap::UseShader(const std::string& _shaderKey)
	{
		// Check if shaderKey is valid
		if (shaderMap.find(_shaderKey) == shaderMap.end())
		{
			std::cerr << "Shader key '" << _shaderKey << "' not found in map" << std::endl;
		}

		shaderMap[_shaderKey]->Use();
	}

	void GlWrap::SpecifyAttributeLayout(const std::string& _shaderKey, const std::string& _vertexArryObjectKey)
	{
		// Check if shaderKey is valid
		if (shaderMap.find(_shaderKey) == shaderMap.end())
		{
			std::cerr << "Shader key '" << _shaderKey << "' not found in map" << std::endl;
			throw std::runtime_error("GlWrap Error");
		}
		// Check if vertexArrayKey is valid
		else if (vertexArrayMap.find(_vertexArryObjectKey) == vertexArrayMap.end())
		{
			std::cerr << "Vertex Array key '" << _vertexArryObjectKey << "' not found in map" << std::endl;
			throw std::runtime_error("GlWrap Error");
		}

		AttributeLayout attributeLayout = vertexArrayMap[_vertexArryObjectKey]->GetAttributeLayout();
		shaderMap[_shaderKey]->SpecifyAttributeLayout(attributeLayout);
	}

	// Set int uniform
	void GlWrap::SetUniform(const std::string& _shaderKey, const std::string& _uniformKey, const int& _value)
	{
		// Check to see if shader is in shader map
		if (shaderMap.find(_shaderKey) == shaderMap.end())
		{
			std::cerr << "Shader key " << _shaderKey << " not found in uniform map";
			throw std::runtime_error("GlWrap error");
		}

		shaderMap[_shaderKey]->SetUniform(_uniformKey, _value);
	}

	// Set float uniform
	void GlWrap::SetUniform(const std::string& _shaderKey, const std::string& _uniformKey, const float& _value)
	{
		// Check to see if shader is in shader map
		if (shaderMap.find(_shaderKey) == shaderMap.end())
		{
			std::cerr << "Shader key " << _shaderKey << " not found in uniform map";
			throw std::runtime_error("GlWrap error");
		}

		shaderMap[_shaderKey]->SetUniform(_uniformKey, _value);
	}

	// Set vec3 uniform
	void GlWrap::SetUniform(const std::string& _shaderKey, const std::string& _uniformKey, const glm::vec3& _value)
	{
		// Check to see if shader is in shader map
		if (shaderMap.find(_shaderKey) == shaderMap.end())
		{
			std::cerr << "Shader key " << _shaderKey << " not found in uniform map";
			throw std::runtime_error("GlWrap error");
		}

		shaderMap[_shaderKey]->SetUniform(_uniformKey, _value);
	}

	// Set vec4 uniform
	void GlWrap::SetUniform(const std::string& _shaderKey, const std::string& _uniformKey, const glm::vec4& _value)
	{
		// Check to see if shader is in shader map
		if (shaderMap.find(_shaderKey) == shaderMap.end())
		{
			std::cerr << "Shader key " << _shaderKey << " not found in uniform map";
			throw std::runtime_error("GlWrap error");
		}

		shaderMap[_shaderKey]->SetUniform(_uniformKey, _value);
	}

	// Set mat4 uniform
	void GlWrap::SetUniform(const std::string& _shaderKey, const std::string& _uniformKey, const glm::mat4& _value)
	{
		// Check to see if shader is in shader map
		if (shaderMap.find(_shaderKey) == shaderMap.end())
		{
			std::cerr << "Shader key " << _shaderKey << " not found in uniform map";
			throw std::runtime_error("GlWrap error");
		}

		shaderMap[_shaderKey]->SetUniform(_uniformKey, _value);
	}

}