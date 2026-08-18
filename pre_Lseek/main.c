#include <stdio.h>
#include "functions.h"
#include "helpers.h"

void printMessage(const char *message)
{
    printf("%s\n", message);
}

int main()
{
    printMessage("Program started");

    function1();

    return 0;
}