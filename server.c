#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "server.h"

void error(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

PairInt init_server(void)
{
	PairInt sockets;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;

	sockets.x = socket(AF_INET, SOCK_STREAM, 0);
	if(sockets.x < 0)
		error("ERROR opening socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT);
	if(bind(sockets.x, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding");

	listen(sockets.x,5);
	fprintf(stderr, "listen\n");
	clilen = sizeof(cli_addr);
	fprintf(stderr, "Attente de connexion\n");
	sockets.y = accept(sockets.x, (struct sockaddr *) &cli_addr, &clilen);
	fprintf(stderr, "Nouveau mec connecté\n");
	if(sockets.y < 0)
		error("ERROR on accept");
	
	return sockets;
}

void close_server(PairInt sockets)
{
	close(sockets.y);
	close(sockets.x);
}

int parseCommand(char* command, double a[MATRIX_DIM][MATRIX_DIM], double b[MATRIX_DIM][MATRIX_DIM])
{
	char word[256] = {0};
	char *token = NULL;
	const char delimiter[3] = " \t";
	token = strtok(command, delimiter);
	if(token == NULL)
		return -1;

	strcpy(word, token);

	if(strcmp(word, "exit") == 0)
		return 0;
	else if(strcmp(word, "resetPsi") == 0)
	{
		strcpy(command, word);
		return 1;
	}
	else if(strcmp(word, "new_matrices") == 0)
	{
		token = strtok(NULL, delimiter);
		if(token == NULL)
			return -1;
		else
		{
			strcpy(word, token);
			int preselect = atoi(word);

			//On charge les angles
			token = strtok(NULL, delimiter);
			double theta = 0, epsilon = 0;

			if(token != NULL)
			{
				theta = atof(token);
				token = strtok(NULL, delimiter);

				if(token != NULL)
					epsilon = atof(token);

			}

			fprintf(stderr, "On prend theta = %lf et epsilon = %lf", theta, epsilon);
			fflush(stderr);
			switch(preselect)
			{
				case 1:
				a[0][0] = cos(theta);
				a[0][1] = 0;
				a[1][0] = 0;
				a[1][1] = sin(theta);
				b[0][0] = cos(epsilon);
				b[0][1] = 0;
				b[1][0] = 0;
				b[1][1] = sin(epsilon);
				break;

				case 2:
				a[0][0] = cos(theta);
				a[0][1] = 0;
				a[1][0] = 0;
				a[1][1] = sin(theta);
				b[0][0] = cos(epsilon);
				b[0][1] = 0;
				b[1][0] = 0;
				b[1][1] = sin(epsilon);
				break;
			}
			return 1;
		}
	}
	else //Commande inconnue
		return -1;
	
	fprintf(stderr, "On a fini de parser !\n");
}
