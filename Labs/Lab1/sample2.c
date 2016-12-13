//CIS 452
//Author: Hayden Miedema
//Title: sample2.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mcheck.h>


#define SIZE 16

int main()
{
    mtrace();
    char *data1, *data2;
    int k;
    do {
        data1 = malloc(SIZE);
        printf ("Please input your EOS username: ");
        scanf ("%s", data1);
        if (! strcmp (data1, "quit")){
	    free(data1); // We must free data1 because it is reallocated every time inside the loop
            break;
	}
        data2 = malloc(SIZE);
        for (k = 0; k < SIZE; k++)
            data2[k] = data1[k];
        free (data1);
        printf ("data2 :%s:\n", data2);
	free (data2); //We must free data2, just like data 1 is freed before
		     // the print statement. This is because data2 is allocated
		     // above with the malloc statement, but never freed.
    } while(1);

    muntrace();
    return 0;
}

