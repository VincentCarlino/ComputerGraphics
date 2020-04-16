#include "Renderable.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <QtGui>
#include <QtOpenGL>

Renderable::Renderable() : lightPos_(0.5f, 0.5f, -2.0f), vbo_(QOpenGLBuffer::VertexBuffer), ibo_(QOpenGLBuffer::IndexBuffer), texture_(QOpenGLTexture::Target2D), numTris_(0), vertexSize_(0), rotationAxis_(0.0, 0.0, 1.0), rotationSpeed_(0.25)
{
	rotationAngle_ = 0.0;
}

Renderable::~Renderable()
{
	if (texture_.isCreated()) {
		texture_.destroy();
	}
	if (vbo_.isCreated()) {
		vbo_.destroy();
	}
	if (ibo_.isCreated()) {
		ibo_.destroy();
	}
	if (vao_.isCreated()) {
		vao_.destroy();
	}
}

void Renderable::createShaders()
{
	QString vertexFilename = "./vert.glsl";
	bool ok = shader_.addShaderFromSourceFile(QOpenGLShader::Vertex, vertexFilename);
	if (!ok) {
		qDebug() << shader_.log();
	}
	QString fragmentFilename = "./frag.glsl";
	ok = shader_.addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFilename);
	if (!ok) {
		qDebug() << shader_.log();
	}
	ok = shader_.link();
	if (!ok) {
		qDebug() << shader_.log();
	}
}

QString getTextureFile(const QString mtlFileName, const QString path)
{
	QString texFilePath;

	QDir parentDirectory(path);
	parentDirectory.cdUp();

	QFile f(parentDirectory.filePath(mtlFileName));
	
	if (!f.open(QFile::ReadOnly | QFile::Text)) {
		qDebug() << path << " failed to open file";
		return "";
	}

	QTextStream in(&f);
	QString line = in.readLine();
	QStringList lineComponents;
	QString header;

	
	while (!line.isNull()) {
		lineComponents = line.split(' ');
		header = lineComponents[0];

		if (header == "map_Kd") {
			texFilePath = parentDirectory.filePath(lineComponents[1]);
		}
		line = in.readLine();
	}
	return texFilePath;
	
}

void Renderable::loadObject(const QString path)
{
	QFile f(path);

	QString *currentLine;

	QVector<QVector3D> positions;
	QVector<QVector3D> normals;
	QVector<QVector2D> texCoords;
	QVector<unsigned int> indexes;

    if (!f.open(QFile::ReadOnly | QFile::Text)) {
		qDebug() << path << " failed to open object file";
		return;
	}

    QTextStream in(&f);

	QString line = in.readLine();
	QStringList lineComponents;
	QString header;
	QString folderPath;
	while (!line.isNull()) {
		lineComponents = line.split(' ');
		header = lineComponents[0];

		if(header == "mtllib" && folderPath == "") {
			folderPath = getTextureFile(lineComponents[1], path);
		}
		else if(header == "v") {
			if(lineComponents.length() < 4) {
				return;
			}
			positions.push_back(QVector3D(lineComponents[1].toFloat(), lineComponents[2].toFloat(), lineComponents[3].toFloat()));
		}
		else if (header == "vt") {
			texCoords.push_back(QVector2D(1 - lineComponents[1].toFloat(), 1 - lineComponents[2].toFloat()));
		}
		else if (header == "vn") {
			normals.push_back(QVector3D(lineComponents[1].toFloat(), lineComponents[2].toFloat(), lineComponents[3].toFloat()));
		}
		else if (header == "f") {
			QStringList f0 = lineComponents[1].split("/");
			QStringList f1 = lineComponents[2].split("/");
			QStringList f2 = lineComponents[3].split("/");

			indexes.push_back(f0[0].toInt() - 1);
			indexes.push_back(f0[1].toInt() - 1);
			indexes.push_back(f0[2].toInt() - 1);
			indexes.push_back(f1[0].toInt() - 1);
			indexes.push_back(f1[1].toInt() - 1);
			indexes.push_back(f1[2].toInt() - 1);
			indexes.push_back(f2[0].toInt() - 1);
			indexes.push_back(f2[1].toInt() - 1);
			indexes.push_back(f2[2].toInt() - 1);
		}
    	line = in.readLine();
	};


	init(positions, normals, texCoords, indexes, folderPath);

	
}

