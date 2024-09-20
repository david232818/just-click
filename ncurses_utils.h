#ifndef __NCURSES_UTILS_H__
#define __NCURSES_UTILS_H__

#include <stdint.h>
#include <ncurses.h>

/* Make style for PRINTSF_AT macro function */
#define MK_STYLE(color, typo) (COLOR_PAIR(color) | (typo))

#define NO_STYLE (0)

/* Print styled format string at specified location */
#define PRINTSF_AT(style, y, x, fmt, ...)	\
    do {					\
	attron((style));			\
	mvprintw((y), (x), (fmt), __VA_ARGS__);	\
	attroff((style));			\
    } while (0)

/*
 * PRINTSF_AT() does not work when __VA_ARGS__ has nothing. But ##__VA_ARGS__
 * seems to be not portable. So we define PRINTSS_AT for this.
 */
#define PRINTSS_AT(style, y, x, s)			\
    do {						\
	attron((style));				\
	mvprintw((y), (x), (s));			\
	attroff((style));				\
    } while (0)

/* Check whether object location is out of screen (OOS) */

#define IS_OOS_Y_UPPER(h, y) ((y) >= LINES - (h) - 1)
#define IS_OOS_Y_LOWER(min, y) ((y) <= (min))
#define IS_OOS_Y(h, min, y) \
    (IS_OOS_Y_UPPER((h), (y)) || IS_OOS_Y_LOWER((min), (y))

#define IS_OOS_X_UPPER(w, x) ((x) >= COLS - (w) - 1)
#define IS_OOS_X_LOWER(min, x) ((x) <= (min))
#define IS_OOS_X(w, min, x) \
    (IS_OOS_X_UPPER((w), (x)) || IS_OOS_X_LOWER((min), (x)))

#define IS_OOS(h, w, y, x) (IS_OOS_Y((h), 1, (y)) || IS_OOS_X((w), 1, (x)))

#define OOS_COOR_DO_NOT_MOVE(side, min, coorcap, coor, mv)		\
    do {								\
	if (BUILD_MACRONAME3(IS_OOS, coorcap, LOWER)((min), (coor) - (mv))) { \
	    (coor) += (mv);						\
	} else if (BUILD_MACRONAME3(IS_OOS, coorcap, UPPER)((side),	\
							    (coor) - (mv))) { \
	    (coor) -= (mv);						\
	}								\
    } while (0)

#define IS_LEFT_BUTTON_PRESSED(bstate) ((bstate) & BUTTON1_PRESSED)

enum button_click_flags {
    NOT_CLICKED = 0,
    LEFT_BUTTON_CLICKED = 1,
    RIGHT_BUTTON_CLICKED
};

#define PRESS_ANY_KEY_TO_EXIT_NODELAY(c)	\
    do {					\
	;					\
    } while (((c) = getch()) == ERR)

WINDOW *create_box(int h, int w, int sy, int sx);
void destroy_win(WINDOW *win);

/*
 * If you clicked left button of your mouse, it implies that the mouse is
 * input.
 */
mmask_t is_left_button_clicked(MEVENT *mevt, int c);

#endif
