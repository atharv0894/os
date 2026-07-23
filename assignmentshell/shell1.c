#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    char input[1024];
    char *args[100];

    while (1)
    {
        printf("shell>");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("\nExiting shell...\n");
            break;
        }
        input[strcspn(input, "\n")] = '\0';
   
       

        // Tokenize input into arguments
        int i = 0;
        char *token = strtok(input, " ");
        while (token != NULL && i < 99) 
        {
            args[i] = token;
            i++;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;


        if (args[0] == NULL)
            continue;


        if (strcmp(args[0], "exit") == 0)
        {
            printf("Exiting shell...\n");
            break;
        }


        if (strcmp(args[0], "cd") == 0)
        {
            if (args[1] == NULL)
            {
                printf("cd: missing directory\n");
            }
            else
            {
                if (chdir(args[1]) != 0)
                {
                    perror("cd");
                }
            }
            continue; 
        }


        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork failed");
        }
        else if (pid == 0)
        {

            execvp(args[0], args);


            perror("Command not found");
            exit(1);
        }
        else
        {

            wait(NULL);
        }
        getchar();
    }

    return 0;
}


