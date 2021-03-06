#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <math.h>

#include "graphics.h"

Point proj_stereo(const Point *m)
{
    Point mp;
    long double lambda;
    if(m->h == 1)
        lambda = 0;
    else
        lambda = (long double) 1/(1-m->h);

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

	double x = 0, y = 0, z = 0, alpha = 0;

	alpha = sqrt(psiOld->x * psiOld->x + psiOld->y * psiOld->y + psiOld->z * psiOld->z);
    glBegin(GL_LINES);
	if(alpha != 0)
	{
		x = psiOld->x / alpha;
		y = psiOld->y / alpha;
		z = psiOld->z / alpha;
    	glColor3ub(127.5 * (1-psiOld->h),0,127.5 * (1+psiOld->h));
	    glVertex3d(x, y, z);
    	glVertex3d(0,0,0);
	}
	else
	{//psiOld->h = 1 so we display something special.
		glColor3ub(255, 0, 0);
		glVertex3d(0,0,1);
	}
    glEnd();

	alpha = sqrt(psiNew->x * psiNew->x + psiNew->y * psiNew->y + psiNew->z * psiNew->z);
    glBegin(GL_LINES);
	if(alpha != 0)
	{
		x = psiNew->x / alpha;
		y = psiNew->y / alpha;
		z = psiNew->z / alpha;
    	glColor3ub(127.5 * (1-psiNew->h),0,127.5 * (1+psiNew->h));
	    glVertex3d(x, y, z);
    	glVertex3d(0,0,0);
	}
	else
	{//psiNew->h = 1 so we display something special.
		glColor3ub(255, 0, 0);
		glVertex3d(0,0,1);
	}
    glEnd();
	
	glFlush();
	SDL_GL_SwapBuffers();
}
