#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "BasicWidget.h"

float camera_x = 0.0f;

//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(QWidget* parent) : QOpenGLWidget(parent), vbo_(QOpenGLBuffer::VertexBuffer), ibo_(QOpenGLBuffer::IndexBuffer)
{
  setFocusPolicy(Qt::StrongFocus);
}

BasicWidget::~BasicWidget()
{
  vbo_.release();
  vbo_.destroy();
  ibo_.release();
  ibo_.destroy();
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

  std::stringstream ss;
  std::string currentVal;
  std::string header;

  std::vector<float> vertices;
  std::vector<float> vertexNormals;
  std::vector<float> idx;

  if(inFile.is_open()){
    while(getline(inFile, line)){

      ss.str(line);
      ss >> header;

      if(header == "v") {
        std::cout << "VERTEX" << std::endl;

        while(ss >> currentVal) {
          std::cout << "Vertex value: " << currentVal << std::endl;
        }
      }
      else if(header == "vn") {
        std::cout << "VERTEX NORMAL" << std::endl;

        while(ss >> currentVal) {
          std::cout << "Vertex normal value: " << currentVal << std::endl;
        }
      }
      else if(header == "f") {
        std::cout << "FACE" << std::endl;
        
        while(ss >> currentVal) {
          std::cout << "Face value: " << currentVal << std::endl;
        }
      }

      ss.clear();

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

  "uniform mat4 modelMatrix;\n"
  "uniform mat4 viewMatrix;\n"
  "uniform mat4 projectionMatrix;\n"

  "out vec4 vertColor;\n"
	"void main()\n"
	"{\n"
  "  mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;\n"
	"  gl_Position = MVP * vec4(position, 1.0);\n"
    "  vertColor = color;\n"
    "}\n";
  return str;
}

QString BasicWidget::fragmentShaderString() const
{
  QString str =
	"#version 330\n"
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
  "  color = vec4(0.0f, 1.0f, 0.6f, 1.0f);\n"
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
  if(keyEvent->key() == Qt::Key_Left) {
    camera_x -= 0.1f;
    qDebug() << camera_x;
  } else if(keyEvent->key() == Qt::Key_Right) {
    camera_x += 0.1f;
    qDebug() << camera_x;
  }
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

  // TODO: parse obj files

  createShader();
  
  // Define our cube verts
  static const GLfloat verts[24] =
  {
    -1.0, -1.0, -1.0,
    -1.0, -1.0, 1.0,
    -1.0, 1.0, -1.0,
    -1.0, 1.0, 1.0,
    1.0, -1.0, -1.0,
    1.0, -1.0, 1.0,
    1.0, 1.0, -1.0,
    1.0, 1.0, 1.0
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
  static const GLuint idx[36] =
  {
      0,4,1,
      1,2,0,
      1,4,5,
      1,7,3,
      2,4,0,
      2,7,6,
      3,2,1,
      3,7,2,
      4,7,5,
      5,7,1,
      6,4,2,
      6,7,4
  };

  shaderProgram_.bind();
  // Create and prepare a vbo
  vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  vbo_.create();
  // Bind our vbo inside our vao
  vbo_.bind();
  vbo_.allocate(verts, 24 * sizeof(GL_FLOAT));
  
  // TODO:  Generate our index buffer
  ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  ibo_.create();
  ibo_.bind();
  ibo_.allocate(idx, 36 * sizeof(GL_UNSIGNED_INT));
  // ENDTODO

  // Create a VAO to keep track of things for us.
  vao_.create();
  vao_.bind();
  vbo_.bind();
  shaderProgram_.enableAttributeArray(0);
  shaderProgram_.setAttributeBuffer(0, GL_FLOAT, 0, 3);
  ibo_.bind();
  // Releae the vao THEN the vbo
  vao_.release();
  shaderProgram_.release();

  glViewport(0, 0, width(), height());
}

void BasicWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
  
  shaderProgram_.bind();
  projection_.perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f); // Set the perspective of our projection matrix (inspired by https://stackoverflow.com/questions/36231719/creating-a-camera-with-opengl-3-3-and-qt-5-6)
  view_.lookAt(QVector3D(3,3,8),QVector3D(0,0,0),QVector3D(0,1,0)); // Set the view to look at (0, 0, 0) from (0, 0, 8) with an up vector of "down"

  shaderProgram_.setUniformValue("viewMatrix", view_);
  shaderProgram_.setUniformValue("projectionMatrix", projection_);
  shaderProgram_.setUniformValue("modelMatrix", model_);

  shaderProgram_.release();
}

void BasicWidget::paintGL()
{
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shaderProgram_.bind();
  vao_.bind();
  // TODO: Change number of indices drawn
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  // ENDTODO
  vao_.release();
  shaderProgram_.release();
  // TODO:  render.
}