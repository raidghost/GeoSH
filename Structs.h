#ifndef STRUCTS_H
#define STRUCTS_H

#define PORT 2000
#define HOST "localhost"
#define MATRIX_DIM 2
#define PI 3.14159
#define SQUARE(x) (x*x)
#define NORM2D2(x) (x[0]*x[0] + x[1]*x[1])
#define SCALAR(x,y) (x[0] * y[0] + x[1] * y[1])

typedef struct Point Point;
struct Point
{
    long double x;
    long double y;
    long double z;
    long double h;
};

typedef struct PairInt PairInt;
struct PairInt
{
	int x;
	int y;
};

typedef struct ListProba ListProba;
struct ListProba
{
	long double proba;
	struct ListProba *next;
};

ListProba* new_list(unsigned long length);
int list_add(ListProba *l, long double proba);
long double mean(const ListProba *l);
#endif
