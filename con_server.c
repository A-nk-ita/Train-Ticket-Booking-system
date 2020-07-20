#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#define PORT 4444
char* i2s(int val){			// integer to string conversion
	 static char buff[32]={0};
	 int i=30;
	 for(; val && i;--i,val /= 10)
	 buff[i]="0123456789abcdef"[val % 10];
	 return &buff[i+1];
}
int s2i(char* str)
{
	int l=strlen(str);
	int num=0;
	int flag=0;
	for(int i=0;i<l;i++)
	{
			if(i==0 && !(str[i]>='0' && str[i]<='9')){
				flag=1;
				continue;
			}
			num = num*10 + (str[i]-'0');
	}
	if(flag) num*=-1;
	return num;
}
struct ticket_info{
    int ticket_no;
};
struct ut_info{
	int uid;
	char train_no[1024];
	char source[1024];
	char destination[1024];
	char date[1024];
	int ticket_no;
};
struct u_info{
	int uid;
  char UserName[1024];
	char Password[1024];
	char type;
	int count;
	int valid;

};
struct t_info{
	  char train_no[1024];
		char source[1024];
		char destination[1024];
		int d_seats[7];

};
typedef struct u_info UINFO;
typedef struct t_info TINFO;
typedef struct ut_info UTINFO;
typedef struct ticket_info TICKET;
void User_Menu(int,int);
void Book_Ticket(int newSocket,int uid){
    char source[1024],destination[1024],date[1024],train_no[1024];
		struct flock lock;
		struct flock lock1;
		recv(newSocket,source,1024,0);
		printf("%s",source);
		recv(newSocket,destination,1024,0);

		send(newSocket,"Welcome to uttarakhand",1024,0);
		recv(newSocket,date,1024,0);
	  int fd,flag2=0;
	  TINFO obj;
	  fd=open("TRAININFO",O_RDWR,0777);//train database
		int seat;
	  while(read(fd,&obj,sizeof(obj))){
			if(strcmp(source,obj.source)==0 && strcmp(destination,obj.destination)==0 && obj.d_seats[s2i(date)]>0){
				send(newSocket,obj.train_no,1024,0);
				send(newSocket,obj.source,1024,0);
				send(newSocket,obj.destination,1024,0);
				seat=obj.d_seats[s2i(date)];
				send(newSocket,i2s(seat),1024,0);
			 }
		 }
	  send(newSocket,"Stop",1024,0);
    recv(newSocket,train_no,1024,0);
		lseek(fd,0,SEEK_SET);
		lock1.l_type=F_WRLCK;
		lock1.l_whence=SEEK_CUR;
		lock1.l_start=0;
		lock1.l_len=sizeof(obj);
		while(read(fd,&obj,sizeof(obj))){
			 if(strcmp(train_no,obj.train_no)==0){
				 	lseek(fd,-sizeof(obj),SEEK_CUR);
				 	 if(fcntl(fd,F_SETLK,&lock)==-1)
					 flag2=1;
					 if(obj.d_seats[s2i(date)]>0){
						 obj.d_seats[s2i(date)]--;//ticket generated
	 					 write(fd,&obj,sizeof(obj));

	 				 	break;
					 }
					else{
						send(newSocket,"2",1024,0);
						User_Menu(newSocket,uid);
					}

			 }
	  	}
			lock1.l_type=F_UNLCK;
			fcntl(fd,F_SETLK,&lock1);
		fd=open("USERTRAININFO",O_CREAT|O_RDWR,0777);// usertrain database
		int fd1=open("TICKETNO",O_RDWR,0777);//ticket number database
		TICKET t;
		lock.l_type=F_WRLCK;
		lock.l_whence=SEEK_CUR;
		lock.l_start=0;
		lock.l_len=sizeof(t);
		read(fd1,&t,sizeof(t));
		UTINFO obj1;
		obj1.uid=uid;
		strcpy(obj1.train_no,train_no);
		strcpy(obj1.source,source);
		strcpy(obj1.destination,destination);
		strcpy(obj1.date,date);
		lseek(fd1,0L,SEEK_SET);
		fcntl(fd1,F_SETLK,&lock);
		int z=t.ticket_no++;
		obj1.ticket_no=++z;
		write(fd1,&t,sizeof(t));
		lock.l_type=F_UNLCK;
		fcntl(fd1,F_SETLK,&lock);
		lseek(fd,0L,SEEK_END);
		write(fd,&obj1,sizeof(obj1));// entry in usertrain info table
		close(fd);
		close(fd1);
		if(flag2)
		 send(newSocket,"-1",1024,0);
		 else
		send(newSocket,i2s(t.ticket_no),1024,0);
		User_Menu(newSocket,uid);
}
void Ticket_History(int  newSocket,int uid){
		UTINFO obj;
	  int fd=open("USERTRAININFO",O_RDWR,0777);
		while(read(fd,&obj,sizeof(obj))){
			 if(obj.uid==uid){
				 send(newSocket,i2s(obj.uid),1024,0);
				 send(newSocket,obj.train_no,1024,0);
				 send(newSocket,obj.source,1024,0);
				 send(newSocket,obj.destination,1024,0);
				 send(newSocket,obj.date,1024,0);
				 send(newSocket,i2s(obj.ticket_no),1024,0);
			 }
		}
			send(newSocket,"Stop",1024,0);
	 	  User_Menu(newSocket,uid);
}
void Cancel_Ticket(int newSocket,int uid){
	   UTINFO obj;
		 int flag=0;
	   char ticket_no[1024],date[1024],train_no[1024];
		 struct flock lock;
		 int fd=open("USERTRAININFO",O_RDWR,0777);
		 while(read(fd,&obj,sizeof(obj))){
				if(obj.uid==uid){
					send(newSocket,i2s(obj.uid),1024,0);
					send(newSocket,obj.train_no,1024,0);
					send(newSocket,obj.source,1024,0);
					send(newSocket,obj.destination,1024,0);
					send(newSocket,obj.date,1024,0);
					send(newSocket,i2s(obj.ticket_no),1024,0);
				}
		 }
		 send(newSocket,"Stop",1024,0);
		 recv(newSocket,date,1024,0);
		 recv(newSocket,ticket_no,1024,0);
		 close(fd);
		int fd1=open("USERTRAININFO",O_RDWR,0777);
		 while(read(fd1,&obj,sizeof(obj))){
			  if(obj.uid==uid && (obj.ticket_no==s2i(ticket_no)) && (strcmp(obj.date,date)==0)){

					obj.uid=0;
					strcpy(train_no,obj.train_no);
					strcpy(obj.train_no,"0");
					strcpy(obj.source,"0");
					strcpy(obj.destination,"0");
					strcpy(obj.date,"0");
					obj.ticket_no=0;

					flag=1;
					break;
				}
		 }
		 if(flag){
			 //printf("i am here2\n");
			 lseek(fd1,-sizeof(obj),SEEK_CUR);
			 write(fd1,&obj,sizeof(obj));
		 }
		 int fd2=open("TRAININFO",O_RDWR,0777);
		 TINFO obj1;
		 lock.l_type=F_WRLCK;
		 lock.l_whence=SEEK_CUR;
		 lock.l_start=0;
		 lock.l_len=sizeof(obj1);
		 while(read(fd2,&obj1,sizeof(obj1))){
           if(strcmp(train_no,obj1.train_no)==0){
					  lseek(fd2,-sizeof(obj1),SEEK_CUR);
						 fcntl(fd2,F_SETLKW,&lock);
						 obj1.d_seats[s2i(date)]++;
						 write(fd2,&obj1,sizeof(obj1));
					 }
        }
				lock.l_type=F_UNLCK;
				fcntl(fd2,F_SETLK,&lock);
	close(fd1);
	close(fd2);
//	printf("%d\n",flag);
	send(newSocket,i2s(flag),1024,0);
	User_Menu(newSocket,uid);
}
void Logout(int newSocket,int uid){
	 char msg[1024];
	 recv(newSocket,msg,1024,0);
	 UINFO obj1;
		int fd=open("USERINFO",O_RDWR,0777);
		while(read(fd,&obj1,sizeof(obj1))){
				 if(obj1.uid==uid){
					 printf("hey dosto\n");
					 obj1.count--;
					 lseek(fd,-sizeof(obj1),SEEK_CUR);
					 write(fd,&obj1,sizeof(obj1));
					 break;
				 }
			}
		close(fd);
	 if(strcmp(msg,"Exit")==0)
		 close(newSocket);

}
void User_Menu(int newSocket,int uid){
	 char choice1[1024];
	 recv(newSocket,choice1,1024,0);
	 int choice=atoi(choice1);

	 switch(choice){
		 case 1: Book_Ticket(newSocket,uid);
		         break;
		 case 2: Cancel_Ticket(newSocket,uid);
		 				 break;
		 case 3: Ticket_History(newSocket,uid);
		         break;
		 case 4: Logout(newSocket,uid);
		         break;
		 deafult:printf("\n Wrong choice entered\n ");
		         User_Menu(newSocket,uid);

	 }
}
void Admin_Menu(int,int);
void Add_Train(int newSocket,int uid){
	//	 printf("\nhi\n");
   int flag=0;
	 char train_no[1024],source[1024],destination[1024],seats[1024],msg[1024];
	 recv(newSocket,train_no,1024,0);
	 int fd=open("TRAININFO",O_RDWR,0777);
	 TINFO obj;
	 while(read(fd,&obj,sizeof(obj))){
		  if(strcmp(obj.train_no,train_no)==0){
				flag=1;
				break;
			}
	 }
	 if(flag){
		strcpy(msg,"1");
		send(newSocket,msg,1024,0);
	 }
	 else{
	 strcpy(msg,"0");
	 send(newSocket,msg,1024,0);
	 recv(newSocket,source,1024,0);
	 recv(newSocket,destination,1024,0);
	 recv(newSocket,seats,1024,0);
	 lseek(fd,0L,SEEK_END);
   strcpy(obj.train_no,train_no);
	 strcpy(obj.source,source);
	 strcpy(obj.destination,destination);
	 int k=s2i(seats);
		for(int i=0;i<7;i++)
			obj.d_seats[i]=k;
		write(fd,&obj,sizeof(obj));
		close(fd);
	 }
	 Admin_Menu(newSocket,uid);
}
void Show_Train(int newSocket,int uid){

	 int fd=open("TRAININFO",O_RDONLY,0777);
	 TINFO obj;

	 while(read(fd,&obj,sizeof(obj))){
		 if(strcmp(obj.train_no,"0")==0)continue;
		 send(newSocket,obj.train_no,1024,0);
		 send(newSocket,obj.source,1024,0);
		 send(newSocket,obj.destination,1024,0);
		 int f=obj.d_seats[0];
		 send(newSocket,i2s(f),1024,0);

	 }
	 send(newSocket,"Exit",1024,0);
	 close(fd);
	 Admin_Menu(newSocket,uid);

}
void Delete_Train(int newSocket,int uid){
	int fd=open("TRAININFO",O_RDWR,0777);
	TINFO obj;
	int flag=0;
	struct flock lock;
	  lock.l_type=F_WRLCK;
	  lock.l_whence=SEEK_CUR;
	  lock.l_start=0;
	  lock.l_len=sizeof(obj);
	while(read(fd,&obj,sizeof(obj))){
		if(strcmp(obj.train_no,"0")==0)continue;
		send(newSocket,obj.train_no,1024,0);
		send(newSocket,obj.source,1024,0);
		send(newSocket,obj.destination,1024,0);
		int f=obj.d_seats[0];
		send(newSocket,i2s(f),1024,0);
	}
	char train_no[1024];
	send(newSocket,"Exit",1024,0);
	recv(newSocket,train_no,1024,0);
	lseek(fd,0L,SEEK_SET);
	while(read(fd,&obj,sizeof(obj))){
		 if(strcmp(obj.train_no,train_no)==0){
			  lseek(fd,-sizeof(obj),SEEK_CUR);
			  fcntl(fd,F_SETLKW,&lock);
			  flag=1;
			  strcpy(obj.train_no,"0");
				strcpy(obj.source,"0");
				strcpy(obj.destination,"0");
				for(int i=0;i<7;i++)
				   obj.d_seats[i]=0;
				break;
		 }
	 }
		 if(flag){
			 write(fd,&obj,sizeof(obj));
		 }
		 lock.l_type=F_UNLCK;
		 fcntl(fd,F_SETLK,&lock);
		close(fd);
   	Admin_Menu(newSocket,uid);

}
void Delete_User(int newSocket,int uid){
	int fd,flag=0;
	char uid1[1024];
	struct flock lock;
	fd=open("USERINFO",O_RDWR,0777);
	UINFO obj;
	char type[1024];
	lock.l_type=F_WRLCK;
  lock.l_whence=SEEK_CUR;
  lock.l_start=0;
  lock.l_len=sizeof(obj);
	while(read(fd,&obj,sizeof(obj))){
		 if(obj.uid==0)continue;
		 send(newSocket,i2s(obj.uid),1024,0);
		 send(newSocket,obj.UserName,1024,0);
		 type[0]=obj.type;
		 send(newSocket,type,1024,0);
		 send(newSocket,i2s(obj.count),1024,0);

		}
	send(newSocket,"Exit",1024,0);
	recv(newSocket,uid1,1024,0);
	lseek(fd,0L,SEEK_SET);
	while(read(fd,&obj,sizeof(obj))){
		if(obj.uid==(s2i(uid1))){
			lseek(fd,-sizeof(obj),SEEK_CUR);
			fcntl(fd,F_SETLKW,&lock);
			 obj.uid=0;
			 strcpy(obj.UserName,"0");
			 strcpy(obj.Password,"0");
			 obj.type='0';
			 obj.count=0;
			 obj.valid=0;
			 flag=1;
		   break;
		}
	}
	if(flag)
		write(fd,&obj,sizeof(obj));
		lock.l_type=F_UNLCK;
	  fcntl(fd,F_SETLK,&lock);
		close(fd);
		Admin_Menu(newSocket,uid);
}

