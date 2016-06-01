#ifndef BHTREE_H
#define BHTREE_H

#include "Particle.h"
#include "globals.h"

class BHTree {

public:

    BHTree(double mx, double my, double len);
    BHTree(int quad, BHTree* parent);
    ~BHTree();

    void insert (Particle* p);  // Insert
    bool isIn   (Particle* p);  // Return whether the particle is in this quadrant
    int  whichQuad(Particle* p, BHTree* t);
    bool isInternal();          // Return whether the node is internal
    bool isExternal();
    bool isEmpty();             // Return whether the node contains a body or not
    void updateCG(Particle* p); // Update the CG of this node
    void calcForce(Particle* p);// Calculate the force exerted on particle p and update it

    double getCGx() { return CG_x; }
    double getCGy() { return CG_y; }
    double getCGMass() { return CG_mass; }

    double getMidX() { return mid_x; }
    double getMidY() { return mid_y; }
    double getLength() { return length; }

    void print();

private:

    double mid_x;   // Center of quadrant represented by this node
    double mid_y;   // ...
    double length;  // Length of quadrant's sides

    double CG_x;    // Location of center of gravity
    double CG_y;    // ...
    double CG_mass; // Combined mass of all contained bodies

    void addToCG(Particle* p);
    double calcDistanceTo(Particle* p);

    bool quadIsEmpty(int q);

    BHTree* quads[4]; // NW, NE, SW, and SE quadrants

    Particle* m_body;
};

#endif // BHTREE_H
