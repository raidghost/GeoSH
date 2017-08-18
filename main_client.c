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
#define THETA_INIT (PI / 3)
#define EPSILON_INIT (PI / 8)

int main(int argc, char *argv[])
{
	srand(0);

	serverTalk data;
	GtkWidget *window, *grid, *buttonConnexion, *button, *textview;

	data.connecte = FALSE;
	data.theta_00 = THETA_INIT;
	data.epsilon_00 = EPSILON_INIT;
	data.theta_01 = THETA_INIT;
	data.epsilon_01 = EPSILON_INIT;
	data.exit = FALSE;
	data.listProba = NULL;
	data.lengthListProba = 1;
	data.plotFD = NULL;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Client Simulation");
	g_signal_connect_swapped(window, "destroy", G_CALLBACK(gtk_main_quit), window);
	gtk_container_set_border_width(GTK_CONTAINER (window), 10);

	grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 20);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
	gtk_grid_set_row_homogeneous(GTK_GRID(grid), FALSE);
	gtk_grid_set_column_homogeneous(GTK_GRID(grid), FALSE);
	gtk_container_add(GTK_CONTAINER (window), grid);

	data.text2print = gtk_label_new("Commencez par vous connecter au serveur graphique.");
	gtk_grid_attach(GTK_GRID(grid), data.text2print, 0, 0, 2, 1);

	buttonConnexion = gtk_button_new_with_label("Connecter");
	g_signal_connect(buttonConnexion, "clicked", G_CALLBACK(connectionButton), (gpointer)&data);
	gtk_grid_attach(GTK_GRID(grid), buttonConnexion, 2, 0, 1, 1);

	data.probaBar = gtk_level_bar_new();
	gtk_level_bar_set_value(GTK_LEVEL_BAR(data.probaBar), 0);
	gtk_grid_attach(GTK_GRID(grid), data.probaBar, 4, 0, 4, 1);

	GtkAdjustment *vAdjustmentTheta_00 = gtk_adjustment_new(50, 0, 2 * PI, 1, 10, 0);
	button = gtk_scale_new(GTK_ORIENTATION_VERTICAL, GTK_ADJUSTMENT(vAdjustmentTheta_00));
	g_signal_connect(button, "value-changed", G_CALLBACK(scale_adjustment_theta_00), (gpointer)&data);
	gtk_adjustment_set_value(GTK_ADJUSTMENT(vAdjustmentTheta_00), THETA_INIT);
	gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 1, 5);

	GtkAdjustment *vAdjustmentEpsilon_00 = gtk_adjustment_new(50, 0, 2 * PI, 1, 10, 0);
	button = gtk_scale_new(GTK_ORIENTATION_VERTICAL, GTK_ADJUSTMENT(vAdjustmentEpsilon_00));
	g_signal_connect(button, "value-changed", G_CALLBACK(scale_adjustment_epsilon_00), (gpointer)&data);
	gtk_adjustment_set_value(GTK_ADJUSTMENT(vAdjustmentEpsilon_00), EPSILON_INIT);
	gtk_grid_attach(GTK_GRID(grid), button, 1, 1, 1, 5);

	GtkAdjustment *vAdjustmentTheta_01 = gtk_adjustment_new(50, 0, 2 * PI, 1, 10, 0);
	button = gtk_scale_new(GTK_ORIENTATION_VERTICAL, GTK_ADJUSTMENT(vAdjustmentTheta_01));
	g_signal_connect(button, "value-changed", G_CALLBACK(scale_adjustment_theta_01), (gpointer)&data);
	gtk_adjustment_set_value(GTK_ADJUSTMENT(vAdjustmentTheta_01), THETA_INIT);
	gtk_grid_attach(GTK_GRID(grid), button, 2, 1, 1, 5);

	GtkAdjustment *vAdjustmentEpsilon_01 = gtk_adjustment_new(50, 0, 2 * PI, 1, 10, 0);
	button = gtk_scale_new(GTK_ORIENTATION_VERTICAL, GTK_ADJUSTMENT(vAdjustmentEpsilon_01));
	g_signal_connect(button, "value-changed", G_CALLBACK(scale_adjustment_epsilon_01), (gpointer)&data);
	gtk_adjustment_set_value(GTK_ADJUSTMENT(vAdjustmentEpsilon_01), EPSILON_INIT);
	gtk_grid_attach(GTK_GRID(grid), button, 3, 1, 1, 5);

	data.meanBar = gtk_level_bar_new();
	gtk_level_bar_set_value(GTK_LEVEL_BAR(data.meanBar), 0);
	gtk_grid_attach(GTK_GRID(grid), data.meanBar, 4, 1, 4, 1);

	button = gtk_button_new_with_label("Launch");
	g_signal_connect(button, "clicked", G_CALLBACK(launch), (gpointer)&data);
	gtk_grid_attach(GTK_GRID (grid), button, 0, 6, 1, 1);

	button = gtk_button_new_with_label("Quit");
	g_signal_connect(button, "clicked", G_CALLBACK(quit), (gpointer)&data);
	gtk_grid_attach(GTK_GRID (grid), button, 1, 6, 1, 1);

	button = gtk_button_new_with_label("Reset Psi");
	g_signal_connect(button, "clicked", G_CALLBACK(resetPsi), (gpointer)&data);
	gtk_grid_attach(GTK_GRID(grid), button, 2, 6, 1, 1);

	textview = gtk_text_view_new();
	data.bufferGtk = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
	gtk_text_buffer_set_text(data.bufferGtk, "exit", -1);
	gtk_grid_attach(GTK_GRID(grid), textview, 0, 7, 2, 2);

	button = gtk_button_new_with_label("Plot");
	g_signal_connect(button, "clicked", G_CALLBACK(plot), (gpointer)&data);
	gtk_grid_attach(GTK_GRID(grid), button, 2, 7, 1, 1);

	button = gtk_button_new_with_label("Play");
	g_signal_connect(button, "clicked", G_CALLBACK(play), (gpointer)&data);
	gtk_grid_attach(GTK_GRID(grid), button, 3, 7, 1, 1);

	data.randomInputs = gtk_check_button_new_with_label("Random Inputs");
	gtk_grid_attach(GTK_GRID(grid), data.randomInputs, 4, 7, 1, 1);

	data.spinButtonX = gtk_spin_button_new_with_range(0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), data.spinButtonX, 5,7,1,1);

	data.spinButtonY = gtk_spin_button_new_with_range(0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), data.spinButtonY, 6,7,1,1);


	gtk_widget_show_all(window);
	update_measurements(&data);
	connectionButton(buttonConnexion, (gpointer)&data);
	init_psi(&data);
	gtk_main();
	if(data.plotFD != NULL)
		fclose(data.plotFD);
	
	return EXIT_SUCCESS;
}
