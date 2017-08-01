#ifndef SERVER_H
#define SERVER_H

#include "Structs.h"
#define COMMA2COLON(token) ({\
		int i = 0;\
		while(token[i] != '\0')\
		{\
			if(token[i] == ',')\
			{\
				token[i] = '.';\
				break;\
			}\
			i++;\
		}})

void error(const char *msg);
PairInt init_server(void);
void close_server(PairInt sockets);
int parseCommand(char* command, Point* psi);

#endif
