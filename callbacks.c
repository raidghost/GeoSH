#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <gtk/gtk.h>

#include "client.h"
#include "callbacks.h"

#define PORT 2000
#define HOST "localhost"

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
	sprintf(command, "new_matrices 1 %lf %lf", data->theta, data->epsilon);
	gtk_text_buffer_set_text(data->bufferGtk, command, -1);
}

void scale_adjustment_epsilon(GtkWidget *widget, gpointer gdata)
{
	struct serverTalk* data = (struct serverTalk*)gdata;
	char command[256] = "";

	data->epsilon = gtk_range_get_value(GTK_RANGE(widget));
	sprintf(command, "new_matrices 1 %lf %lf", data->epsilon, data->epsilon);
	gtk_text_buffer_set_text(data->bufferGtk, command, -1);
}

void resetPsi(GtkWidget *widget, gpointer gdata)
{
	struct serverTalk* data = (serverTalk*)gdata;
	gtk_text_buffer_set_text(data->bufferGtk, "resetPsi", -1);
	talk2server(gdata);
}

void quit(GtkWidget *widget, gpointer gdata)
{
	struct serverTalk* data = (serverTalk*)gdata;
	gtk_text_buffer_set_text(data->bufferGtk, "exit", -1);
	talk2server(data);
	close(data->sockfd);
	gtk_main_quit();
}
