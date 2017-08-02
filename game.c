#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <math.h>

#include "game.h"

void tensor(const double a[MATRIX_DIM][MATRIX_DIM], const double b[MATRIX_DIM][MATRIX_DIM], double c[VECTOR_DIM][VECTOR_DIM])
//Computes the tensor product of two matrices of dimension MATRIX_DIM.
{
	for(int i = 0 ; i < VECTOR_DIM ; i++)
	{
		for(int j = 0 ; j < VECTOR_DIM ; j++)
			c[i][j] = a[i / 2][j / 2] * b[i % 2][j % 2];
	}
}

Point matrixVectorProduct(const double m[VECTOR_DIM][VECTOR_DIM], const Point *vector)
{
	unsigned int i = 0, j = 0;
	double lines[VECTOR_DIM] = {0}, vect2List[VECTOR_DIM] = {0};
	Point result;

	vect2List[0] = vector->x;
	vect2List[1] = vector->y;
	vect2List[2] = vector->z;
	vect2List[3] = vector->h;

	for(i = 0 ; i < VECTOR_DIM ; i++)
	{
		for(j = 0 ; j < VECTOR_DIM ; j++)
			lines[i] += m[i][j] * vect2List[j];
	}
	
	result.x = lines[0];
	result.y = lines[1];
	result.z = lines[2];
	result.h = lines[3];

	return result;
}

double norm2(const Point *point)
{
	return sqrt(point->x * point->x + point->y * point->y + point->z * point->z + point->h * point->h);
}

double norm2_3D(const Point *point)
{
	return sqrt(point->x * point->x + point->y * point->y + point->z * point->z);
}

void normalize(Point *point)
{
	//We call "alpha" the normalization coef.
	double alpha = norm2(point);
	point->x /= alpha;
	point->y /= alpha;
	point->z /= alpha;
	point->h /= alpha;
}

void normalize_3D(Point *point)
{//We only normalize the 3D part of a 4D vector.
	//We call "alpha" the normalization coef.
	double alpha = norm2_3D(point);
	point->x /= alpha;
	point->y /= alpha;
	point->z /= alpha;
}

double proba(const Point *psi, const double m[MATRIX_DIM][MATRIX_DIM], const double n[MATRIX_DIM][MATRIX_DIM])
//Returns the probability of Alice answering "a" and Bob answering "b" given the setting (psi,m,n) (where m -> a, n -> b)
{
	//a0 is the first column of m, a1 the second.
	double a0[2], a1[2], b0[2], b1[2];
	a0[0] = m[0][0];
	a0[1] = m[1][0];
	a1[0] = m[0][1];
	a1[1] = m[1][1];

	b0[0] = n[0][0];
	b0[1] = n[1][0];
	b1[0] = n[0][1];
	b1[1] = n[1][1];

	return psi-> x * psi->x * NORM2D2(a0) * NORM2D2(b0) + psi->y * psi->y * NORM2D2(a0) * NORM2D2(b1) + psi->z * psi->z * NORM2D2(a1) * NORM2D2(b0) + psi->h * psi->h * NORM2D2(a1) * NORM2D2(b1) + 2 * psi->x * psi->y * NORM2D2(a0) * SCALAR(b0,b1) + 2 * psi->x * psi->z * NORM2D2(b0) * SCALAR(a0,a1) + 2 * psi->x * psi->h * SCALAR(a0,a1) * SCALAR(b0,b1) + 2 * psi->y * psi->z * SCALAR(a0,a1) * SCALAR(b0,b1) + 2 * psi->y * psi->h * SCALAR(a0,a1) * NORM2D2(b1) + 2 * psi->z * psi->h * NORM2D2(a1) * SCALAR(b0,b1);
}

Point measure(const Point *psi, const double m[MATRIX_DIM][MATRIX_DIM], const double n[MATRIX_DIM][MATRIX_DIM])
{//This function returns the post measurement state after measuring with m and n.
	Point postState;
	double *c = NULL;

	c = (double*)malloc(VECTOR_DIM * VECTOR_DIM * sizeof(double));
	if(c == NULL)
		exit(EXIT_FAILURE);

	tensor(m,n,(double (*)[VECTOR_DIM])c);
	postState = matrixVectorProduct((double (*)[VECTOR_DIM])c, psi);

	free(c);
	normalize(&postState);
	return postState;
}
