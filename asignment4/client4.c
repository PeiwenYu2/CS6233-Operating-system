/* Pass the server ip address and port number in as two arguments when run the client 
----  ./client.out server_ipaddress portnum */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 1024
int main(int argc, char *argv[])
{
    int socketfd, portnum, str_len, i;
    char buffer[BUFFER_SIZE];
    char secret_key[] = "C00L";
    struct sockaddr_in server_addr;
    struct hostent *server;
	/* check whether server ip address and port number are provided or not */
    if (argc < 3) {
       fprintf(stderr,"Error,host ipaddress and port needed\n");
       exit(EXIT_FAILURE);
    }
    portnum = atoi(argv[2]);
	/* create a new socket */
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0){
        perror("Error on opening socket");
		exit(EXIT_FAILURE);
	 }
    server = gethostbyname(argv[1]);
    if (server == NULL){
        fprintf(stderr,"Error,no such host\n");
        exit(EXIT_FAILURE);
    }
	/* initialize the server_addr */
    memset((char*)&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&server_addr.sin_addr.s_addr,
         server->h_length);
    server_addr.sin_port = htons(portnum);
	/* connect to the server */
    if (connect(socketfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0){
        perror("Error on connecting");
		exit(EXIT_FAILURE);
	}
	/* initialize buffer */
	memset(buffer,0,BUFFER_SIZE);
    while(1){
		printf("Please enter the message: ");
		/* read from the user input */
        fgets(buffer, BUFFER_SIZE, stdin);
		str_len = strlen(buffer);
		/* check whether user enter an alpha numeric strings or not */
		for(i = 0; i< str_len - 1; i++)
		{
			if((buffer[i] >= 0x30 && buffer[i] <= 0x39) ||(buffer[i] >= 0x41 && buffer[i] <= 0x5A)||(buffer[i] >= 0x61 && buffer[i] <= 0x7A))
				continue;
			else
				break;
		}
		if(i == str_len - 1)
        	if(strstr(buffer, secret_key) != NULL){
				/* write the user input to socket */
		    	if (write(socketfd,buffer,strlen(buffer)) < 0) {
        			perror("Error on writing to socket");
					exit(EXIT_FAILURE);
		 		}
				else {
						/* read the reply form the server */
	    				if (read(socketfd,buffer,BUFFER_SIZE) < 0) {
        					perror("Error on reading from socket");
							exit(EXIT_FAILURE);
		 	   	 		}
		   	 			printf("%s\n",buffer);
					}
			}
	}
    return EXIT_SUCCESS;
}