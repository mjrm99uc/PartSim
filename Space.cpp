#include "Space.h"

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <omp.h>
#include <vector>

using namespace std;

Space::Space() {

    m_numParticles = 0;
}

Space::~Space() {

    m_numParticles = m_parts.size();

    for (int i = 0; i < m_numParticles; i++)
        delete m_parts[i];
}

void Space::genRandomParticle(int maxDist) {

    double magnitude = rand() % (maxDist * 10) / 10.0;
    double angle = rand() / 20.0;

    double x = magnitude * cos(angle);
    double y = magnitude * sin(angle);

    Particle* p = new Particle(x, y);

    double R = (rand() % 500) / 1000.0 + 0.5;
    double G = (rand() % 500) / 1000.0 + 0.5;
    double B = (rand() % 500) / 1000.0 + 0.5;

    //double mass = ((rand() % 100 - 50) / 100.0) + 1;

    //p->setMass(mass);
    p->setColor(R, G, B);

    addParticle(p);
}

void Space::generateParticles(int n, int maxDist) {

    for (int i = 0; i < n; i++)
        genRandomParticle(maxDist);
}

void Space::calculateForces() {

    for (int p1 = 0; p1 < m_numParticles/2; p1++) {

        for (int p2 = m_numParticles-1; p2 >= m_numParticles/2; p2--) {

            if (p1 == p2)
                continue; // Don't calculate a particle's gravitational interaction with itself

            Particle* P1 = m_parts[p1];
            Particle* P2 = m_parts[p2];

            double dx = P2->getX() - P1->getX();
            double dy = P2->getY() - P1->getY();
            double dist = sqrt( (dx * dx) + (dy * dy) );
            double soften = SOFTEN; // To avoid dividing by zero
            double magnitude = (P1->getMass() * P2->getMass()) / ((dist * dist) + soften);

            double x_comp = magnitude * dx / dist; // x and y components of force
            double y_comp = magnitude * dy / dist; // "dx / dist" and "dy / dist" are important - they are the opposite (and adjacent) sides over the hypotenuse, cos and sin!

            P1->addForce(x_comp, y_comp);
            P2->addForce(x_comp * -1, y_comp * -1);
        }
    }
}

void Space::BH_calculateForces(BHTree* t) {

    for (int i = 0; i < m_numParticles; i++) { // Insert all particles

        t->insert(m_parts[i]);
    }

    #pragma omp parallel for

    for (int i = 0; i < m_numParticles; i++) { // Calculate all forces

        t->calcForce(m_parts[i]);
    }
}

void Space::resetAllForces() {

    for (int i = 0; i < m_numParticles; i++)
        m_parts[i]->resetForces();
}

void Space::updateAll(double timestep) {

    for (int i = 0; i < m_numParticles; i++)
        m_parts[i]->update(timestep);
}

void Space::step(double timestep) {

    resetAllForces();
    calculateForces();
    updateAll(timestep);
    if (COLOR_MODE == "V")
        colorize_by_velocity();
}

void Space::BH_step(double timestep, BHTree* t) {

    resetAllForces();
    BH_calculateForces(t);
    updateAll(timestep);
    if (COLOR_MODE == "V")
        colorize_by_velocity();
}

void Space::leapfrog_init(double timestep) {

    calculateForces();
    leapfrog_updateall(timestep);
}

void Space::leapfrog_updateall(double timestep) {

    for (int i = 0; i < m_numParticles; i++)
        m_parts[i]->leapfrog_update(timestep);
}

void Space::colorize_by_velocity() {

    vector<double> vels(m_numParticles);

    double maxvel = 0;

    for (int i = 0; i < m_numParticles; i++) {

        vels[i] = m_parts[i]->getVel2();

        if (vels[i] > maxvel)
            maxvel = vels[i];
    }
    for (int i = 0; i < m_numParticles; i++) {

        m_parts[i]->setColorVel(vels[i] / maxvel);
    }
}

void Space::render() {

    glBegin(GL_POINTS);

    for (int i = 0; i < m_numParticles; i++) {

        glColor3d(
                  m_parts[i]->getR(),
                  m_parts[i]->getG(),
                  m_parts[i]->getB());

        glVertex3d(
                   m_parts[i]->getX(),
                   m_parts[i]->getY(),
                   -1 );
    }
    glEnd();
}
