#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/random.h>
#include <ncurses.h>
#include "ncurses_utils.h"
#include "utils.h"

#define SCREEN_LINES_MIN (25)
#define SCREEN_COLS_MIN (80)

#define IS_SCREEN_TOO_SMALL(lines, minlines, cols, mincols) \
    (((lines) <= (minlines)) && ((cols) <= (mincols)))

#define SCORE_PER_BOX (10)
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

static unsigned int get_rand_under(unsigned int maxval);
static unsigned int getscore(int c, int y, int x);

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

    if (IS_SCREEN_TOO_SMALL(LINES, SCREEN_LINES_MIN, COLS, SCREEN_COLS_MIN)) {
	PRINTSS_AT(MK_STYLE(1, A_ITALIC), EXIT_MSG_LOC_Y, EXIT_MSG_LOC_X, \
		   "Screen Too Small!!");
	PRESS_ANY_KEY_TO_EXIT_NODELAY(c);
	goto OUT;
    }

    boxloc_y = get_rand_under(LINES - BOX_HEIGHT - 1);
    boxloc_x = get_rand_under(COLS - BOX_WIDTH - 1);
    win = create_box(BOX_HEIGHT, BOX_WIDTH, boxloc_y, boxloc_x);
    if (!win)
	goto FAIL;

    score = 0;
    prevscore = score;
    PRINTSF_AT(MK_STYLE(1, A_ITALIC), SCORE_LOC_Y, SCORE_LOC_X, \
	       "score: %d", score);

    boxcnt = 0;
    newbox_time = time(NULL) + BOX_INTERVAL;
    while (boxcnt < 10) {
	c = getch();
	if (c != ERR && IS_READY_TO_SCORE(prevscore, score)) {
	    score += getscore(c, boxloc_y, boxloc_x);
	    PRINTSF_AT(MK_STYLE(1, A_ITALIC), SCORE_LOC_Y, SCORE_LOC_X, \
		       "score: %d", score);
	}

	if (IS_TIME_FOR_NEWBOX(newbox_time, time(NULL))) {
	    prevscore = score;

	    boxloc_y = get_rand_under(LINES - BOX_HEIGHT - 1);
	    boxloc_x = get_rand_under(COLS - BOX_WIDTH - 1);

	    destroy_win(win);
	    win = create_box(BOX_HEIGHT, BOX_WIDTH, boxloc_y, boxloc_x);
	    if (!win)
		goto FAIL;

	    newbox_time = time(NULL) + BOX_INTERVAL;
	    boxcnt++;
	}
    }

    PRINTSS_AT(NO_STYLE, EXIT_MSG_LOC_Y, EXIT_MSG_LOC_X, \
	       "Press any key to exit..");
    PRESS_ANY_KEY_TO_EXIT_NODELAY(c);

    destroy_win(win);

OUT:
    nocbreak();
    endwin();
    return 0;

FAIL:
    nocbreak();
    endwin();
    return -1;
}

/* get_rand_under: get random number under maxval */
static unsigned int get_rand_under(unsigned int maxval)
{
    unsigned int randbytes;

    if (getrandom(&randbytes, sizeof(randbytes), 0) == -1) {
	srand(time(NULL));
	randbytes = rand();
    }
    return randbytes % maxval;
}

static unsigned int getscore(int c, int y, int x)
{
    MEVENT mevt;

    if (is_left_button_clicked(&mevt, c) == BUTTON1_PRESSED)
	if (IS_IN_DOMAIN(mevt.y, y, y + BOX_HEIGHT) &&
	    IS_IN_DOMAIN(mevt.x, x, x + BOX_WIDTH))
	    return SCORE_PER_BOX;
    return 0;
}
