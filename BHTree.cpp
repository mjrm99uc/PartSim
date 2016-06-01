#include "BHTree.h"

#include <iostream>
#include <stdio.h>
#include <cmath>

using namespace std;

BHTree::~BHTree() {

    for (int i = 0; i < 4; i++)
        delete quads[i];
}

BHTree::BHTree(double mx, double my, double len) {

    mid_x = mx;
    mid_y = my;
    length = len;

    CG_x = 0;
    CG_y = 0;
    CG_mass = 0;

    for (int i = 0; i < 4; i++)
        quads[i] = nullptr;

    m_body = nullptr;
}

BHTree::BHTree(int quad, BHTree* parent) {

    length = parent->getLength() / 2.0;
    CG_x = 0;
    CG_y = 0;
    CG_mass = 0;

    for (int i = 0; i < 4; i++)
        quads[i] = nullptr;

    m_body = nullptr;

    double p_midx = parent->getMidX();
    double p_midy = parent->getMidY();

    if (quad == 0) { // NW

        mid_y = p_midy + length / 2;
        mid_x = p_midx - length / 2;
    }
    else if (quad == 1) { // NE

        mid_y = p_midy + length / 2;
        mid_x = p_midx + length / 2;
    }
    else if (quad == 2) { // SW

        mid_y = p_midy - length / 2;
        mid_x = p_midx - length / 2;
    }
    else if (quad == 3) { // SE

        mid_y = p_midy - length / 2;
        mid_x = p_midx + length / 2;
    }
    else { // Something went horribly wrong

        std::cout << "Tried to allocate a node with quadrant " << quad << std::endl;
        exit(-1);
    }
}

void BHTree::insert(Particle* p) {

    if (!isIn(p) || p->getX() != p->getX()) // if the particle is out of range of the tree (or its location is an infinity, hence != itself), don't add it
        return;

    if (isEmpty()) { // If node is empty
        m_body = p;

    CG_x = p->getX(); // Initialize the node's CG to the particle's properties
    CG_y = p->getY();
    CG_mass = p->getMass();

    }
    else { // Node contains a body already

        if (isInternal()) { // Contains subnodes, so insert into a deeper subnode

            addToCG(p);

            int quad = whichQuad(p, this);

            if (quadIsEmpty(quad))
                quads[quad] = new BHTree(quad, this);

            quads[quad]->insert(p);
        }
        else { // Node contains one body and is external

            Particle* other = m_body;                   // First, insert the existing particle into a deeper subnode
                                                        // .
            int quad = whichQuad(other, this);          // .
                                                        // .
            if (quadIsEmpty(quad))                      // .
                quads[quad] = new BHTree(quad, this);   // .
                                                        // .
            quads[quad]->insert(other);                 // <

            quad = whichQuad(p, this);                  // Then, insert the new particle into a deeper subnode
                                                        // .
            if (quadIsEmpty(quad))                      // .
                quads[quad] = new BHTree(quad, this);   // .
                                                        // .
            quads[quad]->insert(p);                     // <
                                                        //
            addToCG(p);                                 //
        }
    }
}

void BHTree::addToCG(Particle* p) {

    double initmass = CG_mass;
    double pmass = p->getMass();
    double x1 = CG_x;
    double y1 = CG_y;
    double x2 = p->getX();
    double y2 = p->getY();

    CG_mass = initmass + pmass;
    CG_x = (x1 * initmass + x2 * pmass) / CG_mass;
    CG_y = (y1 * initmass + y2 * pmass) / CG_mass;
}

void BHTree::calcForce(Particle* p) {

    if (isExternal() && !isEmpty() && m_body != p) { // External node, non-empty, contained body is not the particle we're checking it against
        p->calcForce(this); // Different, particle definition of calcForce, not recursion

    } else { // Internal node

        double s = this->length;
        double d = calcDistanceTo(p);

        if (s/d < BH_THETA) { // If node is far enough away to approximate force of all children using the CG

            p->calcForce(this);
        }
        else { // Else, node is close enough to require more precision

            for (int q = 0; q < 4; q++) {

                if (quads[q] != nullptr)
                    quads[q]->calcForce(p); // Recursive call on all children
            }
        }
    }
}

double BHTree::calcDistanceTo(Particle* p) {

    double dx = p->getX() - CG_x;
    double dy = p->getY() - CG_y;

    return sqrt( (dx*dx) + (dy*dy) );
}

bool BHTree::isIn(Particle* p) {

    double x = p->getX();
    double y = p->getY();

    double leftBound = getMidX() - (getLength() / 2);
    double rightBound = leftBound + getLength();
    double lowerBound = getMidY() - (getLength() / 2);
    double upperBound = lowerBound + getLength();

    if (x < leftBound || x >= rightBound || y < lowerBound || y >= upperBound)
        return false;

    return true;
}

int BHTree::whichQuad(Particle* p, BHTree* t) {

    double x = p->getX();
    double y = p->getY();

    double midX = t->getMidX();
    double midY = t->getMidY();

    if (y < midY) { // Particle is south

        if (x < midX) // Particle is west
            return SW;

        else // Particle is east
            return SE;
    }
    else { // Particle is north

        if (x < midX) // Particle is west
            return NW;

        else // Particle is east
            return NE;
    }
}

bool BHTree::isExternal() {

    if (quads[NW] == nullptr && quads[NE] == nullptr && quads[SW] == nullptr && quads[SE] == nullptr) // Node has no children
        return true;

    return false;
}

bool BHTree::isInternal() {

    return !isExternal();
}

bool BHTree::isEmpty() {

    return m_body == nullptr;
}

bool BHTree::quadIsEmpty(int q) {

    return (quads[q]) == nullptr;
}

void BHTree::print() {

    cout << "CG: (" << getCGx() << ", " << getCGy() << ") Mass: " << getCGMass() << endl;
}
