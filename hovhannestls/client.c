#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define VERBOS

int main(int argc, char* argv[])
{
	char inputStr[255];
	int port = 0;
	char *dom;
	int socket_desc;
	struct sockaddr_in client;
	char message[2048];

	if(argc > 2)
	{
		int l = strlen(argv[1]);
		dom = malloc(sizeof(char) * (l+1) );
		strncpy(dom, argv[1], l);
		char *port_str = argv[2];
		port = atoi(port_str);
	}
	else
	{
		dom = malloc(sizeof(char)*255);
		fgets(dom, 255, stdin);
		char tstr[255];
		fgets(tstr, 255, stdin);
		char *port_str = strtok(tstr, " ");
		port = atoi(port_str);
	}

	if(port == 0)
	{
#ifdef VERBOS
		printf("Port is not specified. Using port 8888\n");
#endif
		port = 8888;
	}
#ifdef VERBOS
	printf("Client is started at %s:%d\n", dom, port);
#endif

	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_desc == -1)
	{
		perror("Failed to create socket");
		free(dom);
		exit(1);
	}
#ifdef VERBOS
	printf("Socket created\n");
#endif

	client.sin_addr.s_addr = inet_addr(dom);
	client.sin_family = AF_INET;
	client.sin_port = htons(port);

	if(connect(socket_desc, (struct sockaddr*)&client, sizeof(client)) < 0)
	{
		perror("Connect failed!");
		free(dom);
		exit(1);
	}
#ifdef VERBOS
	printf("Connected\n");
#endif

	free(dom);
	// keep alive
	while(1)
	{
		printf("Enter your message or 'bye' to quite\n");

		fgets(message, 2048, stdin);

		if( send(socket_desc, message, strlen(message), 0) < 0)
		{
			perror("Cannot send");
			exit(1);
		}

		memset(message, 0, 2048);
		int recv_retval =  recv(socket_desc, message, 2048, 0 );
		if( recv_retval < 0)
		{
#ifdef VERBOS
			printf("recv failed\n");
#endif
			break;
		}
		else if (recv_retval == 0)
		{
#ifdef VERBOS
			printf("connection closed\n");
#endif
			break;
		}

		printf("%s", message);

		// Check if connection is closed (again;)
		recv_retval = recv(socket_desc, message, 1, 0);
		
		if( recv_retval < 0)
		{
#ifdef VERBOS
			printf("recv failed\n");
#endif
			break;
		}
		else if (recv_retval == 0)
		{
#ifdef VERBOS
			printf("connection closed\n");
#endif
			break;
		}
	}

	return 0;
}