void Admin_Menu(int newSocket,int uid){
	char choice1[1024];
  recv(newSocket,choice1,1024,0);
  int choice=atoi(choice1);
	printf("%d\n",choice);
	switch(choice){
		case 1: Add_Train(newSocket,uid);
						break;
		case 2: Delete_Train(newSocket,uid);
						break;
		case 3: Delete_User(newSocket,uid);
						break;
		case 4: Show_Train(newSocket,uid);
		        break;
		case 5: Logout(newSocket,uid);
						break;
		deafult:printf("\n Wrong choice entered\n ");
						Admin_Menu(newSocket,uid);

	}
}
void login(int);
void Add_User(int newSocket){
	char buffer1[1024],buffer2[1024],msg[1024];
	int flag=0,d;
	recv(newSocket,buffer1,1024,0);
	int fd=open("USERINFO",O_RDWR,0777);
	UINFO obj;
	while(read(fd,&obj,sizeof(obj))){
		 d=obj.uid;
		if(strcmp(obj.UserName,buffer1)==0){
			flag=1;
			break;
		}
	}
	if(flag==1){
		//printf("exists\n");
		strcpy(msg,"-1");
		send(newSocket,msg,1024,0);
		Add_User(newSocket);
	  }
	else{
	//	printf("doesn't exists");
			strcpy(msg,"1");
			send(newSocket,msg,1024,0);
		 }

	recv(newSocket,buffer2,1024,0);
	obj.uid=d+1;
	strcpy(obj.UserName,buffer1);
	strcpy(obj.Password,buffer2);
	obj.type='U';
	obj.count=0;
	//obj.valid=1;
	lseek(fd,0L,SEEK_END);
	write(fd,&obj,sizeof(obj));
	//login(newSocket);
}
UINFO checkU( char buffer[]){
	 UINFO obj;
	 int fd=open("USERINFO",O_RDONLY,0744);
	 lseek(fd,0L,SEEK_SET);
	 while(read(fd,&obj,sizeof(obj))){
		 if(strcmp(obj.UserName,buffer)==0){
			 obj.valid=1;
			 close(fd);
			 return obj;

		 }
    }
   obj.valid=0;
	 return obj;

}

