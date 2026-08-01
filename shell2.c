#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

static char shell_path[1024] = "/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin";

static int my_execvp(char *cmd, char *argv[])
{
    char fullpath[1024];
    char *path_copy;
    char *token;

    if (strchr(cmd, '/') != NULL) {
        execv(cmd, argv);
        return -1;
    }

    path_copy = strdup(shell_path);
    if (path_copy == NULL) {
        return -1;
    }

    token = strtok(path_copy, ":");
    while (token != NULL) {
        snprintf(fullpath, sizeof(fullpath), "%s/%s", token, cmd);
        execl(fullpath, cmd, argv[0], argv[1], argv[2], argv[3], argv[4],
              argv[5], argv[6], argv[7], argv[8], argv[9], NULL);
        token = strtok(NULL, ":");
    }

    free(path_copy);
    return -1;
}

int main(int argc, char const *argv[])
{
    char input[1024];
    char *args[100];
    char *redir_in = NULL;
    char *redir_out = NULL;
    int append_mode = 0;

    while (1)
    {
        printf("shell> ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("\nExiting shell...\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0)
            continue;

        redir_in = NULL;
        redir_out = NULL;
        append_mode = 0;

        char *append_ptr = strstr(input, ">>");
        char *out_ptr = NULL;

        if (append_ptr != NULL)
        {
            append_mode = 1;
            *append_ptr = '\0';
            redir_out = append_ptr + 2;
            while (*redir_out == ' ') redir_out++;
            char *end = redir_out + strlen(redir_out) - 1;
            while (end > redir_out && *end == ' ') *end-- = '\0';
        }
        else
        {
            out_ptr = strstr(input, ">");
            if (out_ptr != NULL && (out_ptr == input || *(out_ptr - 1) != '>'))
            {
                *out_ptr = '\0';
                redir_out = out_ptr + 1;
                while (*redir_out == ' ') redir_out++;
                char *end = redir_out + strlen(redir_out) - 1;
                while (end > redir_out && *end == ' ') *end-- = '\0';
            }
        }

        char *in_ptr = strstr(input, "<");
        if (in_ptr != NULL)
        {
            *in_ptr = '\0';
            redir_in = in_ptr + 1;
            while (*redir_in == ' ') redir_in++;
            char *end = redir_in + strlen(redir_in) - 1;
            while (end > redir_in && *end == ' ') *end-- = '\0';
        }

        int i = 0;
        char *token = strtok(input, " \t");
        while (token != NULL && i < 99)
        {
            args[i] = token;
            i++;
            token = strtok(NULL, " \t");
        }
        args[i] = NULL;

        if (args[0] == NULL)
            continue;

        if (strncmp(args[0], "PATH=", 5) == 0)
        {
            char *new_path = args[0] + 5;
            if (strlen(new_path) == 0)
            {
                printf("Usage: PATH=/usr/bin:/bin:/sbin\n");
            }
            else
            {
                strncpy(shell_path, new_path, sizeof(shell_path) - 1);
                shell_path[sizeof(shell_path) - 1] = '\0';
            }
            continue;
        }

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

        if (redir_in != NULL)
        {
            int fd = open(redir_in, O_RDONLY);
            if (fd < 0)
            {
                fprintf(stderr, "Error: cannot open input file '%s': %s\n",
                        redir_in, strerror(errno));
                continue;
            }
            close(fd);
        }

        if (redir_out != NULL)
        {
            if (strlen(redir_out) == 0)
            {
                fprintf(stderr, "Error: missing filename for output redirection\n");
                continue;
            }
            int flags = append_mode ? (O_WRONLY | O_CREAT | O_APPEND)
                                    : (O_WRONLY | O_CREAT | O_TRUNC);
            int fd = open(redir_out, flags, 0644);
            if (fd < 0)
            {
                fprintf(stderr, "Error: cannot open output file '%s': %s\n",
                        redir_out, strerror(errno));
                continue;
            }
            close(fd);
        }

        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork failed");
            continue;
        }
        else if (pid == 0)
        {
            if (redir_in != NULL)
            {
                int fd = open(redir_in, O_RDONLY);
                if (fd < 0)
                {
                    fprintf(stderr, "Error: cannot open '%s': %s\n",
                            redir_in, strerror(errno));
                    exit(1);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            if (redir_out != NULL)
            {
                int flags = append_mode ? (O_WRONLY | O_CREAT | O_APPEND)
                                        : (O_WRONLY | O_CREAT | O_TRUNC);
                int fd = open(redir_out, flags, 0644);
                if (fd < 0)
                {
                    fprintf(stderr, "Error: cannot open '%s': %s\n",
                            redir_out, strerror(errno));
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            my_execvp(args[0], args);
            fprintf(stderr, "%s: command not found\n", args[0]);
            exit(1);
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}
