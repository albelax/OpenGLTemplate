#pragma once

#include <QMainWindow>

namespace Ui
{
  class MainWindow;
}
class GLWindow;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget * parent = 0);
  ~MainWindow();

private:
  Ui::MainWindow * m_ui;
  GLWindow * m_gl;
  void keyPressEvent(QKeyEvent * _event);
  void mouseMoveEvent(QMouseEvent * _event);
  void mousePressEvent(QMouseEvent *_event);
  void mouseReleaseEvent(QMouseEvent *_event);
};

