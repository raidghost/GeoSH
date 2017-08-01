#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <math.h>

#include "graphics.h"

Point proj_stereo(const Point *m)
{
    Point mp;
    double lambda;
    if(m->h == 1)
        lambda = 0;
    else
        lambda = (double) 1/(1-m->h);

    mp.x = lambda * m->x;
    mp.y = lambda * m->y;
    mp.z = lambda * m->z;
    mp.h = 0;
    return mp;
}

void showSphere(const Point *psiOld, const Point *psiNew, double angleX, double angleY, double angleZ)
{
	//Initialisation d'OpenGL
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(1.2,1.2,1.2,0,0,0,0,0,1);
    glRotated(angleX,1,0,0);
	glRotated(angleY, 0,1,0);
    glRotated(angleZ,0,0,1);


	GLUquadric* params = gluNewQuadric();
    glColor3ub(255,255,255);

    gluQuadricDrawStyle(params,GLU_LINE);
    gluQuadricTexture(params,GL_TRUE);
    gluSphere(params,1,10,10);

    gluDeleteQuadric(params);

	params = gluNewQuadric();
    glColor3ub(0,255,0);

    gluQuadricDrawStyle(params,GLU_FILL);
    gluQuadricTexture(params,GL_TRUE);
    gluSphere(params,0.05,10,10);

    gluDeleteQuadric(params);


    glBegin(GL_LINES);
    glColor3ub(127.5 * (1-psiOld->h),0,127.5 * (1+psiOld->h));
    glVertex3d(0,0,0);
    glVertex3d(psiOld->x, psiOld->y, psiOld->z);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(127.5 * (1-psiNew->h),0,127.5 * (1+psiNew->h));
    glVertex3d(0,0,0);
    glVertex3d(psiNew->x, psiNew->y, psiNew->z);
    glEnd();

    glFlush();
    SDL_GL_SwapBuffers();
}
