#include <stdio.h>
#include <ncurses.h>
#include "ncurses_utils.h"

mmask_t is_left_button_clicked(MEVENT *mevt, int c)
{
    mmask_t res;

    res = ~BUTTON1_PRESSED;
    if (!mevt)
	goto OUT;
    
    switch (c) {
    case KEY_MOUSE:
	if (getmouse(mevt) == OK)
	    res = mevt->bstate & BUTTON1_PRESSED;
	break;
    default:
	break;
    }
OUT:
    return res;
}

WINDOW *create_box(int h, int w, int sy, int sx)
{
    WINDOW *local_win;

    local_win = newwin(h, w, sy, sx);
    if (!local_win)
	return NULL;

    box(local_win, 0, 0);
    wrefresh(local_win);
    return local_win;
}

/* destroy_win: destory window */
void destroy_win(WINDOW *win)
{
    if (!win)
	return ;
    
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(win);
    delwin(win);
}
