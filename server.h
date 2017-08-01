#ifndef SERVER_H
#define SERVER_H

#define PORT 2000
#define HOST "localhost"
#define MATRIX_DIM 2

typedef struct PairInt PairInt;
struct PairInt
{
	int x;
	int y;
};

void error(const char *msg);
PairInt init_server(void);
void close_server(PairInt sockets);
int parseCommand(char* command, double a[MATRIX_DIM][MATRIX_DIM], double b[MATRIX_DIM][MATRIX_DIM]);

#endif
