/*6.12*/
#include<unistd.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<stdio.h>
#include<signal.h>
#include<termios.h>
#include<stdlib.h>

char c=0;

struct winsize wbuf;

void tty_mode(int how){
	static struct termios original_mode;
	if(how==0){
		tcgetattr(0,&original_mode);
	}
	else{
		tcsetattr(0,TCSANOW,&original_mode);
	}
}
void set_cr_noecho_mode(){
	struct termios ttystate;
	tcgetattr(0,&ttystate);
	ttystate.c_lflag&=~ICANON;
	ttystate.c_lflag&=~ECHO;
	ttystate.c_cc[VMIN]=1;
	tcsetattr(0,TCSANOW,&ttystate);
}

void printScreen(int row,int col){
	int i,j;
	for(i=0;i<row;++i){
		for(j=0;j<col;++j)
			putchar('A'+c);
		putchar('\n');
	}
}
void win_handler(int signum){
	if(ioctl(0,TIOCGWINSZ,&wbuf)==-1)
		exit(1);
	c++;
	printScreen(wbuf.ws_row,wbuf.ws_col);
}

int main(void){
	char ch;
	if(ioctl(0,TIOCGWINSZ,&wbuf)==-1)
		return 1;
	tty_mode(0);
	set_cr_noecho_mode();
	printScreen(wbuf.ws_row,wbuf.ws_col);
	signal(SIGWINCH,win_handler);
	while((ch=getchar())!=EOF){
		switch(ch){
			case 'Q':
				tty_mode(1);
				return 0;
			default:
				c=0;
				printScreen(wbuf.ws_row,wbuf.ws_col);
		}
	}
	return 0;
}
