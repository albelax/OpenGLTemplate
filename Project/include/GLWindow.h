#pragma once

#include "Shader.h"
#include "TrackballCamera.h"
#include "Mesh.h"
#include "Buffer.h"

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <ext.hpp>
#include <glm.hpp>
#include <QOpenGLWidget>
#include <QResizeEvent>
#include <QEvent>
#include <memory>

class GLWindow : public QOpenGLWidget
{
Q_OBJECT
public :
  /// @brief Constructor for GLWindow
  /// @param [in] _parent the parent window to create the GL context in
  GLWindow( QWidget *_parent );

  /// @brief dtor
  ~GLWindow();
  void mouseMove( QMouseEvent * _event );
  void mouseClick( QMouseEvent * _event );

public slots:
  void rotating( const bool _rotating ) { m_rotating = _rotating; }

protected:
  /// @brief  The following methods must be implimented in the sub class
  /// this is called when the window is created
  void initializeGL();

  /// @brief this is the main gl drawing routine which is called whenever the window needs to be re-drawn
  void paintGL();
  void renderScene();

private:
  std::unique_ptr<Mesh> pMesh;
  std::unique_ptr<Buffer> pBuffer;
  Shader m_shader;

  GLint m_MVAddress;
  GLint m_MVPAddress;
  GLint m_NAddress;
  glm::mat4 m_projection;
  glm::mat4 m_view;
  glm::mat4 m_MV;
  glm::mat4 m_MVP;
  TrackballCamera m_camera;
  bool m_rotating;
  int m_amountVertexData;
};

