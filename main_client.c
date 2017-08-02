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

#define PORT 2000
#define HOST "localhost"

#define PI 3.14159

int main(int argc, char *argv[])
{
	serverTalk data;
	GtkWidget *window, *grid, *button, *label, *textview;

	data.theta = PI / 3;
	data.epsilon = PI / 8;
	data.exit = FALSE;

	data.psi.x = 1/sqrt(2);
	data.psi.y = 0;
	data.psi.z = 0;
	data.psi.h = 1/sqrt(2);

	data.m[0][0] = cos(data.theta);
	data.m[0][1] = 0;
	data.m[1][0] = 0;
	data.m[1][1] = sin(data.theta);

	data.n[0][0] = 0;
	data.n[0][1] = cos(data.epsilon);
	data.n[1][0] = sin(data.epsilon);
	data.n[1][1] = 0;

//	data.m[0][0] = 1;
//	data.m[0][1] = 0;
//	data.m[1][0] = 0;
//	data.m[1][1] = 1;

//	data.n[0][0] = 1;
//	data.n[0][1] = 0;
//	data.n[1][0] = 0;
//	data.n[1][1] = 1;


	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Client Simulation");
	g_signal_connect_swapped(window, "destroy", G_CALLBACK(gtk_main_quit), window);
	gtk_container_set_border_width(GTK_CONTAINER (window), 10);

	grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 20);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
	gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
	gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
	gtk_container_add(GTK_CONTAINER (window), grid);

	label = gtk_label_new("Bonjour monsieur");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 2, 1);

	button = gtk_button_new_with_label("Connecter");
	g_signal_connect(button, "clicked", G_CALLBACK(connectionButton), (gpointer)&data);
	gtk_grid_attach(GTK_GRID(grid), button, 2, 0, 1, 1);

	button = gtk_button_new_with_label("Launch");
	g_signal_connect(button, "clicked", G_CALLBACK(launch), (gpointer)&data);
	gtk_grid_attach(GTK_GRID (grid), button, 0, 2, 1, 1);

	button = gtk_button_new_with_label("Quit");
	g_signal_connect(button, "clicked", G_CALLBACK(quit), (gpointer)&data);
	gtk_grid_attach(GTK_GRID (grid), button, 1, 2, 1, 1);

	GtkAdjustment *vAdjustmentTheta = gtk_adjustment_new(50, 0, 2 * PI, 1, 10, 0);
	button = gtk_scale_new(GTK_ORIENTATION_VERTICAL, GTK_ADJUSTMENT(vAdjustmentTheta));
	g_signal_connect(button, "value-changed", G_CALLBACK(scale_adjustment_theta), &data);
	gtk_grid_attach(GTK_GRID(grid), button, 2, 1, 1, 1);

	GtkAdjustment *vAdjustmentEpsilon = gtk_adjustment_new(50, 0, 2 * PI, 1, 10, 0);
	button = gtk_scale_new(GTK_ORIENTATION_VERTICAL, GTK_ADJUSTMENT(vAdjustmentEpsilon));
	g_signal_connect(button, "value-changed", G_CALLBACK(scale_adjustment_epsilon), &data);
	gtk_grid_attach(GTK_GRID(grid), button, 3, 1, 1, 1);

	button = gtk_button_new_with_label("Reset Psi");
	g_signal_connect(button, "clicked", G_CALLBACK(resetPsi), (gpointer)&data);
	gtk_grid_attach(GTK_GRID(grid), button, 2, 2, 1, 1);

	button = gtk_button_new_with_label("Play");
	g_signal_connect(button, "clicked", G_CALLBACK(play), (gpointer)&data);
	gtk_grid_attach(GTK_GRID(grid), button, 2, 3, 1, 1);

	textview = gtk_text_view_new();
	data.bufferGtk = gtk_text_view_get_buffer(GTK_TEXT_VIEW (textview));
	gtk_text_buffer_set_text(data.bufferGtk, "new_matrices", -1);
	gtk_grid_attach(GTK_GRID(grid), textview, 0, 1, 2, 1);

	gtk_widget_show_all(window);
	gtk_main();
	
	return EXIT_SUCCESS;
}
