#ifndef GLOBALS_H
#define GLOBALS_H

#define MODE "BF" // "BF" for brute force calculation, "BH" for Barnes-Hut approximation
#define COLOR_MODE "MC" // "V" for velocity-based, "MC" for random multi-color

const int N_PARTS = 1200;
const int X_RES = 720;
const int Y_RES = 720;
const double POINT_SIZE = 3.0;
const double SLOW_CONST = 1.0;
const double TIME_STEP = 1.0;
const double SOFTEN = 4.0;  // Softens close particle interactions

// Barnes-Hut constants

const double BH_THETA = 0.5;
const int NW = 0;
const int NE = 1;
const int SW = 2;
const int SE = 3;

#endif // GLOBALS_H_INCLUDED
