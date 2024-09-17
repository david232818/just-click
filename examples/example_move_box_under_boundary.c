#include <ncurses.h>
#include "ncurses_utils.h"

/* This example shows how to limit a movement of a box in the screen. */

int main()
{
    int c;
    int height, width, y, x;
    WINDOW *win;
    
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    refresh();

    height = 3;
    width = 10;
    y = (LINES - height) / 2;
    x = (COLS - width) / 2;
    win = create_box(height, width, y, x);
    PRINTSS_AT(A_ITALIC, LINES - 2, COLS - 24, "Press F2 to exit");
    
    while ((c = getch()) != KEY_F(2)) {
	switch (c) {
	case KEY_LEFT:
	    destroy_win(win);
	    OOS_COOR_DO_NOT_MOVE(width, 1, X, x, 1);
	    win = create_box(height, width, y, --x);
	    break;
	case KEY_RIGHT:
	    destroy_win(win);
	    OOS_COOR_DO_NOT_MOVE(width, 1, X, x, 1);
	    win = create_box(height, width, y, ++x);
	    break;
	case KEY_UP:
	    destroy_win(win);
	    OOS_COOR_DO_NOT_MOVE(height, 1, Y, y, 1);
	    win = create_box(height, width, --y, x);
	    break;
	case KEY_DOWN:
	    destroy_win(win);
	    OOS_COOR_DO_NOT_MOVE(height, 1, Y, y, 1);
	    win = create_box(height, width, ++y, x);
	    break;
	default:
	    break;
	}
	PRINTSF_AT(A_BOLD, LINES - 1, COLS - 24, "(%d, %d), (%d, %d)", y, x, \
		   height, width);
    }
    endwin();
    return 0;
}
