#ifndef PARTICLE_H
#define PARTICLE_H

//Physics Particle Class 
class Particle {
public:
    float x, y, z;       // Position
    float vx, vy, vz;    // Velocity
    float ax, ay, az;    // Acceleration

    // constructor
    Particle(float startX, float startY, float startZ);

    // update velocity
    void UpdateVelocity(float time);

    // update position
    void UpdatePosition(float time);

    // update both
    void Update(float time);
};

#endif