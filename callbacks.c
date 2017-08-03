#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <gtk/gtk.h>

#include "client.h"
#include "callbacks.h"
#include "game.h"

#define PORT 2000
#define HOST "localhost"


void play(GtkWidget* button, gpointer gdata)
{//On sélectionne une valeur arbitraire pour le post measurement state (il y en a deux possibles à chaque fois).
	struct serverTalk *data = (struct serverTalk*)gdata;
	gchar text[256] = "";
	int x = 0, y = 0, m = 0, n = 0;//m and n correspond to what Alice and Bob observe.
	long double probaGagner = 0, proba1 = 0, proba2 = 0;

	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->randomInputs)))
	{
		x = rand() % 2;
		y = rand() % 2;
	}
	else
	{
		x = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(data->spinButtonX));
		y = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(data->spinButtonY));
	}

	if(x == 0 && y == 0)
	{//On gagne ssi Alice et Bob répondent la même chose.
		data->psi_current = &(data->psi_00);
		proba1 = proba(data->psi_current, data->m0_0, data->n0_0);
		proba2 = proba(data->psi_current, data->m0_1, data->n0_1);
		probaGagner = proba1 + proba2;

		sprintf(text, "Alice et Bob gagnent avec proba %LF (x = %d, y = %d)\n", probaGagner, x, y);

		*data->psi_current = measure(data->psi_current, data->m0_0, data->n0_0);
	}
	else if(x == 0 && y == 1)
	{//On gagne ssi Alice et Bob répondent la même chose.
		data->psi_current = &(data->psi_01);
		proba1 = proba(data->psi_current, data->m0_0, data->n1_0);
		proba2 = proba(data->psi_current, data->m0_1, data->n1_1);
		probaGagner = proba1 + proba2;
		sprintf(text, "Alice et Bob gagnent avec proba %LF (x = %d, y = %d)\n", probaGagner, x, y);
		*data->psi_current = measure(data->psi_current, data->m0_0, data->n0_1);
	}
	else if(x == 1 && y == 0)
	{//On gagne ssi Alice et Bob répondent la même chose.
		data->psi_current = &(data->psi_10);
		proba1 = proba(data->psi_current, data->m1_0, data->n0_0);
		proba2 = proba(data->psi_current, data->m1_1, data->n0_1);
		probaGagner = proba1 + proba2;
		sprintf(text, "Alice et Bob gagnent avec proba %LF (x = %d, y = %d)\n", probaGagner, x, y);
		*data->psi_current = measure(data->psi_current, data->m0_1, data->n0_0);
	}
	else//(x == 1 && y == 1)
	{//On gagne ssi Alice et Bob ne répondent pas la même chose.
		data->psi_current = &(data->psi_11);
		proba1 = proba(data->psi_current, data->m1_0, data->n1_1);
		proba2 = proba(data->psi_current, data->m1_1, data->n1_0);
		probaGagner = proba1 + proba2;
		sprintf(text, "Alice et Bob gagnent avec proba %LF (x = %d, y = %d)\n", probaGagner, x, y);
		*data->psi_current = measure(data->psi_current, data->m0_1, data->n1_1);
	}

	gtk_label_set_text(GTK_LABEL(data->text2print), text);
	talk2server(data);
}

int connectionButton(GtkWidget* button, gpointer gdata)
{
	if(strcmp(gtk_button_get_label(GTK_BUTTON(button)), "Connecter") == 0){
		if(connect2server((serverTalk*)gdata) == EXIT_SUCCESS)
		{
			gtk_button_set_label(GTK_BUTTON(button), "Déconnecter");
			gtk_label_set_text(GTK_LABEL(((struct serverTalk*)gdata)->text2print), "Vous pouvez maintenant pipauter les paramètres et appuyer sur \"Play\".");
		}
		else
			g_print("Erreur de connexion");
	}
	else
		gtk_button_set_label(GTK_BUTTON(button), "Connecter");
	
	return 0;
}

void launch(GtkWidget *button, gpointer gdata)
{//Gérer les dépassement de buffer
	struct serverTalk *data = (struct serverTalk*)gdata;
	char text[256] = "";

	if(data->exit)
		sprintf(text, "exit");
	else
		sprintf(text, "newPsi %LF %LF %LF %LF", data->psi_current->x, data->psi_current->y, data->psi_current->z, data->psi_current->h);

	gtk_text_buffer_set_text(data->bufferGtk, text, -1);
	talk2server(data);
}

void scale_adjustment_theta_00(GtkWidget *widget, gpointer gdata)
{
	struct serverTalk* data = (struct serverTalk*)gdata;

	data->theta_00 = gtk_range_get_value(GTK_RANGE(widget));
	data->m0_0[0][0] = cos(data->theta_00);
	data->m0_0[1][1] = sin(data->theta_00);
	data->m0_1[0][1] = cos(data->theta_00);
	data->m0_1[1][0] = sin(data->theta_00);
}

void scale_adjustment_epsilon_00(GtkWidget *widget, gpointer gdata)
{
	struct serverTalk* data = (struct serverTalk*)gdata;

	data->epsilon_00 = gtk_range_get_value(GTK_RANGE(widget));
	data->n0_0[0][0] = cos(data->epsilon_00);
	data->n0_0[1][1] = sin(data->epsilon_00);
	data->n0_1[0][1] = cos(data->epsilon_00);
	data->n0_1[1][0] = sin(data->epsilon_00);
}

void scale_adjustment_theta_01(GtkWidget *widget, gpointer gdata)
{
	struct serverTalk* data = (struct serverTalk*)gdata;

	data->theta_01 = gtk_range_get_value(GTK_RANGE(widget));
	data->m1_0[0][0] = cos(data->theta_01);
	data->m1_0[1][1] = sin(data->theta_01);
	data->m1_1[0][1] = cos(data->theta_01);
	data->m1_1[1][0] = sin(data->theta_01);
}

void scale_adjustment_epsilon_01(GtkWidget *widget, gpointer gdata)
{
	struct serverTalk* data = (struct serverTalk*)gdata;

	data->epsilon_01 = gtk_range_get_value(GTK_RANGE(widget));
	data->n1_0[0][0] = cos(data->epsilon_01);
	data->n1_0[1][1] = sin(data->epsilon_01);
	data->n1_1[0][1] = cos(data->epsilon_01);
	data->n1_1[1][0] = sin(data->epsilon_01);
}

void resetPsi(GtkWidget *widget, gpointer gdata)
{
	init_psi((serverTalk*)gdata);
	gtk_label_set_text(GTK_LABEL(((serverTalk*)gdata)->text2print), "Appuyez sur \"Play\".");
	talk2server((serverTalk*)gdata);
}

void quit(GtkWidget *widget, gpointer gdata)
{
	struct serverTalk* data = (serverTalk*)gdata;
	data->exit = TRUE;
	talk2server(data);
	close(data->sockfd);
	gtk_main_quit();
}
