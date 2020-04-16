#pragma once

#include "Particle.h"
#include "Camera.h"

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

class Emitter 
{
protected:
    QVector<Particle> particles_;
    QVector3D position_;
    QVector3D orientation_;
    unsigned int particlesPerSecond_;
    float timeSinceLastEmission_;
    QString modelPath_;
  
public:
    Emitter(const QVector3D& position, const QVector3D& orientation, unsigned int pps, QString modelPath);
    virtual ~Emitter();

    void update(unsigned int msSinceLastFrame, const QMatrix4x4& world, const Camera& camera);

    void emitParticle();
};