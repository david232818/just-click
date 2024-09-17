#include <ncurses.h>
#include "ncurses_utils.h"

int main()
{
    int c;
    int height, width, y, x;
    MEVENT mevt;
    
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    mouseinterval(0);
    printf("\033[?1002h\n");

    PRINTSS_AT(A_ITALIC, LINES - 2, COLS - 24, "Press F2 to exit");
    
    while ((c = getch()) != KEY_F(2)) {
	if (is_left_button_clicked(&mevt, c) == LEFT_BUTTON_CLICKED)
	    PRINTSF_AT(NO_STYLE, LINES - 2, 2, \
		       "left button: (%d, %d)", mevt.x, mevt.y);
    }
    endwin();
    return 0;
}
