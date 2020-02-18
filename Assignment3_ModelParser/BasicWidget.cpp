#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "BasicWidget.h"

bool wireframeMode = false;
bool useObject1 = true;

std::string OBJECT1_PATH = "../objects/bunny.obj";
std::string OBJECT2_PATH = "../objects/monkey.obj";

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
std::vector<float> parseObjVerts(std::string fileName1)
{
  std::ifstream inFile;
  inFile.open(fileName1);
  std::string line;

  std::stringstream ss;
  std::string currentVal;
  std::string header;

  std::vector<float> vertices;

  if(inFile.is_open()){
    while(getline(inFile, line)){

      ss.str(line);
      ss >> header;

      if(header == "v") {
        while(ss >> currentVal) {
          vertices.push_back(stof(currentVal));
        }
      }
      ss.clear();
    }
  }
  inFile.close();

  return vertices;

}

std::vector<int> parseObjIdx(std::string fileName, int offset)
{
  std::ifstream inFile;
  inFile.open(fileName);
  std::string line;
  std::stringstream ss;
  std::string currentVal;
  std::string header;

  std::vector<int> idxs;

  size_t pos = 0;
  std::string token;

  if(inFile.is_open()) {
    while(getline(inFile, line)){

      ss.str(line);
      ss >> header;

      if(header == "f") {
        while(ss >> currentVal) {
          pos = currentVal.find("//");
          token = currentVal.substr(0, pos);
          currentVal.erase(0, pos + 2);

          idxs.push_back(stoi(token) - 1 + offset);
        }
      }
      
      ss.clear();
    }
  }

  inFile.close();
  return idxs;

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
  "  color = vec4(0.0f, 0.5f, 0.7f, 1.0f);\n"
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
  if(keyEvent->key() == Qt::Key_W) {
    wireframeMode = !wireframeMode;
    update();
  }
  else if (keyEvent->key() == Qt::Key_Q) {
    close();
    std::exit(EXIT_SUCCESS);
  }
  else if (keyEvent->key() == Qt::Key_1) {
    useObject1 = true;
    update();
  }
  else if (keyEvent->key() == Qt::Key_2) {
    useObject1 = false;
    update();
  }
}
void BasicWidget::initializeGL()
{
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

  glViewport(0, 0, width(), height());
}

void BasicWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
  
  shaderProgram_.bind();
  projection_.perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f); // Set the perspective of our projection matrix (inspired by https://stackoverflow.com/questions/36231719/creating-a-camera-with-opengl-3-3-and-qt-5-6)
  view_.lookAt(QVector3D(0,2,3),QVector3D(0,1,0),QVector3D(0,1,0));

  shaderProgram_.setUniformValue("viewMatrix", view_);
  shaderProgram_.setUniformValue("projectionMatrix", projection_);
  shaderProgram_.setUniformValue("modelMatrix", model_);

  vao_.create();
  shaderProgram_.release();
}

int BasicWidget::setVerts()
{

  std::vector<float> vertices = parseObjVerts(OBJECT1_PATH);
  std::vector<int> i1 = parseObjIdx(OBJECT1_PATH, 0);
  std::vector<int> i2 = parseObjIdx(OBJECT2_PATH, vertices.size() / 3);
  std::vector<float> obj2verts = parseObjVerts(OBJECT2_PATH);

  vertices.insert( vertices.end(), obj2verts.begin(), obj2verts.end());

  GLfloat verts[vertices.size()];

  GLuint idxOne[i1.size()];
  GLuint idxTwo[i2.size()];


  for (int i = 0; i < vertices.size(); ++i) {
    verts[i] = vertices[i];
  }
  for (int i = 0; i < i1.size(); ++i) {
    idxOne[i] = i1[i];
  }
  for (int i = 0; i < i2.size(); ++i) {
    idxTwo[i] = i2[i];
  }

  shaderProgram_.bind();
  // Create and prepare a vbo
  vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  vbo_.create();
  // Bind our vbo inside our vao
  vbo_.bind();
  vbo_.allocate(verts, vertices.size() * sizeof(GL_FLOAT));
  

  ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  ibo_.create();
  ibo_.bind();
  if(useObject1) {
    ibo_.allocate(idxOne, i1.size() * sizeof(GL_UNSIGNED_INT));
  }
  else {
    ibo_.allocate(idxTwo, i2.size() * sizeof(GL_UNSIGNED_INT));
  }

  // Create a VAO to keep track of things for us.
  vao_.bind();
  vbo_.bind();
  shaderProgram_.enableAttributeArray(0);
  shaderProgram_.setAttributeBuffer(0, GL_FLOAT, 0, 3);
  ibo_.bind();
  // Releae the vao THEN the vbo
  vao_.release();
  shaderProgram_.release();

  shaderProgram_.bind();
  vao_.bind();

  if(useObject1) {
    return i1.size();
  }
  else {
    return i2.size();
  }

}

void BasicWidget::paintGL()
{
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  int idxToDraw = setVerts();

  // If we are set to wireframe mode, render the obj as a wireframe
  if(wireframeMode) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
  glDrawElements(GL_TRIANGLES, idxToDraw, GL_UNSIGNED_INT, 0);
  vao_.release();
  shaderProgram_.release();
}