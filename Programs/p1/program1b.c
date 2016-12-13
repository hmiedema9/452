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

	/* Variable for the main window */
    WINDOW * mainWin;
    
	/*  Initialize ncurses  */
    if ( (mainWin = initscr()) == NULL ) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }

	/* Variables for the cursor, terminal rows and columns, and input */
    int rowNum=0, numCol=0;
    int cursorPos = 15;
    char plane[100];
    char input[100];
 
    for(int i = 0; i < numCol; i++)
        mvprintw((rowNum-5), i, "+");

    while(1)
    {
		getmaxyx(stdscr, rowNum, numCol); 
		sprintf(plane, "Columns: %d Rows: %d", numCol, rowNum);
		mvprintw(0, 0, plane);					/* All of this inside the for loop */
        mvprintw(LINES - 2, 0, "Items entered: ");		/* Allows dynamic sizing for the screen */
        mvprintw((rowNum/2), (numCol-(numCol/2)-16), "%s", "Enter an item (q to quit): ");
        getstr(input);
        if(!strcmp(input, "q"))
                break;
        mvprintw(LINES - 2, 0, "Items entered: ");
        mvprintw(LINES - 2, cursorPos, "%s, ",input );
        cursorPos += strlen(input)+2;
    }
    
    getch();
    delwin(mainWin);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}
