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
{
	struct serverTalk *data = (struct serverTalk*)gdata;
	gchar text[256] = "";
	int x = 0, y = 0;

/*	x = rand() % 2;
	y = rand() % 2;*/

	if(x == 0 && y == 0)
	{
		data->psi_current = &(data->psi_00);
		sprintf(text, "Alice et Bob gagnent avec proba %lf (x = %d, y = %d)\n", proba(data->psi_current, data->m0_0, data->n0_0), x, y);
		*data->psi_current = measure(data->psi_current, data->m0_0, data->n0_0);
	}
	
	gtk_label_set_text(GTK_LABEL(data->text2print), text);
	talk2server(data);
}

int connectionButton(GtkWidget* button, gpointer gdata)
{
	if(strcmp(gtk_button_get_label(GTK_BUTTON(button)), "Connecter") == 0){
		if(connect2server((serverTalk*)gdata) == EXIT_SUCCESS)
			gtk_button_set_label(GTK_BUTTON(button), "Déconnecter");
		else
			g_print("Erreur de connexion");
	}
	else
		gtk_button_set_label(GTK_BUTTON(button), "Connecter");
	
	return 0;
}

void launch(GtkWidget *button, gpointer gdata)
{
	talk2server((serverTalk*)gdata);
}

void scale_adjustment_theta(GtkWidget *widget, gpointer gdata)
{
	struct serverTalk* data = (struct serverTalk*)gdata;
	char command[256] = "";

	data->theta = gtk_range_get_value(GTK_RANGE(widget));
	data->m0_0[0][0] = cos(data->theta);
	data->m0_0[1][1] = sin(data->theta);
}

void scale_adjustment_epsilon(GtkWidget *widget, gpointer gdata)
{
	struct serverTalk* data = (struct serverTalk*)gdata;
	char command[256] = "";

	data->epsilon = gtk_range_get_value(GTK_RANGE(widget));
	data->n0_0[0][1] = cos(data->epsilon);
	data->n0_0[1][0] = sin(data->epsilon);
}

void resetPsi(GtkWidget *widget, gpointer gdata)
{
	init_psi((serverTalk*)gdata);
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
