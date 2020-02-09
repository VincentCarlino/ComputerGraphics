#pragma once

#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>


class BasicWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT

private:
  QString vertexShaderString() const;
  QString fragmentShaderString() const;
  void createShader();
  QOpenGLVertexArrayObject vao_;

protected:
  // Required interaction overrides
  void keyReleaseEvent(QKeyEvent* keyEvent) override;

  // Required overrides form QOpenGLWidget
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  QOpenGLBuffer vbo_;
  QOpenGLBuffer ibo_;
  QOpenGLBuffer cbo_;
  QOpenGLShaderProgram shaderProgram_;


public:
  BasicWidget(QWidget* parent=nullptr);
  virtual ~BasicWidget();
  
};
