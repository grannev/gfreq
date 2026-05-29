#include "../include/crypto.h"
#include <stdio.h>


int main()
{
    char *msg = "Hello, World!";

    puts(msg);
    encrypt(msg);

    return 0;
}

