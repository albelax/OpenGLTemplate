#include "Scene.h"
#include <QEvent>

///
/// \brief Scene::Scene
///
Scene::Scene()
{
  Initialize();
}

///
/// \brief Initialize
///
void Scene::Initialize()
{
  // create camera and set its initial state
  pCamera = std::make_unique<TrackballCamera>();
  pCamera->setInitialMousePos(0,0);
  pCamera->setTarget(0.0f, 0.0f, 0.0f);
  pCamera->setEye(0.0f, 0.0f, 10.0f);
  rotating = false;

  // load a default mesh
  pMesh = std::make_unique<Mesh>( "models/Suzanne.obj", "Suzanne" );

  // create a buffer we'll use to draw triangles
  pBuffer = std::make_unique<Buffer>();
  pMesh->setBufferIndex( 0 );

  // load and compile the shaders
  std::string shadersAddress = "shaders/";
  pShader = std::make_unique<Shader>( "shader", shadersAddress + "phong_vert.glsl", shadersAddress + "simplefrag.glsl" );

  glLinkProgram( pShader->getShaderProgram() );
  glUseProgram( pShader->getShaderProgram() );

  // link matrices with shader locations
  MVAddress = glGetUniformLocation( pShader->getShaderProgram(), "MV" );
  MVPAddress = glGetUniformLocation( pShader->getShaderProgram(), "MVP" );
  NAddress = glGetUniformLocation( pShader->getShaderProgram(), "N" );
}

///
/// \brief Render
///
void Scene::Render(int width, int height)
{
  glClearColor( 1, 1, 1, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  pCamera->update();
  projection = glm::perspective( glm::radians( 60.0f ),
                                   static_cast<float>( width ) / static_cast<float>( height ), 0.1f, 100.0f );
  view = glm::lookAt( glm::vec3( 0.0f, 0.0f, 5.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );

  if ( rotating )
    MV = glm::rotate( MV, glm::radians( -1.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
  MVP = projection * pCamera->viewMatrix() * MV;

  glm::mat3 N = glm::mat3( glm::inverse( glm::transpose( MV ) ) );

  glUniformMatrix4fv( MVPAddress, 1, GL_FALSE, glm::value_ptr( MVP ) );
  glUniformMatrix4fv( MVAddress, 1, GL_FALSE, glm::value_ptr( MV ) );

  glUniformMatrix3fv( NAddress, 1, GL_FALSE, glm::value_ptr( N ) );

  pBuffer->Load(*pShader, pMesh->getVertexData(), pMesh->getNormalsData());
  glDrawArrays( GL_TRIANGLES, 0, ( pMesh->getAmountVertexData() / 3 ) );

  float gizmoSize = 2.0f;
  std::vector<float> gizmo = {0,0,0, 0,gizmoSize,0, 0,0,0, gizmoSize,0,0, 0,0,0, 0,0,gizmoSize};
  pBuffer->Load(*pShader, gizmo, std::vector<float>());
  glDrawArrays( GL_LINES, 0 , gizmo.size()/3);
}

///
/// \brief HandleCameraMove
/// \param x
/// \param y
///
void Scene::HandleCameraMove(double x, double y)
{
  pCamera->handleMouseMove(x, y);
}

///
/// \brief HandleCameraClick
/// \param action
///
void Scene::HandleCameraClick(QMouseEvent * action)
{
  pCamera->handleMouseClick(action);
}
