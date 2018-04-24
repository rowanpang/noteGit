#include <ncurses.h>
#include <string.h>
#include <locale.h>
#include <panel.h>

int main()
{
	char *msg="Just a test";
	char in=0;
	int scr_Y,scr_X;

	setlocale(LC_ALL,"");
	initscr();
	start_color();
	noecho();
	nonl();
	cbreak();
	/*curs_set(0);*/
	scrollok(stdscr,0);

	char sp_buf[10];
	sprintf(sp_buf,"%%%dc",COLS);

	init_pair(14, COLOR_YELLOW, COLOR_BLUE);
	attrset(COLOR_PAIR(14) | A_BOLD);

	move(0,0);
	printw(sp_buf,' ');
	move(0,1);
	mvprintw(0, 1, "IPTraf");

	move(LINES-1,0);
	printw(sp_buf,' ');
	mvprintw(LINES - 1, 1, "Linux/x86_64");

	init_pair(1, COLOR_BLUE, COLOR_WHITE); 
	attrset(COLOR_PAIR(1));

	int row;
	for(row=1;row<=LINES-2;row++){
		move(row,0);
		printw(sp_buf,' ');
	}



	/*WINDOW *win;*/
	/*PANEL *panel;*/
	/*int ch;*/
	/*win = newwin(18, 62, (LINES - 17) / 2, (COLS - 62) / 2);                                       */
	/*panel = new_panel(win);*/

	/*meta(win, TRUE);*/
		/*keypad(win, TRUE);*/
		/*notimeout(win, 0);*/
		/*scrollok(win, 1);*/

	/*wtimeout(win, -1);*/
		/*[>wattrset(win, BOXATTR);<]*/
	
	getmaxyx(stdscr,scr_Y,scr_X);
	mvprintw(scr_Y/2,(scr_X-strlen(msg))/2,"%s",msg);
	mvprintw(scr_Y/2+1,0,"this screen has %d rows and %d columns",scr_Y,scr_X);
	/*printw("Hello World\n");*/
	/*refresh();*/

	while(in=getch()){
		if(in=='q'){
			break;
		}
		printw("%c",in);
	}
	endwin();
	return 0;
};
