#ifndef SPACE_H
#define SPACE_H

#include <vector>

#include "Particle.h"
#include "BHTree.h"

class Quad;

class Space {

public:

    Space();
    virtual ~Space();

    unsigned int GetnumParticles() { return m_numParticles; }

    void addParticle(Particle* p) { p->setSpace(this); m_parts.push_back(p); m_numParticles++; }
    void genRandomParticle(int maxDist);
    void generateParticles(int n, int maxDist);
    void step(double timestep);
    void BH_step(double timestep, BHTree* t);
    void leapfrog_init(double timestep);
    void render();

private:

    unsigned int m_numParticles;
    std::vector<Particle*> m_parts;

    void resetAllForces();
    void calculateForces();
    void BH_calculateForces(BHTree* t);
    void updateAll(double timestep);
    void leapfrog_updateall(double timestep);
    void colorize_by_velocity();
};

#endif // SPACE_H
