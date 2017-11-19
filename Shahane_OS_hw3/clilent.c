#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

 
int main(int argc , char *argv[])
{
    char inp_str[100];
    char * temp_s;
    char * ip;
    char * p;
    int port = 0;
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
    
    fgets(inp_str, 100, stdin);
    temp_s = strtok(inp_str," ");
    
    int i = 0;
    while (temp_s != NULL)
    {
	if(i == 0)
	{
		ip = temp_s;
	}
	else
	{
		p = temp_s;
	}
	temp_s = strtok (NULL, " ,.-");
	i++;
    }

    port  = atoi(p);

    printf("%s\n",ip);
    printf("%d\n",port);
 
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");
     
    //keep communicating with server
    while(1)
    {
        printf("Enter message : ");
        scanf("%s" , message);
         
        //Send some data
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
         
        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
        if(strcmp(server_reply,"Server is disconnected")==0)
	{
		break;
	} 
        puts("Server reply :");
        puts(server_reply);
    }
     
   
    return 0;
}
