#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <ncurses.h>
#include "ncurses_utils.h"

int64_t is_left_button_clicked(int c)
{
    int64_t res;
    MEVENT mevt = { 0, };

    switch (c) {
    case KEY_MOUSE:
	getmouse(&mevt);
	break;
    default:
	res = NOT_CLICKED;
	break;
    }
    if (IS_LEFT_BUTTON_PRESSED(mevt.bstate)) {
	res = mevt.y;
	res = (res << 32) | mevt.x;
    }
    return res;
}

/* create_box: create new window that displays a box */
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
