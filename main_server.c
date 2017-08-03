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
#include "server.h"

#define PI 3.14159
#define PORT 2000
#define HOST "localhost"
#define TRUE 1
#define FALSE !TRUE


int main(int argc, char *argv[])
{
	int run = TRUE, keyDown = FALSE, parseAnswer = 0;
	unsigned int i = 0, j = 0;
	double angleX = 0, angleY = 0, angleZ = 0;

	Point psiOld, psiNew;
	psiOld.x = 1/sqrt(2);
	psiOld.y = 0;
	psiOld.z = 0;
	psiOld.h = 1/sqrt(2);
	psiNew = psiOld;

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
				fprintf(stderr, "lecture\n");
				fflush(stderr);
				result = recv(sockets.y, buffer, 255, 0);
				if(result == 0)
				{
					//This means the other side closed the socket
					fprintf(stderr, "Le client a fermé la connexion. On se barre.\n");
					run = FALSE;//Le "close_server" est après la boucle while(run).
				}
				else
				{
					psiOld = psiNew;
					parseAnswer = parseCommand(buffer, &psiNew);
					switch(parseAnswer)
					{
						case 1:
						fprintf(stderr, "Psi a bien été mis à jour.\n");
						break;

						case -1:
						fprintf(stderr, "Erreur lors de la mise à jour de Psi.\n");
						break;

						case 0://On quite le programme à la demande du client.
						run = FALSE;
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
