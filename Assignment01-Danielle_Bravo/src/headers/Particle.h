#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>

// Physics Particle Class 
class Particle {
public:
    float x, y, z;       // Position
    float vx, vy, vz;    // Velocity
    float ax, ay, az;    // Acceleration
    
    // Force Trackers 
    float fx, fy, fz;    // Force Accumulator
    float mass;          // Particle mass

    // Identity & Rendering vars
    std::string name;
    float r, g, b;       

    // kinematics constructor
    Particle(float startX, float startY, float startZ, 
             std::string pName, float red, float green, float blue);

    // add force 
    void addForce(float fX, float fY, float fZ);

    // clear the accumulator at the start of a frame
    void resetForce();

    // update velocity using accumulated forces
    void UpdateVelocity(float time);

    // update position
    void UpdatePosition(float time);

    // update both
    void Update(float time);
};

#endif