#pragma once

#include <string>
#include <fstream>

class Shader
{
public:
  Shader(std::string _name, std::string _vertexPath, std::string _fragmentPath);
  Shader();
  std::string getName() const { return this -> m_name; }
  unsigned int getShaderProgram() const { return m_shaderProgram; }

protected:
	std::string m_name;
  unsigned int m_shaderProgram;
	void setName(std::string _name) { this -> m_name = _name; }
	std::string loadShader(std::string _filename);
};

