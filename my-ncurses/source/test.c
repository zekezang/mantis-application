#include "ncurses/ncurses.h"
#include "ncurses/curses.h"

WINDOW *my_win;
static int winy = 40,winx = 60;

int main(int argc, char **argv) {
	my_win = initscr();
    cbreak();
    //noecho(); 不关闭回显
//    getmaxyx(my_win, winy, winx);
    box(my_win, winx, winy);

    move(winy*0.3,winx*0.4);
    printw("d");
    move(12, 2);
//    go_write(winy-10,winx-10,"dsd");




    refresh();
    getch();

	endwin();

	return 0;
}