void Renderable::init(const QVector<QVector3D>& positions, const QVector<QVector3D>& normals, const QVector<QVector2D>& texCoords, const QVector<unsigned int>& indexes, const QString& textureFile)
{
	// // NOTE:  We do not currently do anything with normals -- we just
	// // have it here for a later implementation!
	// // We need to make sure our sizes all work out ok.
	// if (positions.size() != texCoords.size() ||
	// 	positions.size() != normals.size()) {
	// 	qDebug() << "[Renderable]::init() -- positions size mismatch with normals/texture coordinates";
	// 	return;
	// }

	// Set our model matrix to identity
	modelMatrix_.setToIdentity();
	// Load our texture.
	texture_.setData(QImage(textureFile));

	bool pos = true;
	bool norm = false;
	bool tex = false;

	// set our number of trianges.
	numTris_ = indexes.size() / 3 / 3;

	// num verts (used to size our vbo)
	int numVerts = indexes.size() / 3;
	vertexSize_ = 3 + 3 + 2;  // Position + normal + texCoord
	int numVBOEntries = numVerts * vertexSize_;

	QVector<QVector3D> indexVerts;
	QVector<QVector3D> indexNormals;
	QVector<QVector2D> indexTexCoord;

	// Setup our shader.
	createShaders();

	// Now we can set up our buffers.
	// The VBO is created -- now we must create our VAO
	vao_.create();
	vao_.bind();
	vbo_.create();
	vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo_.bind();

	for(unsigned int i: indexes) {
		if(pos == true) {
			indexVerts.push_back(positions.at(i));
			pos = false;
			norm = false;
			tex = true;
		}
		else if (norm == true) {
			indexNormals.push_back(normals.at(i));
			pos = true;
			norm = false;
			tex = false;
		}
		else if (tex == true) {
			indexTexCoord.push_back(texCoords.at(i));
			pos = false;
			norm = true;
			tex = false;
		}
	}

	float* data = new float[numVBOEntries];
	
	for (int i = 0; i < numVerts; ++i) {
		data[i * vertexSize_ + 0] = indexVerts.at(i).x();
		data[i * vertexSize_ + 1] = indexVerts.at(i).y();
		data[i * vertexSize_ + 2] = indexVerts.at(i).z();
		data[i * vertexSize_ + 3] = indexNormals.at(i).x();
		data[i * vertexSize_ + 4] = indexNormals.at(i).y();
		data[i * vertexSize_ + 5] = indexNormals.at(i).z();
		data[i * vertexSize_ + 6] = indexTexCoord.at(i).x();
		data[i * vertexSize_ + 7] = indexTexCoord.at(i).y();
	}
	vbo_.allocate(data, numVBOEntries * sizeof(float));
	delete[] data;

	// Create our index buffer
	ibo_.create();
	ibo_.bind();
	ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	// create a temporary array for our indexes
	unsigned int* idxAr = new unsigned int[indexes.size()];
	for (int i = 0; i < numVerts; ++i) {
		idxAr[i] = i;
	}
	ibo_.allocate(idxAr, numVerts * sizeof(unsigned int));
	delete[] idxAr;

	// Make sure we setup our shader inputs properly
	shader_.enableAttributeArray(0);
	shader_.setAttributeBuffer(0, GL_FLOAT, 0, 3, vertexSize_ * sizeof(float));
	shader_.enableAttributeArray(1);
	shader_.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, vertexSize_ * sizeof(float));
	shader_.enableAttributeArray(2);
	shader_.setAttributeBuffer(2, GL_FLOAT, (3+3) * sizeof(float), 2, vertexSize_ * sizeof(float));

	// Release our vao and THEN release our buffers.
	vao_.release();
	vbo_.release();
	ibo_.release();
}

void Renderable::update(const qint64 msSinceLastFrame)
{
	float sec = msSinceLastFrame / 1000.0f;
	float anglePart = sec * rotationSpeed_ * 360.f;
	rotationAngle_ += anglePart;
	while(rotationAngle_ >= 360.0) {
		rotationAngle_ -= 360.0;
	}
}

void Renderable::draw(const QMatrix4x4& world, const QMatrix4x4& view, const QMatrix4x4& projection)
{

	// incorporate a real world transform if want it.
	QMatrix4x4 modelMat = modelMatrix_;
	modelMat = world * modelMat;
	// Make sure our state is what we want
	shader_.bind();
	// Set our matrix uniforms!
	QMatrix4x4 id;
	id.setToIdentity();
	shader_.setUniformValue("modelMatrix", modelMat);
	shader_.setUniformValue("viewMatrix", view);
	shader_.setUniformValue("projectionMatrix", projection);

	shader_.setUniformValue("pointLights[0].color", 1.0f, 1.0f, 1.0f);
    shader_.setUniformValue("pointLights[0].position", lightPos_);
    shader_.setUniformValue("pointLights[0].ambientIntensity", 0.5f);
    shader_.setUniformValue("pointLights[0].specularStrength", 0.5f);
    shader_.setUniformValue("pointLights[0].constant", 1.0f);
    shader_.setUniformValue("pointLights[0].linear", 0.09f);
    shader_.setUniformValue("pointLights[0].quadratic", 0.032f);

	vao_.bind();
	texture_.bind();
	glDrawElements(GL_TRIANGLES, 3225, GL_UNSIGNED_INT, 0);
	texture_.release();
	vao_.release();
	shader_.release();
}

QMatrix4x4 Renderable::getModelMatrix()
{
	return modelMatrix_;
}

void Renderable::setModelMatrix(const QMatrix4x4& transform)
{
	modelMatrix_ = transform;
}

void Renderable::setRotationAxis(const QVector3D& axis)
{
	rotationAxis_ = axis;
}

void Renderable::setRotationSpeed(float speed)
{
	rotationSpeed_ = speed;
}