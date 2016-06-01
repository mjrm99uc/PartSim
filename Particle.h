#ifndef PARTICLE_H
#define PARTICLE_H

#include "globals.h"

class Space;
class BHTree;

class Particle {

    public:

        Particle();
        Particle(double x, double y);
        Particle(double x, double y, Space* s);
        Particle(double x, double y, double mass);

        void setSpace(Space* s) { m_space = s; }

        double getX() { return m_x; }
        double getY() { return m_y; }
        double getMass() { return m_mass; }
        double getR() { return m_R; }
        double getG() { return m_G; }
        double getB() { return m_B; }
        double getVel2() { return (m_x_veloc * m_x_veloc) + (m_y_veloc * m_y_veloc); }

        void setX(double x) { m_x = x; }
        void setY(double y) { m_y = y; }
        void setMass(double m) { m_mass = m; }

        void setR(double R) { m_R = R; }
        void setG(double G) { m_G = G; }
        void setB(double B) { m_B = B; }
        void setColor(double R, double G, double B) { m_R = R; m_G = G; m_B = B; }
        void setColorVel(double c) { m_R = c; m_G = c; m_B = c; }

        void addForce(double x, double y) { m_dx += x; m_dy += y; }
        void calcForce(Particle* b);
        void calcForce(BHTree* node);
        void resetForces() { m_dx = 0; m_dy = 0; }
        void update(double timestep);
        void leapfrog_update(double timestep);
        void print();

        double distanceTo(Particle* b);
        void addBodies(Particle* a, Particle* b);

    private:

        double m_x;     // Position
        double m_y;
        double m_dx;    // These are the accumulated force components which will be used at the end of each
        double m_dy;    // frame to update the location of each particle
        double m_x_veloc;
        double m_y_veloc;
        double m_mass;
        double m_R; // Color
        double m_G;
        double m_B;

        Space* m_space;
};

#endif // PARTICLE_H
