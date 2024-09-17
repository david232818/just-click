#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/random.h>
#include <ncurses.h>
#include "ncurses_utils.h"
#include "utils.h"

#define SCORE_LOC_Y (0)
#define SCORE_LOC_X (COLS - 24)

#define EXIT_MSG_LOC_Y (LINES / 2)
#define EXIT_MSG_LOC_X (COLS / 2 - 16)

#define BOX_INTERVAL (2)		/* unit: second */
#define BOX_HEIGHT (3)
#define BOX_WIDTH (3)
#define BOX_SCORE (10)

#define IS_READY_TO_SCORE(prev, curr) ((prev) == (curr))
#define IS_TIME_FOR_NEWBOX(t, curr) ((t) == (curr))

static unsigned int set_coor(unsigned int maxval);
static unsigned int  get_score(int c, int y, int x);

int main()
{
    int c;
    unsigned int boxcnt, boxloc_y, boxloc_x;
    unsigned int score, prevscore;
    time_t newbox_time;
    WINDOW *win;
    
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

    boxloc_y = set_coor(LINES - BOX_HEIGHT - 1);
    boxloc_x = set_coor(COLS - BOX_WIDTH - 1);
    
    win = create_box(BOX_HEIGHT, BOX_WIDTH, boxloc_y, boxloc_x);
    if (win == NULL) {
	nocbreak();
	endwin();
	return -1;
    }

    score = 0;
    prevscore = score;
    PRINTSF_AT(MK_STYLE(1, A_ITALIC), SCORE_LOC_Y, SCORE_LOC_X, \
	       "score: %d", score);

    boxcnt = 0;
    newbox_time = time(NULL) + BOX_INTERVAL;
    while (boxcnt < 10) {
	c = getch();
	if (c != ERR && IS_READY_TO_SCORE(prevscore, score)) {
	    score += get_score(c, boxloc_y, boxloc_x);
	    PRINTSF_AT(MK_STYLE(1, A_ITALIC), SCORE_LOC_Y, SCORE_LOC_X, \
		       "score: %d", score);
	}

	if (IS_TIME_FOR_NEWBOX(newbox_time, time(NULL))) {
	    prevscore = score;

	    boxloc_y = set_coor(LINES - BOX_HEIGHT - 1);
	    boxloc_x = set_coor(COLS - BOX_WIDTH - 1);

	    destroy_win(win);
	    win = create_box(BOX_HEIGHT, BOX_WIDTH, boxloc_y, boxloc_x);
	    if (!win) {
		nocbreak();
		endwin();
		return -1;
	    }
	    newbox_time = time(NULL) + BOX_INTERVAL;
	    boxcnt++;
	}
    }

    PRINTSS_AT(NO_STYLE, EXIT_MSG_LOC_Y, EXIT_MSG_LOC_X, \
	       "Press any key to exit..");
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

static unsigned int get_score(int c, int y, int x)
{
    MEVENT mevt;
    
    if (is_left_button_clicked(&mevt, c) == LEFT_BUTTON_CLICKED)
	if (IS_IN_DOMAIN(mevt.y, y, y + BOX_HEIGHT) &&
	    IS_IN_DOMAIN(mevt.x, x, x + BOX_WIDTH))
	    return BOX_SCORE;
    return 0;
}
