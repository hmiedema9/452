// CIS 452 - Lab 1
// Author: Hayden Miedema
// Title: sample4.c
// Error: scanf was reading too many bytes. See the comment below next to scanf

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 16

int main () {
    char *data1;

    data1 = malloc (SIZE);
    printf ("Please input username: ");
    scanf ("%15s", data1);//We must specify the number of how many bytes we allow scanf to read. By putting %15s we are saying read only from bytes 0-15. This is because it is acting as an index.
    printf ("you entered: %s\n", data1); 
    free (data1);

    return 0;
}


