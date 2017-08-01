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
	data->psi = measure(data->psi, data->m, data->n);
	g_print("play %lf %lf %lf %lf\n", data->psi.x, data->psi.y, data->psi.z, data->psi.h);
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
	data->m[0][0] = cos(data->theta);
	data->m[1][1] = sin(data->theta);
}

void scale_adjustment_epsilon(GtkWidget *widget, gpointer gdata)
{
	struct serverTalk* data = (struct serverTalk*)gdata;
	char command[256] = "";

	data->epsilon = gtk_range_get_value(GTK_RANGE(widget));
	data->n[0][1] = cos(data->epsilon);
	data->n[1][0] = sin(data->epsilon);
}

void resetPsi(GtkWidget *widget, gpointer gdata)
{
	struct serverTalk* data = (serverTalk*)gdata;
	data->psi.x = 1/sqrt(2);
	data->psi.y = 0;
	data->psi.z = 0;
	data->psi.h = 1/sqrt(2);
	talk2server(data);
}

void quit(GtkWidget *widget, gpointer gdata)
{
	struct serverTalk* data = (serverTalk*)gdata;
	data->exit = TRUE;
	talk2server(data);
	close(data->sockfd);
	gtk_main_quit();
}
