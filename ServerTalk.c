#include <math.h>
#include <netdb.h> 
#include <gtk/gtk.h>
#include "ServerTalk.h"

void init_psi(serverTalk* s)
{
	s->psi_00.x = 1/sqrt(2);
	s->psi_00.y = 0;
	s->psi_00.z = 0;
	s->psi_00.h = 1/sqrt(2);

	s->psi_01.x = 1/sqrt(2);
	s->psi_01.y = 0;
	s->psi_01.z = 0;
	s->psi_01.h = 1/sqrt(2);

	s->psi_10.x = 1/sqrt(2);
	s->psi_10.y = 0;
	s->psi_10.z = 0;
	s->psi_10.h = 1/sqrt(2);

	s->psi_11.x = 1/sqrt(2);
	s->psi_11.y = 0;
	s->psi_11.z = 0;
	s->psi_11.h = 1/sqrt(2);

	s->psi_current = &(s->psi_00);
	g_print("On reset psi.\n");
}

void update_measurements(serverTalk* data)
{
	data->m0_0[0][0] = cos(data->theta_00);
	data->m0_0[0][1] = 0;
	data->m0_0[1][0] = 0;
	data->m0_0[1][1] = sin(data->theta_00);

	data->m0_1[0][0] = 0;
	data->m0_1[0][1] = cos(data->theta_00);
	data->m0_1[1][0] = sin(data->theta_00);
	data->m0_1[1][1] = 0;
	
	data->n0_0[0][0] = cos(data->epsilon_00);
	data->n0_0[0][1] = 0;
	data->n0_0[1][0] = 0;
	data->n0_0[1][1] = sin(data->epsilon_00);

	data->n0_1[0][0] = 0;
	data->n0_1[0][1] = cos(data->epsilon_00);
	data->n0_1[1][0] = sin(data->epsilon_00);
	data->n0_1[1][1] = 0;
	
	data->m1_0[0][0] = cos(data->theta_01);
	data->m1_0[0][1] = 0;
	data->m1_0[1][0] = 0;
	data->m1_0[1][1] = sin(data->theta_01);

	data->m1_1[0][0] = 0;
	data->m1_1[0][1] = cos(data->theta_01);
	data->m1_1[1][0] = sin(data->theta_01);
	data->m1_1[1][1] = 0;

	data->n1_0[0][0] = cos(data->epsilon_01);
	data->n1_0[0][1] = 0;
	data->n1_0[1][0] = 0;
	data->n1_0[1][1] = sin(data->epsilon_01);

	data->n1_1[0][0] = 0;
	data->n1_1[0][1] = cos(data->epsilon_01);
	data->n1_1[1][0] = sin(data->epsilon_01);
	data->n1_1[1][1] = 0;
}
