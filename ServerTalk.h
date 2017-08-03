#ifndef SERVERTALK_H
#define SERVERTALK_H

#include "Structs.h"

typedef struct serverTalk serverTalk;
struct serverTalk
{
	struct sockaddr_in serverAddress;
	int sockfd;
	int exit;
	long double theta_00;
	long double epsilon_00;
	long double theta_01;
	long double epsilon_01;

	//Measurements : {m0_0,m0_1} and {m1_0,m1_1} are sets of measurements for Alice.
	long double m0_0[MATRIX_DIM][MATRIX_DIM];
	long double n0_0[MATRIX_DIM][MATRIX_DIM];
	long double m0_1[MATRIX_DIM][MATRIX_DIM];
	long double n0_1[MATRIX_DIM][MATRIX_DIM];

	long double m1_0[MATRIX_DIM][MATRIX_DIM];
	long double n1_0[MATRIX_DIM][MATRIX_DIM];
	long double m1_1[MATRIX_DIM][MATRIX_DIM];
	long double n1_1[MATRIX_DIM][MATRIX_DIM];

	//States. psi_ij should be the state once Alice has just seen i and Bob j.
	Point psi_00, psi_01, psi_10, psi_11;
	Point *psi_current;//The current psi to print. This should point toward one of the previous psi.

	//Devil's input (= your input :P)
	GtkWidget *spinButtonX;
	GtkWidget *spinButtonY;
	GtkWidget *randomInputs;//A checkbox to know if the user wants random inputs or not.

	GtkWidget *text2print;
	GtkTextBuffer *bufferGtk;
};

void init_psi(serverTalk* s);
void update_measurements(serverTalk* s);
#endif
