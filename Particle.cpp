#include "Particle.h"
#include "BHTree.h"

#include <cstdlib>
#include <iostream>
#include <cmath>

Particle::Particle() {

    m_x = rand() % 100 - 50;
    m_y = rand() % 100 - 50;
    m_mass = 1;
    m_space = nullptr;
    m_dx = 0;
    m_dy = 0;
    m_x_veloc = 0;
    m_y_veloc = 0;
    m_R = 1;
    m_G = 1;
    m_B = 1;
}

Particle::Particle(double x, double y) {

    m_x = x;
    m_y = y;
    m_mass = 1;
    m_space = nullptr;
    m_dx = 0;
    m_dy = 0;
    m_x_veloc = 0;
    m_y_veloc = 0;
    m_R = 1;
    m_G = 1;
    m_B = 1;
}

Particle::Particle(double x, double y, Space* s) {

    m_x = x;
    m_y = y;
    m_mass = 1;
    m_space = s;
    m_dx = 0;
    m_dy = 0;
    m_x_veloc = 0;
    m_y_veloc = 0;
    m_R = 1;
    m_G = 1;
    m_B = 1;
}

Particle::Particle(double x, double y, double mass) {

    m_x = x;
    m_y = y;
    m_mass = mass;
    m_space = nullptr;
    m_dx = 0;
    m_dy = 0;
    m_x_veloc = 0;
    m_y_veloc = 0;
    m_R = 1;
    m_G = 1;
    m_B = 1;
}

void Particle::update(double timestep) {

    m_x_veloc += timestep * m_dx / (m_mass * SLOW_CONST);
    m_y_veloc += timestep * m_dy / (m_mass * SLOW_CONST);
    m_x += timestep * m_x_veloc;
    m_y += timestep * m_y_veloc;
}

void Particle::leapfrog_update(double timestep) {

    m_x_veloc += (timestep / 2) * m_dx / (m_mass * SLOW_CONST);
    m_y_veloc += (timestep / 2) * m_dy / (m_mass * SLOW_CONST);
    m_x += timestep * m_x_veloc;
    m_y += timestep * m_y_veloc;
}

void Particle::print() {

    std::cout << "Currently at (" << m_x << ", " << m_y << ")" << std::endl;
    std::cout << "Velocity: (" << m_x_veloc << ", " << m_y_veloc << ")" << std::endl;
}

double Particle::distanceTo(Particle* b) {

    double dx = b->getX() - this->getX();
    double dy = b->getY() - this->getY();
    double dist = sqrt( (dx * dx) + (dy * dy) );

    return dist;
}

void Particle::calcForce(Particle* b) {

    Particle* a = this;

    double dx = b->getX() - a->getX();
    double dy = b->getY() - a->getY();
    double dist = sqrt( (dx*dx) + (dy*dy) );
    double soften = SOFTEN;
    double force = (a->getMass() * b->getMass()) / ((dist*dist) + soften);

    double fx = force * dx / dist;
    double fy = force * dy / dist;

    a->addForce(fx, fy);
}

void Particle::calcForce(BHTree* node) {

    Particle* a = this;

    double dx = node->getCGx() - a->getX();
    double dy = node->getCGy() - a->getY();
    double dist = sqrt( (dx*dx) + (dy*dy) );
    double soften = SOFTEN;
    double force = (a->getMass() * node->getCGMass()) / ((dist*dist) + soften);

    double fx = force * dx / dist;
    double fy = force * dy / dist;

    a->addForce(fx, fy);

    if (fx > 1500 || fy > 1500) { // Large force

        std::cout << "Large force detected. Force: (" << fx << ", " << fy << ")" << std::endl;
        this->print();
        std::cout << "Node: ";
        node->print();
        std::cout << std::endl;
    }
}

void Particle::addBodies(Particle* a, Particle* b) {

    double combined_mass = a->getMass() + b->getMass();
    double x = (a->getX() * a->getMass() + b->getX() * b->getMass()) / combined_mass;
    double y = (a->getY() * a->getMass() + b->getY() * b->getMass()) / combined_mass;

    a->setX(x);
    a->setY(y);
    a->setMass(combined_mass);
}
