#include <stdio.h>  /* use of printf() */
#include <stdlib.h> /* use of malloc(), realloc(), free(), execvp(), EXIT_SUCCESS */
#include <unistd.h> /* use of chdir(), fork(), exec(), pid_t */
#include <string.h> /* use of strcmp(), strtok() */
#include <ctype.h> /* use of toupper */
#define WORKDONE 1
#define ARG_BUFSIZE 64
#define ARG_DELIM " \t\r\n\a"
char *shell_read_line()
{
    char *line = NULL;
    size_t bufsize = 0; /* have getline allocate the buffer size */
    getline(&line, &bufsize, stdin);    /* read a line */
    return line;
}
char **shell_split_line(char *line)
{
    int bufsize = ARG_BUFSIZE, position = 0;
    char **cmd = malloc(bufsize * sizeof(char*)); /* allocate the initial buffer size for user command */
    char *arg;
    arg = strtok(line, ARG_DELIM);  /* get the first argument from the user command */
    while (arg != NULL) {
        cmd[position] = arg;
        position++;
        if (position >= bufsize)    /* increase the buffer size if the user command is too long */
        {
            bufsize += ARG_BUFSIZE;
            cmd = realloc(cmd, bufsize * sizeof(char*));
        }
        arg = strtok(NULL, ARG_DELIM); /* get the following argument from the user command */
    }
    cmd[position] = NULL;
    return cmd;
}

int shell_execute(char **cmd)
{
    int status;
    pid_t pid;
    if (cmd[0] == NULL)        /* An empty command was entered */
        return WORKDONE;       /* command excuted */
    if (strcmp(cmd[0], "cd") == 0)  /* excute cd comand */
    {
        chdir(cmd[1]);
        return WORKDONE;      /* command excuted */
    }
    pid = fork();             /* fork a child process */
    if (pid == 0)
        execvp(cmd[0], cmd);  /* excute other user command */
    else
        while (wait(&status) != pid);  /* wait for child process to be finished */
    return WORKDONE;                          /* command excuted */
}

void myshell()
{
    char *line;
    char **cmd;
    int status;
    
    do {
        printf(">> ");
        line = shell_read_line();      /* read from user input */
        cmd = shell_split_line(line);  /* parse user input */
        status = shell_execute(cmd);   /* decide execution status */
        free(line);
        free(cmd);
    } while (status);
}
int main(int argc, char* argv[])
{
    myshell();
    return EXIT_SUCCESS;
}