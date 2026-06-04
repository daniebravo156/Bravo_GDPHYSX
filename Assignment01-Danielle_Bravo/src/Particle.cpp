#include "headers/Particle.h"

// constructor
Particle::Particle(float startX, float startY, float startZ) 
    : x(startX), y(startY), z(startZ), vx(0), vy(0), vz(0), ax(0), ay(0), az(0) {}

// update velocity
void Particle::UpdateVelocity(float time) {
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