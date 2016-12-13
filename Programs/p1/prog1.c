#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <term.h>
#include <stdlib.h>

int main()
{
	int nrows, ncolumns;
	char msg[50000];	
	
	setupterm((char*) 0, 1, (int*) 0);
	nrows = tigetnum("lines");
	ncolumns = tigetnum("cols");
	printf("This terminal has %d columns and %d rows\n", ncolumns, nrows);
	

	printf("Please enter your name and age: ");
	scanf("%s", msg);
	printf("You entered: %s\n", msg);
	exit(0);

}
