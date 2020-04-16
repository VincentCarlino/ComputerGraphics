#pragma once

#include "Renderable.h"
#include "Camera.h"

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

class Particle {
protected:
  Renderable* modelToRender_;
  QMatrix4x4 particleTransform_;
  QVector3D velocity_;
  float secondsLeftToLive_;

public:
  Particle(Renderable* model, const QVector3D& velocity, float lifespan);
  virtual ~Particle();
  void updateAndDraw(unsigned int msSinceLastFrame, const QMatrix4x4& world, const Camera& camera);
  bool isDead() const {return secondsLeftToLive_ > 0.0;}
};