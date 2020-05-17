#pragma once

#include "Scene.h"
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

protected:
  /// @brief  The following methods must be implimented in the sub class
  /// this is called when the window is created
  void initializeGL();

  /// @brief this is the main gl drawing routine which is called whenever the window needs to be re-drawn
  void paintGL();
  std::unique_ptr<Scene> pScene;
};

