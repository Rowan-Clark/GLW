#include "GLW/VertexArray.h"

namespace GLW
{

    VertexArray::VertexArray(const std::vector<float>& _vertices,
        const std::vector<unsigned int>& _elements,
        const AttributeLayout& _attributeLayout) :
        vao(0), vbo(0), attributeLayout(_attributeLayout)
    {
        // Create Vertex Array Object
        GL_CHECK(glGenVertexArrays(1, &vao));
        GL_CHECK(glBindVertexArray(vao));

        // Create a Vertex Buffer Object and copy the vertex data to it
        GL_CHECK(glGenBuffers(1, &vbo));

        // Bind the Vertex Buffer to the Vertex Array
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));

        // Move the vertex data into the vertex buffer (i.e. onto the graphics card)
        GL_CHECK((glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), &_vertices[0], GL_STATIC_DRAW)));

        // Generate an element buffer object (essitially a list 
        glGenBuffers(1, &ebo);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _elements.size() * sizeof(GLuint), &_elements[0], GL_STATIC_DRAW);

        numIndices = _elements.size();
    }

    VertexArray::~VertexArray()
    {
        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    void VertexArray::Bind()
    {
        GL_CHECK(glBindVertexArray(vao));
    }

    void VertexArray::Render()
    {
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    }

    AttributeLayout VertexArray::GetAttributeLayout()
    {
        return attributeLayout;
    }

}