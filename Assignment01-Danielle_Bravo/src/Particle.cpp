#include "headers/Particle.h"

// constructor
Particle::Particle(float startX, float startY, float startZ, 
                   std::string pName, float red, float green, float blue) 
    : x(startX), y(startY), z(startZ), 
      vx(0.0f), vy(0.0f), vz(0.0f), 
      ax(0.0f), ay(0.0f), az(0.0f),
      fx(0.0f), fy(0.0f), fz(0.0f), mass(1.0f),
      name(pName), r(red), g(green), b(blue) {}

// add force to the accumulator
void Particle::addForce(float fX, float fY, float fZ) {
    fx += fX;
    fy += fY;
    fz += fZ;
}

// clear the accumulator
void Particle::resetForce() {
    fx = 0.0f;
    fy = 0.0f;
    fz = 0.0f;
}

// update velocity
void Particle::UpdateVelocity(float time) {
    // a = F / m
    if (mass > 0.0f) {
        ax = fx / mass;
        ay = fy / mass;
        az = fz / mass;
    }

    vx += ax * time;
    vy += ay * time;
    vz += az * time;
}

// update position
void Particle::UpdatePosition(float time) {
    x += vx * time;
    y += vy * time;
    z += vz * time;
}

// update both
void Particle::Update(float time) {
    UpdateVelocity(time);
    UpdatePosition(time);
}