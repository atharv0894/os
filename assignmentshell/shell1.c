#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

void shellloop()
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
    }
}
int main(int argc, char const *argv[])
{
    
       she

    return 0;
}
