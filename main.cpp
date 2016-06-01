#include <iostream>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <omp.h>
#include <stdio.h>

#include "Space.h"
#include "globals.h"

using namespace std;

void initGraphics(int argc, char** argv);
void renderScene();
void changeSize(int w, int h);
void idle();

Space x;

int main(int argc, char** argv)
{
    x.generateParticles(N_PARTS, 400);
    x.leapfrog_init(TIME_STEP);

    // Get the number of processors in this system
    int iCPU = omp_get_num_procs();

    // Now set the number of threads
    omp_set_num_threads(iCPU);

    initGraphics(argc, argv);
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(idle);
    glutMainLoop();
}

void initGraphics(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(X_RES, Y_RES);
	glutCreateWindow("N-body");
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(POINT_SIZE);
}

void changeSize(int w, int h) {

	if (h == 0)
		h = 1;

	float ratio =  w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);

    glOrtho(0.0f - X_RES/2, X_RES/2, Y_RES/2, 0.0f - Y_RES/2, 0.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    x.render();
    glutSwapBuffers();
}

void idle() {

    if (MODE == "BH") {

        BHTree* t = new BHTree(0, 0, 1000);
        x.BH_step(TIME_STEP, t);
        delete t;
    }
    else if (MODE == "BF") {

        x.step(TIME_STEP);
    }
    else {

        cout << "MODE incorrectly set" << endl;
        exit(-1);
    }
    glutPostRedisplay();
}
