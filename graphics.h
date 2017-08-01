#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#define PI 3.14159
#define SQUARE(x) (x*x)
#define NORM2D2(x) (x[0]*x[0] + x[1]*x[1])

typedef struct Point Point;
struct Point
{
    double x;
    double y;
    double z;
    double h;
};

Point proj_stereo(const Point *m);
void showSphere(const Point *psiOld, const Point *psiNew, double angleX, double angleY, double angleZ);

#endif // GRAPHICS_H_INCLUDED
