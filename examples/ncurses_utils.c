#include <ncurses.h>
#include "ncurses_utils.h"

int is_left_button_clicked(MEVENT *mevt, int c)
{
    int res;

    res = -1;
    if (mevt == NULL)
	return res;

    res = NOT_CLICKED;
    switch (c) {
    case KEY_MOUSE:
	if (getmouse(mevt) == OK)
	    if (IS_LEFT_BUTTON_PRESSED(mevt->bstate))
		res = LEFT_BUTTON_CLICKED;
	break;
    default:
	break;
    }
    return res;
}

/* create_box: create new window that displays a box */
WINDOW *create_box(int h, int w, int sy, int sx)
{
    WINDOW *local_win;

    local_win = newwin(h, w, sy, sx);
    box(local_win, 0, 0);
    wrefresh(local_win);
    return local_win;
}

/* destroy_win: destory window */
void destroy_win(WINDOW *win)
{
    if (win == NULL)
	return ;
    
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(win);
    delwin(win);
}
