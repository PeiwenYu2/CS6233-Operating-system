#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 1024
#define SHAREDMEM_SIZE 1024
int main(){
    char user_string[BUFFER_SIZE];
    char secret_key[] = "C00L";
    int shmid,i,str_len;
    char *shm;
    key_t key;
	/* Name our shared memory segment as 6233 */
    key = 6233;
	/*	Create the shared memory segment 
		shmget() function shall return the shared memory identifier associated with key
		IPC_CREAT | 0666 for a server (i.e., creating and granting read and write access to the server)
	*/
    if ((shmid = shmget(key, SHAREDMEM_SIZE, IPC_CREAT | 0666)) == -1) {
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
		printf("Please enter the message: ");
		/* read from the user input */
        fgets(user_string, BUFFER_SIZE, stdin);
		str_len = strlen(user_string);
		/* alpha numeric strings or not */
		for(i = 0; i< str_len - 1; i++)
		{
			if((user_string[i] >= 0x30 && user_string[i] <= 0x39) ||(user_string[i] >= 0x41 && user_string[i] <= 0x5A)||(user_string[i] >= 0x61 && user_string[i] <= 0x7A))
				continue;
			else
				break;
		}
		/* copy the user input to shared memory */
		if(i == str_len - 1)
			if(strstr(user_string, secret_key) != NULL){
            	strcpy(shm,user_string);
				while(1){
					strcpy(user_string,shm);
					if(user_string[0] != '!')
						break;
				}
			}
	}
    return EXIT_SUCCESS;
}