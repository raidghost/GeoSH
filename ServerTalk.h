#ifndef SERVERTALK_H
#define SERVERTALK_H

#include "Structs.h"

typedef struct serverTalk serverTalk;
struct serverTalk
{
	int sockfd;
	int exit;
	double theta;
	double epsilon;
	double m[MATRIX_DIM][MATRIX_DIM];
	double n[MATRIX_DIM][MATRIX_DIM];
	Point psi;
	GtkTextBuffer *bufferGtk;
	struct sockaddr_in serverAddress;
};

#endif
