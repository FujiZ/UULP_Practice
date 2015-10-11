/* play_again1.c
 * purpose: ask if user wants another transcation
 * method: set tty into char-by-char mode,read char,retyrb result
 * returns: 0->yes,1->no
 * better: do no echo inappropriate input
 */
#include<stdio.h>
#include<stdlib.h>
#include<termios.h>

#define QUESTION  "Do you want play again?"
int get_response(char* question);
void set_crmode();
void tty_mode(int how);

int main(void){
	int response;
	tty_mode(0);
	set_crmode();
	response =get_response(QUESTION);
	tty_mode(1);
	return response;
}

int get_response(char* question){
	char input;
	printf("%s (y/n)?",question);
	while(1){
		switch(input=getchar()){
			case 'y':
			case 'Y':
				return 0;
			case 'n':
			case 'N':
			case EOF:
				return 1;
			default:
				printf("\ncant understand %c\n",input);
		}
	}
}

void set_crmode(){
	struct termios ttystate;
	tcgetattr(0,&ttystate);
	ttystate.c_lflag&=~ICANON;
	ttystate.c_cc[VMIN]=1;
	tcsetattr(0,TCSANOW,&ttystate);
}

void tty_mode(int how){
	static struct termios original_mode;
	if(how==0)
		tcgetattr(0,&original_mode);
	else
		tcsetattr(0,TCSANOW,&original_mode);
}
