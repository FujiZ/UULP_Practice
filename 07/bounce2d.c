/* bounce2d 1.0
 * bounce a character(default is 'o')around the screen
 * defined by some parameters
 *
 * userinput:	s slow down x component,S:slow y component
 *				f speed up x component,F:speed up y component
 *				Q quit
 *				-,+ move the board
 * blocks on read, but timer tick sends SIGALRM caught by ball_move
 * build: cc bounce2d.c set_ticker.c -l curses -o bounce2d
 */
#include<curses.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include"bounce.h"

struct ppball the_ball;
struct board the_board;
int score;

void set_up();
void wrap_up();
int bounce_or_lose(struct ppball *bp);
void ball_move(int);
int set_ticker(int);
void boardUp();
void boardDown();

void gameover();
void addScore();


int main(void){
	char c;
	
	set_up();

	while((c=getchar())!='Q'){
		switch(c){
			case 'f':
				the_ball.x_ttm--;break;
			case 's':
				the_ball.x_ttm++;break;
			case 'F':
				the_ball.y_ttm--;break;
			case 'S':
				the_ball.y_ttm++;break;
			case '-':
				boardUp();break;
			case '+':
				boardDown();break;
			default:
				break;
		}
	}
	gameover();
	wrap_up();

}

void set_up(){
	int i;
	the_ball.y_pos=Y_INIT;
	the_ball.x_pos=X_INIT;
	the_ball.y_ttg=the_ball.y_ttm=Y_TTM;
	the_ball.x_ttg=the_ball.x_ttm=X_TTM;
	the_ball.y_dir=1;
	the_ball.x_dir=1;
	the_ball.symbol=DFL_SYMBOL;
	
	the_board.x_pos=RIGHT_EDGE+1;
	the_board.upBound=(BOT_ROW-TOP_ROW)/2-2;
	the_board.downBound=(BOT_ROW-TOP_ROW)/2+2;
	the_board.symbol=BOARD;
	
	score=0;

	initscr();
	noecho();
	crmode();

	signal(SIGINT,SIG_IGN);
	//print frame
	for(i=LEFT_EDGE;i<=RIGHT_EDGE;++i){
		mvaddch(TOP_ROW-1,i,'-');
		mvaddch(BOT_ROW+1,i,'-');
	}

	for(i=TOP_ROW-1;i<=BOT_ROW+1;++i){
		mvaddch(i,LEFT_EDGE-1,'|');
	}

	mvaddch(the_ball.y_pos,the_ball.x_pos,the_ball.symbol);

	for(i=the_board.upBound;i<=the_board.downBound;++i)
		mvaddch(i,the_board.x_pos,the_board.symbol);

	refresh();

	signal(SIGALRM,ball_move);
	set_ticker(1000/TICKS_PER_SEC);

}

void wrap_up(){
	set_ticker(0);
	endwin();
}

void ball_move(int signum){
	int y_cur,x_cur,moved;

	int result=1;

	signal(SIGALRM,SIG_IGN);
	y_cur=the_ball.y_pos;
	x_cur=the_ball.x_pos;
	moved=0;
	if(the_ball.y_ttm>0&&the_ball.y_ttg--==1){
		the_ball.y_pos+=the_ball.y_dir;
		the_ball.y_ttg=the_ball.y_ttm;
		moved=1;
	}

	
	if(the_ball.x_ttm>0&&the_ball.x_ttg--==1){
		the_ball.x_pos+=the_ball.x_dir;
		the_ball.x_ttg=the_ball.x_ttm;
		moved=1;
	}
	
	if(moved){
		//mvaddch(y_cur,x_cur,BLANK);
		mvaddch(y_cur,x_cur,BLANK);
		mvaddch(the_ball.y_pos,the_ball.x_pos,the_ball.symbol);
		result=bounce_or_lose(&the_ball);
		if(result==0)
			gameover();
		else
			addScore();
		move(LINES-1,COLS-1);
		refresh();
	}

	signal(SIGALRM,ball_move);

}

int bounce_or_lose(struct ppball *bp){
	int return_val=1;

	if(bp->y_pos==TOP_ROW){
		bp->y_dir=1;
		return_val=1;
	}else if(bp->y_pos==BOT_ROW){
		bp->y_dir=-1;
		return_val=1;
	}
	
	if(bp->x_pos==LEFT_EDGE){
		bp->x_dir=1;
		return_val=1;
	}else if(bp->x_pos==RIGHT_EDGE){
		if(bp->y_pos<=the_board.downBound&&bp->y_pos>=the_board.upBound){
			bp->x_dir=-1;
			score++;
			return_val=1;
		}
		else
			return_val=0;
	}

	return return_val;

}
	
void boardUp(){
	if(the_board.upBound>TOP_ROW){
		mvaddch(the_board.downBound,the_board.x_pos,BLANK);
		the_board.upBound--;
		the_board.downBound--;
		mvaddch(the_board.upBound,the_board.x_pos,the_board.symbol);
		move(LINES-1,COLS-1);
		refresh();
	}
}


void boardDown(){
	if(the_board.downBound<BOT_ROW){
		mvaddch(the_board.upBound,the_board.x_pos,BLANK);
		the_board.downBound++;
		the_board.upBound++;
		mvaddch(the_board.downBound,the_board.x_pos,the_board.symbol);
		move(LINES-1,COLS-1);
		refresh();
	}
}

void gameover(){
	mvaddstr((BOT_ROW-TOP_ROW)/2,(RIGHT_EDGE-LEFT_EDGE)/2,"Game over!");
	mvprintw((BOT_ROW-TOP_ROW)/2+1,(RIGHT_EDGE-LEFT_EDGE)/2,"Your score: %d\n",score);
	refresh();
	wrap_up();
	exit(0);
}

void addScore(){
	mvaddstr(TOP_ROW,RIGHT_EDGE+2,"        ");
	mvprintw(TOP_ROW,RIGHT_EDGE+2,"%d",score);
}

