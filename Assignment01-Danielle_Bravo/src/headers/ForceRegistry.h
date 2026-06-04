#ifndef FORCE_REGISTRY_H
#define FORCE_REGISTRY_H

#include <vector>
#include "Particle.h"
#include "ForceGenerator.h"

// Force Registry Class
class ForceRegistry {
protected:
    struct ParticleForceRegistration {
        Particle* particle;
        ForceGenerator* generator;
    };
    std::vector<ParticleForceRegistration> registry;

public:
    // map a force to a particle
    void add(Particle* particle, ForceGenerator* generator) {
        ParticleForceRegistration registration;
        registration.particle = particle;
        registration.generator = generator;
        registry.push_back(registration);
    }

    // unmap a force
    void remove(Particle* particle, ForceGenerator* generator) {
        for (auto it = registry.begin(); it != registry.end(); ++it) {
            if (it->particle == particle && it->generator == generator) {
                registry.erase(it);
                break;
            }
        }
    }

    // wipe registry
    void clear() {
        registry.clear();
    }

    // execute mapped forces
    void updateForces(float time) {
        for (auto& reg : registry) {
            reg.generator->updateForce(reg.particle, time);
        }
    }
};

#endif