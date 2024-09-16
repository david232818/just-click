#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/random.h>
#include <ncurses.h>
#include "ncurses_utils.h"
#include "utils.h"

#define BOX_INTERVAL 2		/* unit: second */
#define BOX_HEIGHT 3
#define BOX_WIDTH 3
#define BOX_SCORE 10

static unsigned int set_coor(unsigned int maxval);
static unsigned int  get_score(MEVENT *mevt, int c, int y, int x);

int main()
{
    int c;
    unsigned int y, x;
    unsigned int cnt, score, prevscore;
    time_t chkpt;
    WINDOW *win;
    MEVENT mevt;
    
    initscr();
    start_color();
    init_pair(1, COLOR_RED, 0);
    cbreak();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    mouseinterval(0);
    printf("\033[?1002h\n");
    refresh();

    y = set_coor(LINES - BOX_HEIGHT - 1);
    x = set_coor(COLS - BOX_WIDTH - 1);
    
    win = create_box(BOX_HEIGHT, BOX_WIDTH, y, x);
    if (win == NULL) {
	nocbreak();
	endwin();
	return -1;
    }

    score = 0;
    prevscore = score;
    PRINTSF_AT(MK_STYLE(1, A_ITALIC), 0, COLS - 24, "score: %d", score);

    cnt = 0;
    chkpt = time(NULL) + BOX_INTERVAL;
    while (cnt < 10) {
	c = getch();
	if (c != ERR && prevscore == score) {
	    score += get_score(&mevt, c, y, x);
	    PRINTSF_AT(MK_STYLE(1, A_ITALIC), 0, COLS - 24, "score: %d", score);
	}

	if (chkpt == time(NULL)) {
	    prevscore = score;

	    y = set_coor(LINES - BOX_HEIGHT - 1);
	    x = set_coor(COLS - BOX_WIDTH - 1);

	    destroy_win(win);
	    win = create_box(BOX_HEIGHT, BOX_WIDTH, y, x);
	    if (win == NULL) {
		nocbreak();
		endwin();
		return -1;
	    }
	    chkpt = time(NULL) + BOX_INTERVAL;
	    cnt++;
	}
    }

    PRINTSS_AT(NO_STYLE, LINES / 2, COLS / 2, "Press any key to exit..");
    PRESS_ANY_KEY_TO_EXIT_NODELAY(c);
    destroy_win(win);
    endwin();
    return 0;
}

static unsigned int set_coor(unsigned int maxval)
{
    unsigned int randbytes;

    if (getrandom(&randbytes, sizeof(randbytes), 0) == -1) {
	srand(time(NULL));
	randbytes = rand();
    }
    return randbytes % maxval;
}

static unsigned int get_score(MEVENT *mevt, int c, int y, int x)
{
    if (is_left_button_clicked(mevt, c) == LEFT_BUTTON_CLICKED)
	if (IS_IN_DOMAIN(mevt->y, y, y + BOX_HEIGHT) &&
	    IS_IN_DOMAIN(mevt->x, x, x + BOX_WIDTH))
	    return BOX_SCORE;
    return 0;
}
