// mygrep.c
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        write(1, "Usage: ./mygrep <file> <word>\n", 30);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        write(1, "Cannot open file\n", 17);
        return 1;
    }

    char c, line[1000];
    int i = 0;

    while (read(fd, &c, 1) > 0)
    {
        line[i++] = c;

        if (c == '\n')
        {
            line[i] = '\0';

            int found = 0;
            for (int x = 0; line[x] != '\0'; x++)
            {
                int y = 0;
                while (argv[2][y] && line[x + y] == argv[2][y])
                    y++;

                if (argv[2][y] == '\0')
                {
                    found = 1;
                    break;
                }
            }

            if (found)
                write(1, line, i);
                

            i = 0;
        }
    }


    if (i > 0)
    {
        line[i] = '\0';

        int found = 0;
        for (int x = 0; line[x] != '\0'; x++)
        {
            int y = 0;
            while (argv[2][y] && line[x + y] == argv[2][y])
                y++;

            if (argv[2][y] == '\0')
            {
                found = 1;
                break;
            }
        }

        if (found)
            write(1, line, i);
    }

    close(fd);
    return 0;
}