#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <math.h>

#include "game.h"

void tensor(const double a[MATRIX_DIM][MATRIX_DIM], const double b[MATRIX_DIM][MATRIX_DIM], double *c[VECTOR_DIM])
//Computes the tensor product of two matrices of dimension MATRIX_DIM.
{
	for(int i = 0 ; i < VECTOR_DIM ; i++)
	{
		for(int j = 0 ; j < VECTOR_DIM ; j++)
			c[i][j] = a[i / 2][j / 2] * b[i % 2][j % 2];
	}
}

Point matrixVectorProduct(const double *m[VECTOR_DIM], const Point *vector)
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
		{
			lines[i] += m[i][j] * vect2List[j];
			//printf("m[%d][%d] = %lf et v[%d] = %lf\n", i,j,m[i][j], j, vect2List[j]);
		}
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
	//a0 is the first column of m.
	double a0[2], a1[2], b0[2], b1[2];
	a0[0] = m[0][0];
	a0[1] = m[1][0];
	a1[0] = m[0][1];
	a1[1] = m[1][1];

	b0[0] = n[0][0];
	b0[1] = n[1][0];
	b1[0] = n[0][1];
	b1[1] = n[1][1];

	return 0.5 * (NORM2D2(a0) * NORM2D2(b0) + NORM2D2(a1) * NORM2D2(b1) + 2 * (a0[0] * a1[0] + a0[1] * a1[1]) * (b0[0] * b1[0] + b0[1] * b1[1]));
}

Point measure(const Point psi, const double m[MATRIX_DIM][MATRIX_DIM], const double n[MATRIX_DIM][MATRIX_DIM])
{
	Point postState;
	double **c = NULL;

	c = (double**)malloc(VECTOR_DIM * sizeof(double*));
	if(c == NULL)
		exit(EXIT_FAILURE);
	
	for(int i = 0 ; i < VECTOR_DIM ; i++)
	{
		c[i] = (double*)malloc(VECTOR_DIM * sizeof(double));
		if(c[i] == NULL)
			exit(EXIT_FAILURE);
	}
	
	tensor(m,n,c);
	postState = matrixVectorProduct((const double**)c, &psi);

	for(int i = 0 ; i < VECTOR_DIM ; i++)
		free(c[i]);
	
	free(c);

	normalize_3D(&postState);
//	fprintf(stderr, "%lf\n", proba(psi, m, n));
	return postState;
}
