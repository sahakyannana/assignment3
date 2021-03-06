#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen, pid;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);

     clilen = sizeof(cli_addr);


     while (1) {
        newsockfd = accept(sockfd,
              (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");
        pid = fork();
        if (pid == 0)  {
          while (1) {
            close(sockfd);
            n = read(newsockfd,buffer,255);

            if (n < 0) error("ERROR reading from socket");

            else if(strcmp(buffer, "bye\n") == 0){
              printf("Close connection\n");
              close(newsockfd);
              return 0;
            }

            else printf("Here is the message: %s\n",buffer);
            n = write(newsockfd,"I got your message",18);
          }


          exit(0);
      }
        else close(newsockfd);
    } /* end of while */
     return 0;
}
