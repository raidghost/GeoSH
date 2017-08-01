#ifndef STRUCTS_H
#define STRUCTS_H

#define PORT 2000
#define HOST "localhost"
#define MATRIX_DIM 2
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

typedef struct PairInt PairInt;
struct PairInt
{
	int x;
	int y;
};

#endif
