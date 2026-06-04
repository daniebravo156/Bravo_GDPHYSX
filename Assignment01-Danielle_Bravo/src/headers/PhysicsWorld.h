#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H

#include <list>
#include "Particle.h"
#include "ForceRegistry.h"

// World Manager 
class PhysicsWorld {
public:
    std::list<Particle*> particles;
    ForceRegistry forceRegistry;

    // track a new object in the world
    void AddParticle(Particle* particle) {
        particles.push_back(particle);
    }

    // standard world loop update (centralized physics update)
    void Update(float time) {
        // Clear accumulators from previous frame
        for (Particle* p : particles) {
            p->resetForce();
        }

        // Accumulate new forces mapped in the registry
        forceRegistry.updateForces(time);

        // Update kinematics via integration
        for (Particle* p : particles) {
            p->Update(time);
        }
    }
};

#endif