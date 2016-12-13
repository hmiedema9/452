#include <stdio.h>
#include <stdlib.h>

int main (int argc, char * argv[]) {
    char * name;
    char command[80];

    name = argv[1];
    sprintf (command, "groups %s", name);
    system (command);
    return 0;
}