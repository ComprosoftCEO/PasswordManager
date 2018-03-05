#include <command.h>
#include <stdio.h>

int param_test(size_t expected, size_t given, int atLeast) {

    const char* cnt;
    if (expected != 1) {cnt = "parameters";}
    else {cnt = "parameter";}

    if (atLeast == 0) {
        if (given == expected) {return 1;}
        printf("-- Expecting exactly %u %s, %u given. --\n",
               (unsigned int) expected, cnt, (unsigned int) given);
    } else {
        if (given >= expected) {return 1;}
        printf("-- Expecting at least %u %s, %u given. --\n",
               (unsigned int) expected, cnt, (unsigned int) given);
    }

    return 0;
}
