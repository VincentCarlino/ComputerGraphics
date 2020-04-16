#include "Emitter.h"

#include <QtGui>
#include <QtOpenGL>

Emitter::Emitter(const QVector3D& position, const QVector3D& orientation, unsigned int pps, QString modelPath)
{
    position_ = position;
    orientation_ = orientation;
    particlesPerSecond_ = pps;
    modelPath_ = modelPath;
    timeSinceLastEmission_ = 0;

}

Emitter::~Emitter()
{}

void Emitter::update(unsigned int msSinceLastFrame, const QMatrix4x4& world, const Camera& camera) 
{
    float sec = msSinceLastFrame / 1000.0f;
    timeSinceLastEmission_ = timeSinceLastEmission_ + sec;
    for(Particle p: particles_)
    {
        p.updateAndDraw(msSinceLastFrame, world, camera);
        if(p.isDead()) {
            particles_.clear();
        }
    }

    if (timeSinceLastEmission_ >= 1) {
        for(int ii = 0; ii < particlesPerSecond_; ++ii) 
        {
            emitParticle();
        }
        timeSinceLastEmission_ = 0;
    }
    

}

void Emitter::emitParticle()
{
    Renderable* object = new Renderable();
    object->loadObject(modelPath_);
    float r0 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

    // Make a random direction and pass it to a new particle.  Add that particle to particles_
    particles_.push_back(Particle(object, QVector3D(r0, r1, r2), 5));
}
