#include "GLWindow.h"

#include <iostream>
#include <QColorDialog>
#include <QGLWidget>
#include <QImage>
#include <QScreen>

//----------------------------------------------------------------------------------------------------------------------

GLWindow::GLWindow( QWidget *_parent ) : QOpenGLWidget( _parent )
{
  // set this widget to have the initial keyboard focus
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  this->resize( _parent->size() );
  m_camera.setInitialMousePos(0,0);
  m_camera.setTarget(0.0f, 0.0f, -2.0f);
  m_camera.setEye(0.0f, 0.0f, 0.0f);
  m_rotating = false;
}

//----------------------------------------------------------------------------------------------------------------------

void GLWindow::initializeGL()
{
#ifdef linux
  // this needs to be after the context creation, otherwise it GLEW will crash
  glewExperimental = GL_TRUE;
  glewInit();
#endif
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_MULTISAMPLE );
  glEnable( GL_TEXTURE_2D );

  pMesh = std::make_unique<Mesh>( "models/Suzanne.obj", "Suzanne" );
  pBuffer = std::make_unique<Buffer>();

  // load and compile the shaders
  std::string shadersAddress = "shaders/";
  m_shader = Shader( "m_shader", shadersAddress + "phong_vert.glsl", shadersAddress + "simplefrag.glsl" );

  glLinkProgram( m_shader.getShaderProgram() );
  glUseProgram( m_shader.getShaderProgram() );

  pMesh->setBufferIndex( 0 );
  m_amountVertexData = pMesh->getAmountVertexData();

  // link matrices with shader locations
  m_MVAddress = glGetUniformLocation( m_shader.getShaderProgram(), "MV" );
  m_MVPAddress = glGetUniformLocation( m_shader.getShaderProgram(), "MVP" );
  m_NAddress = glGetUniformLocation( m_shader.getShaderProgram(), "N" );
  m_MV = glm::translate( m_MV, glm::vec3(0.0f, 0.0f, -2.0f) );
}

//----------------------------------------------------------------------------------------------------------------------

GLWindow::~GLWindow()
{

}

//----------------------------------------------------------------------------------------------------------------------

void GLWindow::mouseMove(QMouseEvent * _event)
{
  m_camera.handleMouseMove( _event->pos().x(), _event->pos().y() );
  update();
}

//----------------------------------------------------------------------------------------------------------------------

void GLWindow::mouseClick(QMouseEvent * _event)
{
  m_camera.handleMouseClick(_event);
  update();
}

//------------------------------------------------------------------------------------------------------------------------------

void GLWindow::paintGL()
{
  renderScene();
  update();
}

//------------------------------------------------------------------------------------------------------------------------------

void GLWindow::renderScene()
{
  glViewport( 0, 0, width()*devicePixelRatio(), height()*devicePixelRatio() );
  glClearColor( 1, 1, 1, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  m_camera.update();
  m_projection = glm::perspective( glm::radians( 60.0f ),
                                   static_cast<float>( width() ) / static_cast<float>( height() ), 0.1f, 100.0f );
  m_view = glm::lookAt( glm::vec3( 0.0f, 0.0f, 5.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );

  if ( m_rotating )
    m_MV = glm::rotate( m_MV, glm::radians( -1.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
  m_MVP = m_projection * m_camera.viewMatrix() * m_MV;

  glm::mat3 N = glm::mat3( glm::inverse( glm::transpose( m_MV ) ) );

  glUniformMatrix4fv( m_MVPAddress, 1, GL_FALSE, glm::value_ptr( m_MVP ) );
  glUniformMatrix4fv( m_MVAddress, 1, GL_FALSE, glm::value_ptr( m_MV ) );

  glUniformMatrix3fv( m_NAddress, 1, GL_FALSE, glm::value_ptr( N ) );

  pBuffer->Load(m_shader, pMesh->getVertexData(), pMesh->getNormalsData());
  glDrawArrays( GL_TRIANGLES, 0, ( m_amountVertexData / 3 ) );

  float gizmoSize = 2.0f;
  std::vector<float> gizmo = {0,0,0, 0,gizmoSize,0, 0,0,0, gizmoSize,0,0, 0,0,0, 0,0,gizmoSize};
  pBuffer->Load(m_shader, gizmo, std::vector<float>());
  glDrawArrays( GL_LINES, 0 , gizmo.size()/3);
}
