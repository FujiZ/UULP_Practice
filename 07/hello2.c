#include<stdio.h>
#include<curses.h>
#include<unistd.h>

int main(void){
	int i;
	initscr();

	clear();
	for(i=0;i<LINES;++i){
		move(i,i+1);
		if(i%2)
			standout();
		addstr("Hello,world!");
		if(i%2)
			standend();
		sleep(1);
		refresh();
	}
	endwin();
	return 0;
}
