#pragma once
#include <vector>

class Shader;

class Buffer
{
public:
	enum BufferType { VERTEX, NORMAL, UV };
  Buffer();
	Buffer(const int _size, int _sizeOfData);
  int append(const void * _address, int size, BufferType _type);
  //void clear(const int _size, int _sizeOfData);
  void reset(const int _size);
  void Load(const Shader & shader, const std::vector<float> & vertices, const std::vector<float> & normals);
  bool LoadVertices(const std::vector<float> & vertices);
  bool LoadNormals(const std::vector<float> & normals);

protected:
  void init();
  unsigned int m_vao;
  unsigned int m_vertexBuffer;
  unsigned int m_normalBuffer;
  unsigned int m_uvBuffer;
	int m_vertex_index;
	int m_normal_index;
	int m_uv_index;
	int m_sizeOfData;
	std::vector<int> m_elements;
};

