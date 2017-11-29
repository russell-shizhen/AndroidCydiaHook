
#include <string.h>
#include "hook_example.h"

const int testPassword(const char *myString)
{
    const char *mySecret = "my_secret_pass";
    int cmpResult = strncmp(myString, mySecret, strlen(mySecret));

    return cmpResult;
}

