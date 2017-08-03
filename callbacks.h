#ifndef CALLBACKS_H
#define CALLBACKS_H

void play(GtkWidget* button, gpointer gdata);
int connectionButton(GtkWidget* widget, gpointer gdata);
void launch(GtkWidget *button, gpointer gdata);
void scale_adjustment_theta_00(GtkWidget *widget, gpointer gdata);
void scale_adjustment_epsilon_00(GtkWidget *widget, gpointer gdata);
void scale_adjustment_theta_01(GtkWidget *widget, gpointer gdata);
void scale_adjustment_epsilon_01(GtkWidget *widget, gpointer gdata);
void resetPsi(GtkWidget *widget, gpointer gdata);
void quit(GtkWidget *widget, gpointer gdata);

#endif
