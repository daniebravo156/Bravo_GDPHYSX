#ifndef FORCE_GENERATOR_H
#define FORCE_GENERATOR_H

#include "Particle.h"
#include <cmath>

// Abstract Base Class
class ForceGenerator {
public:
    virtual void updateForce(Particle* particle, float time) = 0;
};

// Gravity Generator 
class GravityForceGenerator : public ForceGenerator {
private:
    float gx, gy, gz; // 3D gravity vector
public:
    GravityForceGenerator(float x, float y, float z) : gx(x), gy(y), gz(z) {}

    void updateForce(Particle* particle, float time) override {
        // Only apply if mass is valid (avoid divide by zero)
        if (particle->mass <= 0.0f) return;
        
        // F = m * a
        particle->addForce(particle->mass * gx, particle->mass * gy, particle->mass * gz);
    }
};

// Drag Force Generator 
class DragForceGenerator : public ForceGenerator {
private:
    float k1; // Velocity drag coefficient
    float k2; // Velocity squared drag coefficient

public:
    DragForceGenerator(float k1, float k2) : k1(k1), k2(k2) {}

    void updateForce(Particle* particle, float time) override {

        // Calc the magnitude of the velocity
        float velMag = std::sqrt(particle->vx * particle->vx + 
                                 particle->vy * particle->vy + 
                                 particle->vz * particle->vz);
        
        // If velocity is negligible then dont apply drag to avoid jittering
        if (velMag <= 0.0001f) return;

        // Fd = k1 * |v| + k2 * |v|^2
        float dragMag = k1 * velMag + k2 * velMag * velMag;

        // Direction is opposite to velocity
        float dirX = -particle->vx / velMag;
        float dirY = -particle->vy / velMag;
        float dirZ = -particle->vz / velMag;

        // Apply drag force to accumulator
        particle->addForce(dirX * dragMag, dirY * dragMag, dirZ * dragMag);
    }
};

#endif