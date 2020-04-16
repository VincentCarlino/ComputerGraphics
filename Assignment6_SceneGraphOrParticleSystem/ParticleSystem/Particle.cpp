#include "Particle.h"

#include <QtGui>
#include <QtOpenGL>

Particle::Particle(Renderable* model, const QVector3D& velocity, float lifespan)
{
    modelToRender_ = model;
    velocity_ = velocity;
    secondsLeftToLive_ = lifespan;
}

Particle::~Particle()
{}

void Particle::updateAndDraw(unsigned int msSinceLastFrame, const QMatrix4x4& world, const Camera& camera)
{
    // move the model a certain amount 
    QMatrix4x4 transform = modelToRender_->getModelMatrix();
    transform.translate(velocity_);
    modelToRender_->setModelMatrix(transform);
    modelToRender_->draw(world, camera.getViewMatrix(), camera.getProjectionMatrix());

    secondsLeftToLive_ = secondsLeftToLive_ - msSinceLastFrame * 1000;
}