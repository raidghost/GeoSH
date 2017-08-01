//#ifndef CLIENT_H
//#define CLIENT_H

typedef struct serverTalk serverTalk;
struct serverTalk
{
	int sockfd;
	double theta;
	double epsilon;
	GtkTextBuffer *bufferGtk;
	struct sockaddr_in serverAddress;
};

int connect2server(serverTalk *data);
int talk2server(serverTalk *gdata);

//#endif
