#include<stdio.h>
#include<curses.h>
#include<unistd.h>

int main(void){
	int i;
	initscr();

	clear();
	for(i=0;i<LINES;++i){
		move(i,i+i);
		if(i%2)
			standout();
		addstr("Hello,world!");
		if(i%2)
			standend();
		refresh();
		sleep(1);
		move(i,i+i);
		addstr("            ");
	}
	endwin();
	return 0;
}
