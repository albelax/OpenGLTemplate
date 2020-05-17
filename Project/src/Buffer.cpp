#include "Buffer.h"
#include "Shader.h"

#include <iostream>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#define LINUX
#endif

Buffer::Buffer()
{
  init();
}

Buffer::Buffer( const int _size, int _sizeOfData )
{
  init();

	// allocate the buffers
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, _size * m_sizeOfData, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,	m_normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, _size * m_sizeOfData, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,	m_uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, _size * m_sizeOfData, 0, GL_STATIC_DRAW);
	m_vertex_index = 0;
	m_normal_index = 0;
	m_uv_index = 0;
}

void Buffer::init()
{
  glGenVertexArrays(1,&m_vao);
  glBindVertexArray(m_vao);
  glGenBuffers(1, &m_vertexBuffer);
  glGenBuffers(1, &m_normalBuffer);
  glGenBuffers(1, &m_uvBuffer);
}

int Buffer::append(const void * _address, int size, BufferType _type)
{
	// keeps track of the index of the elements, it will point to a specific index depending on the case
	int * currentIndex;
	int tmp_index = 0;
	switch(_type)
	{
		case(VERTEX):
    {
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
			m_elements.push_back(m_vertex_index);
			currentIndex = &m_vertex_index;
			break;
    }
		case(NORMAL):
    {
      glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
      currentIndex = &m_normal_index;
      break;
    }
		case(UV):
    {
      glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
      currentIndex = &m_uv_index;
      break;
    }
    default: break;
	}

	tmp_index = *currentIndex;
	*currentIndex += size;
	glBufferSubData(GL_ARRAY_BUFFER, tmp_index * m_sizeOfData, size * m_sizeOfData, _address);
	return tmp_index; // return the index of the current element
}

void Buffer::reset( const int _size )
{
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _size * m_sizeOfData, 0, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,	m_normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, _size * m_sizeOfData, 0, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,	m_uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, _size * m_sizeOfData, 0, GL_STATIC_DRAW);
  m_elements.clear();
  m_vertex_index = 0;
  m_normal_index = 0;
}

//--------------------------------------//
//--------------------------------------//
//--------------------------------------//

void Buffer::Load(const Shader & shader, const std::vector<float> & vertices, const std::vector<float> & normals)
{
  if (LoadVertices(vertices))
  {
    // pass vertices to shader
    GLint pos = glGetAttribLocation( shader.getShaderProgram(), "VertexPosition" );
    glEnableVertexAttribArray( pos );
    glVertexAttribPointer( pos, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  }

  if (LoadNormals(normals))
  {
    // pass normals to shader
    GLint n = glGetAttribLocation( shader.getShaderProgram(), "VertexNormal" );
    glEnableVertexAttribArray( n );
    glVertexAttribPointer( n, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  }
}

bool Buffer::LoadVertices(const std::vector<float> & vertices)
{
  const unsigned int vertexSize = vertices.size() * sizeof(float);

  if (vertexSize)
  {
    glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffer );
    glBufferData( GL_ARRAY_BUFFER, vertexSize, 0, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, vertexSize, &vertices[0] );
    return true;
  }
  return false;
}

bool Buffer::LoadNormals(const std::vector<float> & normals)
{
  const unsigned int normalSize = normals.size() * sizeof(float);

  if (normalSize)
  {
    glBindBuffer( GL_ARRAY_BUFFER,	m_normalBuffer );
    glBufferData( GL_ARRAY_BUFFER, normalSize, 0, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, normalSize, &normals[0] );
    return true;
  }
  return false;
}
