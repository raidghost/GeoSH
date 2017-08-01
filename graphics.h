#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include "Structs.h"

Point proj_stereo(const Point *m);
void showSphere(const Point *psiOld, const Point *psiNew, double angleX, double angleY, double angleZ);

#endif // GRAPHICS_H_INCLUDED
