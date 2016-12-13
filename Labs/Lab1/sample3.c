// CIS 452 - Lab 1
// Author: Hayden Miedema
// Title: sample3.c


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 16

int main () {

    char *data1, *data2, *tmp;
    int k;
    data1 = malloc (SIZE);
    data2 = malloc (SIZE);

    tmp = data1;
    printf ("Please input your username: ");
    scanf ("%s", data1);
    for (k = 0; k < SIZE; k++)
        data2[k] = *(tmp++);
    
    printf ("data2 :%s:\n", data2);
    tmp = data1; // tmp is pointing at a block that is outside of the allocated range, therefore it must be set back to data1
    printf ("tmp :%s:\n", tmp);
    free (data1); // the freeing of data1 must be moved to after the print of tmp, since tmp is pointing to data1
    free (data2);
    return 0;
}
