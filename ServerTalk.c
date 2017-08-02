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

void update_measurements(serverTalk* s)
{
	s->m0_0[0][0] = cos(s->theta);
	s->m0_0[0][1] = 0;
	s->m0_0[1][0] = 0;
	s->m0_0[1][1] = sin(s->theta);

	s->n0_0[0][0] = 0;
	s->n0_0[0][1] = cos(s->epsilon);
	s->n0_0[1][0] = sin(s->epsilon);
	s->n0_0[1][1] = 0;
}
