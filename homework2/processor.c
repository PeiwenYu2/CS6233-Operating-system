#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 1024
#define SHAREDMEM_SIZE 1024
int main(){
    int shmid,str_len;
    key_t key;
    char *shm, user_string[BUFFER_SIZE];
	/* Get the segment named "6223", created by the server */
    key = 6233;
 	/* Locate the shared memory segment
	   shmget() function shall return the shared memory identifier associated with key
	   0666 for any client (i.e., granting read and write access to the client)
	*/
    if ((shmid = shmget(key, SHAREDMEM_SIZE, 0666)) == -1) {
        perror("shmget");
        return EXIT_FAILURE;
    }
	/*	Attach the shared memory segment identified by shimid to the address space shm of the process 
	shmat() returns the address of the attached shared memory segment
	*/
    if ((shm = (char*) shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        return EXIT_FAILURE;
    }
    while(1){
		/* Read from shared memory */
        strcpy(user_string,shm);
		
		/* Print the digit count along with the original line and clear the shared memory and buffer */
       	if(user_string[0] != '!'){
			str_len = strlen(user_string);
			printf("the user input: %sthe number of digits: %d \n", user_string, str_len - 1);
			memset (shm,'!',1);
		}
    }
    return EXIT_SUCCESS;
}