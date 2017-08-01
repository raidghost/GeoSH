#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "graphics.h"
#include "game.h"
#include "server.h"

#define PI 3.14159
#define PORT 2000
#define HOST "localhost"
#define TRUE 1
#define FALSE !TRUE


int main(int argc, char *argv[])
{
	int run = TRUE, keyDown = FALSE;
	unsigned int i = 0, j = 0;
	double angleX = 0, angleY = 0, angleZ = 0;

	Point psiOld, psiNew;
	psiOld.x = 0;
	psiOld.y = 1/sqrt(2);
	psiOld.z = 1/sqrt(2);
	psiOld.h = 0;
	psiNew = psiOld;
	double a[2][2], b[2][2];
	a[0][0] = 1;
	a[0][1] = 0;
	a[1][0] = 0;
	a[1][1] = 1;
	b[0][0] = 1;
	b[0][1] = 0;
	b[1][0] = 0;
	b[1][1] = 1;

	/*Initialisation du serveur
		bzero(buffer,256);
	 n = read(newsockfd,buffer,255);
	 if (n < 0)
	 	error("ERROR reading from socket");
	
	 fprintf(stderr, "Here is the message: %s\n",buffer);
	 n = write(newsockfd,"1",2);
	 if (n < 0) error("ERROR writing to socket");
	 close(newsockfd);
	 close(sockfd);
	 //Fin initialisation server*/

	int n;
	char buffer[256];
	int result;
	fd_set readset;
	struct timeval timeout;//timeout pour select

	fprintf(stderr, "Initialisation serveur.\n");
	PairInt sockets = init_server();
	fprintf(stderr, "Serveur initialisé.\n");
	
	SDL_Event event, event2;

	//Initialisation de la SDL
	SDL_Init(SDL_INIT_VIDEO);
	atexit(SDL_Quit);
	SDL_WM_SetCaption("Simulation", NULL);
	SDL_SetVideoMode(640, 480, 32, SDL_OPENGL);
//	SDL_EnableKeyRepeat(100, SDL_DEFAULT_REPEAT_INTERVAL);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70,(double)640/480,1,1000);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	showSphere(&psiOld, &psiNew, angleX, angleY, angleZ);



	while(run)
	{
		do
		{
			//select décrémente ces valeurs à chaque appel.
			timeout.tv_sec = 0;
			timeout.tv_usec = 10000;
			FD_ZERO(&readset);
			FD_SET(sockets.y, &readset);
			result = select(sockets.y + 1, &readset, NULL, NULL, &timeout);
		}while(result == -1 && errno == EINTR);

		if(result > 0)
		{
			if(FD_ISSET(sockets.y, &readset))
			{
				//The has data available to be read
				bzero(buffer, 255);
				result = recv(sockets.y, buffer, 255, 0);
				if(result == 0)
				{
					//This means the other side closed the socket
					fprintf(stderr, "Le client a fermé la connexion. On se barre.\n");
					run = FALSE;//Le "close_server" est après la boucle while(run).
				}
				else
				{
					if(parseCommand(buffer, a, b) == -1)
						fprintf(stderr, "Mauvaise commande\n");
					else
					{
						if(strcmp(buffer, "new_matrices") == 0)
						{
							fprintf(stderr, "changement matrices\n");
							psiOld = psiNew;
							psiNew = measure(&psiNew, a, b);
						}
						else if(strcmp(buffer, "resetPsi") == 0)
						{
							psiNew.x = 0;
							psiNew.y = 1/sqrt(2);
							psiNew.z = 1/sqrt(2);
							psiNew.h = 0;
						}
						else if(strcmp(buffer, "exit") == 0)
						{
							fprintf(stderr, "Bye !\n");
							run = FALSE;
						}
					}
				}
			}
		}
		else if (result < 0)
		{
			//An error ocurred, just print it to stdout
			fprintf(stderr, "Error on select(): %s", strerror(errno));
		}
	/*	else
			fprintf(stderr, "Rien à lire.\n");*/
	
		SDL_PollEvent(&event);

		switch(event.type)
		{
			case SDL_QUIT:
			run = FALSE;
			break;

			case SDL_KEYUP:
			keyDown = FALSE;
			break;
			
			case SDL_KEYDOWN:
			if(!keyDown)
			{
				//keyDown = TRUE;
				keyDown = FALSE;
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
					run = FALSE;
					break;

					case SDLK_RETURN:
					fprintf(stderr, "okok");
					break;

					case SDLK_RIGHT:
					angleZ++;
					break;

					case SDLK_LEFT:
					angleZ--;
					break;
				
					case SDLK_UP:
					angleY++;
					break;
				
					case SDLK_DOWN:
					angleY--;
					break;

					case SDLK_KP6:
					angleX++;
					break;

					case SDLK_KP4:
					angleX--;
				}
			}
		}
		showSphere(&psiOld, &psiNew, angleX, angleY, angleZ);
	}
	
	close_server(sockets);
	return 0;
}



