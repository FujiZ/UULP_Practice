/* utmplib.c - function to buffer reads from utmp file
 */
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<utmp.h>

#define NRECS 16
#define NULLUT ((struct utmp*)NULL)

static struct utmp utmpbuf[NRECS];
static int num_recs;
static int cur_rec;
static int fd_utmp=-1;

void utmp_close();
int utmp_reload();

int utmp_open(char* filename){
	fd_utmp=open(filename,O_RDONLY);
	cur_rec=num_recs=0;
	return fd_utmp;
}

struct utmp* utmp_next(){
	struct utmp* recp;
	if(fd_utmp==-1)
		return NULLUT;
	if(cur_rec==num_recs&&utmp_reload()==0)
		return NULLUT;
	recp=&utmpbuf[cur_rec++];
	return recp;
}

int utmp_reload(){
	int amt_read;
	amt_read=read(fd_utmp,utmpbuf,NRECS*sizeof(struct utmp));
	num_recs=amt_read/sizeof(struct utmp);
	cur_rec=0;
	return num_recs;
}

void utmp_close(){
	if(fd_utmp!=-1){
		close(fd_utmp);
		fd_utmp=-1;
	}
}
