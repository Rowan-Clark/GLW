// File: VertexArray.h
// Author: Rowan Clark
//
// Description:
// This class encapsulates the OpenGL object a Vertex Array Object
// which stores the format of the vertex data and the Vertex Buffer
// Objects which contain the actualy vertex data. This class contains
// one vertex buffer and one element buffer.

#ifndef _VERTEX_ARRAY_H_
#define _VERTEX_ARRAY_H

#include <memory>
#include <vector>

#include <glad/glad.h>

#include "AttributeLayout.h"
#include "CheckOpenGLError.h"

namespace GLW
{

	class VertexArray
	{
	public:

		VertexArray(const std::vector<float>& _vertices,
			const std::vector<unsigned int>& _indices,
			const AttributeLayout& _attributeLayout);
		~VertexArray();

		// 
		using VertexArrayObj = std::unique_ptr<VertexArray>;
		static VertexArrayObj Make(const std::vector<float>& _vertices, const std::vector<unsigned int>& _indices, AttributeLayout _attributeLayout)
		{
			return std::make_unique<VertexArray>(_vertices, _indices, _attributeLayout);
		}

		// A vertex array must be bound before it can be rendered.
		void Bind();

		// Render the vertex array as triangles
		void Render();

		// Used to bind the vertex layout to the attributes in the shader
		AttributeLayout GetAttributeLayout();

	private:
		GLuint vao, vbo, ebo;

		int numIndices;

		AttributeLayout attributeLayout;
	};

	using VertexArrayObj = VertexArray::VertexArrayObj;

} // namespace GLW

#endif // _VERTEX_ARRAY_H_
