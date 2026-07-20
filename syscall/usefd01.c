#include <stdio.h>      
#include <unistd.h>
#include <stdlib.h>     

int main()
{
    char buffer[100];      
    int n;                 
    int num;               


    n = read(0, buffer, sizeof(buffer) - 1);

    
    if (n == -1)
    {
        perror("read");
        return 1;
    }


    buffer[n] = '\0';

    num = atoi(buffer);


    printf("Integer entered = %d\n", num);


    int x = 999666333;

    char digits[20];
    int i = 0;


    if (x == 0)
    {
        digits[i++] = '0';
    }
    else
    {

        while (x > 0)
        {
            digits[i++] = (x % 10) + '0';
            x = x / 10;
        }
    }


    while (i > 0)
    {
        i--;
        write(1, &digits[i], 1);
    }


    write(1, "\n", 1);

    return 0;
}