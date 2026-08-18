#include "functions.h"
#include "helpers.h"

void function3()
{
    printMessage("Inside Function 3");

    function4();
}

void function4()
{
    printMessage("Inside Function 4");
}