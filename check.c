#include <unistd.h>
#include <fcntl.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

struct u_info{
	int uid;
  char UserName[1024];
	char Password[1024];
	char type;
	int count;
	int valid;
}db;

struct t_info{
		char train_no[1024];
		char source[1024];
		char destination[1024];
		int d_seats[7];

}tr;
struct ticket_info{
    int ticket_no;
}ti;

typedef struct u_info UINFO;
typedef struct t_info TINFO;
typedef struct ticket_info TICKET;
typedef struct ut_info UTINFO;
int main(void){

	int fd;
	fd = open("USERINFO",O_CREAT|O_RDWR,0777);
    db.uid=1;
    strcpy(db.UserName,"admin");
    strcpy(db.Password,"admin");
    db.type='C';
    db.count=0;


    write(fd,&db,sizeof(db));
    db.uid=2;
    strcpy(db.UserName,"ankita");
    strcpy(db.Password,"ankita");
    db.type='U';
    db.count=0;

    write(fd,&db,sizeof(db));
    db.uid=3;
    strcpy(db.UserName,"shashank");
    strcpy(db.Password,"shashank");
    db.type='A';
    db.count=0;

    write(fd,&db,sizeof(db));
  	close(fd);


    int fd2;
		fd2=open("TRAININFO",O_CREAT|O_RDWR,0744);
	  strcpy(tr.train_no,"100");
		strcpy(tr.source,"haldwani");
		strcpy(tr.destination,"bangalore");
		for(int i=0;i<7;i++){
			 tr.d_seats[i]=50;
		}
    write(fd2,&tr,sizeof(tr));
		strcpy(tr.train_no,"101");
		strcpy(tr.source,"dehradun");
		strcpy(tr.destination,"bangalore");
		for(int i=0;i<7;i++){
			 tr.d_seats[i]=50;
		}
    write(fd2,&tr,sizeof(tr));
    close(fd2);
  int fd3;
	fd3=open("TICKETNO",O_CREAT|O_RDWR,0777);
	ti.ticket_no=0;
  while(read(fd3,&ti,sizeof(ti))){
		printf("\tTicket Number");
		printf("%d\n",ti.ticket_no);
}
write(fd3,&ti,sizeof(ti));
close(fd3);


	return 0;
}