int checkP(char buffer[],UINFO obj){
printf("%s %s\n",buffer,obj.Password);
	 if(strcmp(obj.Password,buffer)==0)
	  return 1;
	 else
	  return -1;

}
void login(int newSocket){
	  char buffer1[1024],buffer2[1024],msg[1024];
		int k;
		recv(newSocket, buffer1, 1024, 0);
		UINFO obj=checkU(buffer1);//user exist  or not
		 recv(newSocket, buffer2, 1024, 0);

		if(obj.valid==1){
			// printf("%d %d\n",strcmp(buffer2,"admin"),strcmp(buffer2,"admin"));
			 if((strcmp(buffer2,"admin")==0) && (strcmp(buffer1,"admin")==0))
			  k=1;
			 else{
				 	printf("%s\n",buffer2);
		 			k=checkP(buffer2,obj);
					}
			printf("%d\n",k);
			 if(k==1){
				 // password matched
				  if(obj.type=='U'){
						 if(obj.count==1){
							 strcpy(msg,"1");
							 send(newSocket,msg,1024,0);
							 login(newSocket);
						  }
						 else{
							  obj.count=1;
								UINFO obj1;
								int fd=open("USERINFO",O_RDWR,0777);
								while(read(fd,&obj1,sizeof(obj1))){
									   if(obj.uid==obj1.uid){
											 lseek(fd,-sizeof(obj1),SEEK_CUR);
											 write(fd,&obj,sizeof(obj));
											 break;
										 }
									}
								close(fd);
							  strcpy(msg,"2");
								send(newSocket,msg,1024,0);
								User_Menu(newSocket,obj.uid);
						   }
						}
					else if(obj.type=='A'){
							  strcpy(msg,"3");
								send(newSocket,msg,1024,0);
								User_Menu(newSocket,obj.uid);
						   }

					else if(obj.type=='C'){
					      strcpy(msg,"4");
					      send(newSocket,msg,1024,0);
					      Admin_Menu(newSocket,obj.uid);
					    }
		   	}
			else{
				 //password not matched
				 strcpy(msg,"5");
				 send(newSocket,msg,strlen(msg),0);
				 login(newSocket);

			}

	  }

		else{
			 //Username doesn't exists
			 strcpy(msg,"6");
			 send(newSocket,msg,strlen(msg),0);
			 Add_User( newSocket);
			 login(newSocket);
		}
}


int main(){

	int sockfd, ret;
	 struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr =INADDR_ANY;

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", PORT);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);
	 	 login(newSocket);
    }

	close(newSocket);
}

	return 1;
}


/*struct flock lock;
  lock.l_type=F_WRLCK;
  lock.l_whence=SEEK_CUR;
  lock.l_start=0;
  lock.l_len=
  fcntl(fd,F_SETLKW,&lock);

  lock.l_type=F_UNLCK;
  fcntl(fd,F_SETLK,&lock);*/
