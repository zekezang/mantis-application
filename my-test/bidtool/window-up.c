#include <unistd.h>
#include <stdlib.h>
#include <curses.h>
#include <stdio.h>
#include <sys/ioctl.h>

int main() {
	struct _win_st *popup_window_ptr;

	struct winsize size;

	if (ioctl(0, TIOCGWINSZ, &size) == -1) {
		exit(EXIT_SUCCESS);
	}

	initscr();

	move(5, 5);
//	printw("%s", "About to begin");
//	refresh();
//	size.ws_row, size.ws_col

	popup_window_ptr = newwin(15, 80, (size.ws_row - 15) / 2, (size.ws_col - 80) / 2);
	box(popup_window_ptr, '|', '-');
	//用|-包围创建一个框
	mvwprintw(popup_window_ptr, 1, 2, "%s", "Pop up window!");
	mvwprintw(popup_window_ptr, 2, 2, "%s", "11111111111111111111!");

	mvwprintw(popup_window_ptr, 3, 2, "%s", "2222222222222222dddddddddddddddddd222!");

	wrefresh(popup_window_ptr);
	sleep(200);

	beep();
	delwin(popup_window_ptr);

	endwin();
	exit(EXIT_SUCCESS);
}
