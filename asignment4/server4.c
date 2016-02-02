/* Pass the port number in as an argument when run the server 
----  ./server.out portnum (2000 to 65535)*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#define BUFFER_SIZE 1024
int line_num,sum_digitcounter;
/* Signal Handler for SIGINT */
void sigintHandler(int sig_num)
{
    signal(SIGINT, sigintHandler);
    printf(" the number of lines received so far is %d and the total digit count is %d \n", line_num, sum_digitcounter);
    exit(EXIT_SUCCESS);
}
int main(int argc, char *argv[])
{
     int socketfd, acceptfd, portnum;
	 socklen_t clilen;
     char buffer[BUFFER_SIZE];
     struct sockaddr_in server_addr, client_addr;
     int i, digit_counter;
	 FILE *fpt; 
	 signal(SIGINT, sigintHandler);
	 /* initialize the buffer */
	 memset(buffer,0,BUFFER_SIZE);
	 line_num = 0;
	 sum_digitcounter = 0;
	 /* check whether port number is provided or not */
     if (argc < 2) {
         fprintf(stderr,"Error, Port not provided\n");
         exit(EXIT_FAILURE);
     }
	 /* create a new socket */
     socketfd = socket(AF_INET, SOCK_STREAM, 0);
     if (socketfd < 0){
        perror("Error on opening socket");
		exit(EXIT_FAILURE);
	 }
	 /* initialize the server_addr */
	 memset((char*)&server_addr,0,sizeof(server_addr));
     portnum = atoi(argv[1]);
     server_addr.sin_family = AF_INET;
     server_addr.sin_addr.s_addr = INADDR_ANY;
     server_addr.sin_port = htons(portnum);
	 /* binds a socket to an address */
     if (bind(socketfd, (struct sockaddr *) &server_addr,sizeof(server_addr)) < 0) {
        perror("Error on binding");
 		exit(EXIT_FAILURE);
     }
	 /* listen on the socket for connections */
     listen(socketfd,5);
     clilen = sizeof(client_addr);
	 /* The accept() system call causes the process to block until a client connects to the server. */
     acceptfd = accept(socketfd, (struct sockaddr *) &client_addr, &clilen);
     if (acceptfd < 0){
        perror("Error on accept");
 		exit(EXIT_FAILURE);
     }
	 while(1){
		/* read from socket */
     	if (read(acceptfd,buffer,BUFFER_SIZE) < 0) {
        perror("Error on reading from socket");
 		exit(EXIT_FAILURE);
    	}
		/* count line number */
		line_num++;
		/* count digit number in on line */
		digit_counter  = 0;
		for( i = 0; i< strlen(buffer); i++)
			if(buffer[i] >= 0x30 && buffer[i]<= 0x39)
				digit_counter++;
		sum_digitcounter += digit_counter;
		/*  dumps the digit count along with the original line in the secrets.out file */
		fpt = fopen("secrets.out","a");
		fprintf(fpt,"the user input: %sthe number of digits: %d \n",buffer, digit_counter); 
		fclose(fpt); 
		/* clear the buffer */
		memset(buffer,0,strlen(buffer));
		/* inform the client */
	    if (write(acceptfd,"Message received",18) < 0) {
        perror("Error on writing to socket");
 		exit(EXIT_FAILURE);
		}
    }
    return EXIT_SUCCESS;
}