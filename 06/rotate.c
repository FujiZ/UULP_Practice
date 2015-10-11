#include<stdio.h>
#include<ctype.h>
#include<fcntl.h>
#include<termios.h>

void tty_mode(int how){
	static struct termios original_mode;
	static int original_flags;
	if(how==0){
		tcgetattr(0,&original_mode);
		original_flags=fcntl(0,F_GETFL);
	}
	else{
		tcsetattr(0,TCSANOW,&original_mode);
		fcntl(0,F_SETFL,original_flags);
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
int main(void){
	int c;
	tty_mode(0);
	set_cr_noecho_mode();
	while((c=getchar())!=EOF){
		if(c=='z'){
			putchar('a');
		}
		else if(c>='a'&&c<='y')
			putchar(++c);
		else if(c=='\b'){
			printf("\b \b");
			fflush(stdout);
		}
		else if(c=='Q')
			break;
	}
	tty_mode(1);
	return 0;
}
