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

#define PORT 2000
#define HOST "localhost"


void error(const char *msg)
{
	perror(msg);
	g_print("%s", msg);
	exit(EXIT_FAILURE);
}

int connect2server(serverTalk *data)
{
	int sockfd, n;
	struct hostent *server;
	struct sockaddr_in serverAddress = data->serverAddress;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		error("Erreur d'ouverture socket\n");
		return EXIT_FAILURE;
	}
	data->sockfd = sockfd;

	server = gethostbyname(HOST);
	if(server == NULL)
	{
		error("Serveur introuvable.");
		return EXIT_FAILURE;
	}
	bzero((char*) &serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	bcopy((char*) server->h_addr, (char*) &serverAddress.sin_addr.s_addr, server->h_length);
	serverAddress.sin_port = htons(PORT);
	data->serverAddress = serverAddress;

	if(connect(sockfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
		error("Erreur de connection");
	

	return EXIT_SUCCESS;
}

int talk2server(struct serverTalk *data)
{
	int sockfd = data->sockfd, n = 0;
	char* buffer = (char*)malloc(256 * sizeof(char));
	GtkTextIter start, end;
	struct sockaddr_in serverAddress = data->serverAddress;

	gtk_text_buffer_get_bounds(data->bufferGtk, &start, &end);
	bzero(buffer, 256);

	if(data->exit)
		strcpy(buffer, "exit");
	else
	{
		sprintf(buffer, "newPsi %LF %LF %LF %LF", data->psi_current->x, data->psi_current->y, data->psi_current->z, data->psi_current->h);
		g_print("On envoie au serveur \"%s\"\n", buffer);
	}

	n = write(sockfd, buffer, strlen(buffer));
	if(n < 0)
		error("Impossible d'écrire dans le socket.");

	free(buffer);
	return EXIT_SUCCESS;
}
