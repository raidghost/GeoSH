#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#define MATRIX_DIM 2
#define VECTOR_DIM (MATRIX_DIM * MATRIX_DIM)

#include "Structs.h"

void tensor(const long double a[MATRIX_DIM][MATRIX_DIM], const long double b[MATRIX_DIM][MATRIX_DIM], long double c[VECTOR_DIM][VECTOR_DIM]);
long double norm2(const Point *point);
long double norm2_3D(const Point *point);
void normalize(Point *point);
void normalize_3D(Point *point);
long double proba(const Point *psi, const long double m[MATRIX_DIM][MATRIX_DIM], const long double n[MATRIX_DIM][MATRIX_DIM]);
Point measure(const Point *psi, const long double m[MATRIX_DIM][MATRIX_DIM], const long double n[MATRIX_DIM][MATRIX_DIM]);
Point matrixVectorProduct(const long double m[VECTOR_DIM][VECTOR_DIM], const Point *vector);

#endif
