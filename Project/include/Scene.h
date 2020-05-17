#pragma once

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <ext.hpp>
#include <glm.hpp>
#include <memory>

#include "Shader.h"
#include "TrackballCamera.h"
#include "Mesh.h"
#include "Buffer.h"

class QMouseEvent;

class Scene
{
public:
  Scene();
  void Initialize();
  void Render(int width, int height);
  void HandleCameraMove(double x, double y);
  void HandleCameraClick(QMouseEvent * action);

protected:
  std::unique_ptr<Mesh> pMesh;
  std::unique_ptr<Buffer> pBuffer;
  std::unique_ptr<TrackballCamera> pCamera;
  std::unique_ptr<Shader> pShader;

  GLint MVAddress;
  GLint MVPAddress;
  GLint NAddress;
  glm::mat4 projection;
  glm::mat4 view;
  glm::mat4 MV;
  glm::mat4 MVP;
  bool rotating;
};
