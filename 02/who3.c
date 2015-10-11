/*who1.c - a first version of the who program
 * open, read UTMP file, and show results
 */
#include<stdio.h>
#include<utmp.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>

#define SHOWHOST
void show_info(struct utmp*);
void show_time(int32_t);

extern int utmp_open(char* filename);
extern struct utmp* utmp_next();
extern void utmp_close();

int main(void){
	struct utmp* utbufp;

	if(utmp_open(UTMP_FILE)==-1){
		perror(UTMP_FILE);
		exit(1);
	}
	while((utbufp=utmp_next())!=NULL)
		show_info(utbufp);
	utmp_close();
	return 0;

}


void show_info(struct utmp* utbufp){
	if(utbufp->ut_type!=USER_PROCESS)
		return;
	printf("%-8.8s",utbufp->ut_user);
	printf(" ");
	printf("%-12.8s",utbufp->ut_line);
	printf(" ");
	show_time(utbufp->ut_time);
	printf(" ");
#ifdef SHOWHOST
	printf("(%s)",utbufp->ut_host);
#endif
	printf("\n");
}

void show_time(int32_t time){
	time_t timeval=(time_t)time;
	struct tm* tmvalp=localtime(&timeval);
	printf("%d-%d-%d %d:%02d",tmvalp->tm_year+1900,tmvalp->tm_mon+1,tmvalp->tm_mday,tmvalp->tm_hour,tmvalp->tm_min);
}
