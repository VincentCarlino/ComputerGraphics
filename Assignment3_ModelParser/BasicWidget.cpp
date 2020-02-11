#include <iostream>
#include <fstream>
#include <string>

#include "BasicWidget.h"

//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(QWidget* parent) : QOpenGLWidget(parent), vbo_(QOpenGLBuffer::VertexBuffer), cbo_(QOpenGLBuffer::VertexBuffer), ibo_(QOpenGLBuffer::IndexBuffer)
{
  setFocusPolicy(Qt::StrongFocus);
}

BasicWidget::~BasicWidget()
{
  vbo_.release();
  vbo_.destroy();
  ibo_.release();
  ibo_.destroy();
  cbo_.release();
  cbo_.destroy();
  vao_.release();
  vao_.destroy();
}

//////////////////////////////////////////////////////////////////////
// Privates
void parseObj(std::string fileName)
{
  std::cout << "Parsing file: " << fileName << std::endl;

  // QVector3D(float xpos, float ypos, float zpos)
  
  std::ifstream inFile;
  inFile.open(fileName);
  std::string line;

  if(inFile.is_open()){
    while(getline(inFile, line)){
      std::cout << line << std::endl;
    }
  }

  std::cout << "EOF" << std::endl;

}


QString BasicWidget::vertexShaderString() const
{
  QString str =
	"#version 330\n"
	"layout(location = 0) in vec3 position;\n"
    "layout(location = 1) in vec4 color;\n"
    "out vec4 vertColor;\n"
	"void main()\n"
	"{\n"
	"  gl_Position = vec4(position, 1.0);\n"
    "  vertColor = color;\n"
    "}\n";
  return str;
}

QString BasicWidget::fragmentShaderString() const
{
  QString str =
	"#version 330\n"
    "in vec4 vertColor;\n"
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
	"  color = vertColor;\n"
	"}\n";
  return str;
}

void BasicWidget::createShader()
{
  QOpenGLShader vert(QOpenGLShader::Vertex);
  vert.compileSourceCode(vertexShaderString());
  QOpenGLShader frag(QOpenGLShader::Fragment);
  frag.compileSourceCode(fragmentShaderString());
  bool ok = shaderProgram_.addShader(&vert);
  if(!ok) {
	qDebug() << shaderProgram_.log();
  }
  ok = shaderProgram_.addShader(&frag);
  if(!ok) {
	qDebug() << shaderProgram_.log();
  }
  ok = shaderProgram_.link();
  if(!ok) {
	qDebug() << shaderProgram_.log();
  }
}

///////////////////////////////////////////////////////////////////////
// Protected
void BasicWidget::keyReleaseEvent(QKeyEvent* keyEvent)
{
  // TODO
  // Handle key events here.
  qDebug() << "You Pressed an unsupported Key!";
  // ENDTODO
}
void BasicWidget::initializeGL()
{
  parseObj("../objects/cube.obj");
  makeCurrent();
  initializeOpenGLFunctions();

  QOpenGLContext* curContext = this->context();
  qDebug() << "[BasicWidget]::initializeGL() -- Context Information:";
  qDebug() << "  Context Valid: " << std::string(curContext->isValid() ? "true" : "false").c_str();
  qDebug() << "  GL Version Used: " << curContext->format().majorVersion() << "." << curContext->format().minorVersion();
  qDebug() << "  Vendor: " << reinterpret_cast<const char*>(glGetString(GL_VENDOR));
  qDebug() << "  Renderer: " << reinterpret_cast<const char*>(glGetString(GL_RENDERER));
  qDebug() << "  Version: " << reinterpret_cast<const char*>(glGetString(GL_VERSION));
  qDebug() << "  GLSL Version: " << reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

  createShader();
  
  // TODO:  Add vertex and index data to draw two triangles
  // Define our verts
  static const GLfloat verts[12] =
  {
	-0.8f, -0.8f, 0.0f, // Left vertex position
	0.8f, -0.8f, 0.0f,  // right vertex position
	-0.8f,  0.8f, 0.0f,  // Top vertex position
    0.8f, 0.8f, 0.0f
  };
  // Define our vert colors
  static const GLfloat colors[16] =
  {
      0.8f, 0.8f, 0.8f, 1.0f,
      0.8f, 0.8f, 0.8f, 1.0f,
      0.8f, 0.8f, 0.8f, 1.0f,
      0.8f, 0.8f, 0.8f, 1.0f,
  };
  // Define our indices
  static const GLuint idx[6] =
  {
      0, 1, 2, 2, 1, 3
  };

  shaderProgram_.bind();
  // Create and prepare a vbo
  vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  vbo_.create();
  // Bind our vbo inside our vao
  vbo_.bind();
  vbo_.allocate(verts, 12 * sizeof(GL_FLOAT));

  // TODO:  Generate our color buffer
  cbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  cbo_.create();
  cbo_.bind();
  cbo_.allocate(colors, 16 * sizeof(GL_FLOAT));
  // ENDTODO
  
  // TODO:  Generate our index buffer
  ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  ibo_.create();
  ibo_.bind();
  ibo_.allocate(idx, 6 * sizeof(GL_UNSIGNED_INT));
  // ENDTODO

  // Create a VAO to keep track of things for us.
  vao_.create();
  vao_.bind();
  vbo_.bind();
  shaderProgram_.enableAttributeArray(0);
  shaderProgram_.setAttributeBuffer(0, GL_FLOAT, 0, 3);
  cbo_.bind();
  shaderProgram_.enableAttributeArray(1);
  shaderProgram_.setAttributeBuffer(1, GL_FLOAT, 0, 4);
  ibo_.bind();
  // Releae the vao THEN the vbo
  vao_.release();
  shaderProgram_.release();

  glViewport(0, 0, width(), height());
}

void BasicWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
}

void BasicWidget::paintGL()
{
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glClearColor(0.0f, 20.0f, 20.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shaderProgram_.bind();
  vao_.bind();
  // TODO: Change number of indices drawn
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
  // ENDTODO
  vao_.release();
  shaderProgram_.release();
  // TODO:  render.
}