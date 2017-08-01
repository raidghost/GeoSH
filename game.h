#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#define MATRIX_DIM 2
#define VECTOR_DIM (MATRIX_DIM * MATRIX_DIM)

#include "Structs.h"

void tensor(const double a[MATRIX_DIM][MATRIX_DIM], const double b[MATRIX_DIM][MATRIX_DIM], double *c[VECTOR_DIM]);
double norm2(const Point *point);
double norm2_3D(const Point *point);
void normalize(Point *point);
void normalize_3D(Point *point);
Point measure(const Point psi, const double m[MATRIX_DIM][MATRIX_DIM], const double n[MATRIX_DIM][MATRIX_DIM]);
Point matrixVectorProduct(const double *m[VECTOR_DIM], const Point *vector);

#endif
