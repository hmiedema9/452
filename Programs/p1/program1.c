/*
 * CIS 452 - Program 1
 * By: Hayden Miedema
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <ncurses.h>
#include <term.h>
#include <string.h>



int main(void) {

    WINDOW * mainwin;


    /*  Initialize ncurses  */

    if ( (mainwin = initscr()) == NULL ) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }

    int numRow=0, numCol=0;
    int pos = 15;
    char grid[100];
    char input[100];
 
    for(int i = 0; i < numCol; i++)
        mvprintw((numRow-5), i, "-");

    while(1)
    {

	getmaxyx(stdscr, numRow, numCol); 
	sprintf(grid, "Columns: %d Rows: %d", numCol, numRow);
	mvprintw(0, 0, grid);					/* All of this inside the for loop */
        mvprintw(LINES - 2, 0, "Items entered: ");		/* Allows dynamic sizing for the screen */
        mvprintw((numRow/2), (numCol-(numCol/2)-16), "%s", "Enter an item: ");
        getstr(input);
        if(!strcmp(input, "q"))
                break;
        mvprintw(LINES - 2, 0, "Items entered: ");
        mvprintw(LINES - 2, pos, "%s, ",input );
        pos += strlen(input)+2;\
    }
    
    getch();
    delwin(mainwin);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}
