#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444
char* i2s(int val)
{
	static char buf[32];
	int i=30;
	for(; val && i ; --i, val/= 10);
	buf[i]="0123456789abcdef"[val % 10];
	return &buf[i+1];
}
int s2i(char* str)
{
	int l=strlen(str);
	int num=0;
	int flag=0;
	for(int i=0;i<l;i++){
		if(i==0 && !(str[i]>='0'&& str[i]<='9')){
			flag=1;
			continue;
		}
		num=num*10+(str[i]-'0');
	}
	if(flag)num*=-1;
	return num;
}
void User_Menu(int);
void Ticket_History(int clientSocket)
{
	char msg[1024];
	printf("\nUid\tTrain NO.\tSource\tDestination\tDate\tTicket NO.\n");
	while(1)
	{
		recv(clientSocket,msg,1024,0);
		if(strcmp(msg,"Stop")==0)
		break;
		printf("\n%s",msg);
		recv(clientSocket,msg,1024,0);
		printf("\t%s",msg);
		recv(clientSocket,msg,1024,0);
		printf("\t%s",msg);
		recv(clientSocket,msg,1024,0);
		printf("\t%s",msg);
		recv(clientSocket,msg,1024,0);
		printf("\t%s OCT 2019",msg);
		recv(clientSocket,msg,1024,0);
		printf("\t%s",msg);
	}
	User_Menu(clientSocket);
}
void Book_Ticket(int clientSocket){
	char msg[1024],source[1024],destination[1024],date[1024],train_no[1024];
	char ticket_no[1024];
   char chk[1024];
	printf("\nEnter Source\n");
	scanf("%s",source);
	send(clientSocket,source,1024,0);
	printf("\nEnter destination\n");
	scanf("%s",destination);
	send(clientSocket,destination,1024,0);
	recv(clientSocket,chk,1024,0);
	printf("%s",chk);
	printf("\nEnter date from 1 to 7 for october 2019\n");
	scanf("%s",date);
	send(clientSocket,date,1024,0);

	printf("\nTrains Available\n");
	while(1)
	{
		recv(clientSocket,msg,1024,0);
		if(strcmp(msg,"Stop")==0)
		break;
		printf("\n%s",msg);
		recv(clientSocket,msg,1024,0);
		printf("\n%s",msg);
		recv(clientSocket,msg,1024,0);
		printf("\n%s",msg);
		recv(clientSocket,msg,1024,0);
		printf("\n%s",msg);
	}
	printf("\nEnter train Number\n");
	scanf("%s",train_no);
	send(clientSocket,train_no,1024,0);
	getchar();
	getchar();
	recv(clientSocket,ticket_no,1024,0);
	if(strcmp(ticket_no,"-1")==0)
	printf("please wait\n");
	else if(strcmp(ticket_no,"2")==0){
		printf("\nSeats not Available kindly try again\n");
		User_Menu(clientSocket);
	}
	else{
		printf("Ticket booked Successfully with your ticket number as %s",ticket_no);
		User_Menu(clientSocket);
	}
}
void Cancel_Ticket(int clientSocket)
{
	char ticket_no[1024],date[1024],msg[1024],train_no[1024];
	printf("\nUid\tTrain NO.\tSource\tDestination\tDate\tTicket NO.\n");
	while(1)
	{
		recv(clientSocket,msg,1024,0);
		if(strcmp(msg,"Stop")==0)
		break;
		printf("\n%s",msg);
		recv(clientSocket,msg,1024,0);
		printf("\t%s",msg);
		recv(clientSocket,msg,1024,0);
		printf("\t%s",msg);
		recv(clientSocket,msg,1024,0);
		printf("\t%s",msg);
		recv(clientSocket,msg,1024,0);
		printf("\t%s OCT 2019",msg);
		recv(clientSocket,msg,1024,0);
		printf("\t%s",msg);
	}


	printf("\nEnter date from 1 to 7 for october 2019 for which you want to cancel ticket\n");
	scanf("%s",date);
	printf("\nEnter ticket_no Number\n");
	scanf("%s",ticket_no);
	send(clientSocket,date,1024,0);
	send(clientSocket,ticket_no,1024,0);

	recv(clientSocket,msg,1024,0);
	if(strcmp(msg,"1")==0)
	printf("\nTicket deleted Successfully");
	else
	printf("\n Ticket does not exists");
	User_Menu(clientSocket);
}
void Logout(int clientSocket)
{
send(clientSocket,"Exit",1024,0);
printf("\n Happy Journey");
close(clientSocket);
exit(0);
}
void User_Menu(int clientSocket){
	printf("\n 1: Book ticket\n 2: Cancel Ticket\n 3: Ticket History \n 4: Logout\n");
	printf("\n Enter your choice");
	char choice1[1024];
	scanf("%s",choice1);
	int choice=s2i(choice1);
	printf("%d",choice);
	send(clientSocket,choice1,1024,0);

	switch(choice)
	{
		case 1: Book_Ticket(clientSocket);
						break;
		case 2: Cancel_Ticket(clientSocket);
						break;
		case 3: Ticket_History(clientSocket);
						break;
		case 4: Logout(clientSocket);
						break;
		default: printf("\n Wrong choice entered\n");
						User_Menu(clientSocket);
	}
}
void Admin_Menu(int);
void Add_Train(int clientSocket){
	char train_no[1024],source[1024],destination[1024],d_seat[1024],msg[1024];
	printf("\nWelcome to Uttrakhand Railways\n");
	printf("\nYou are going to add train now from 1 oct 2019 to 7 oct 2019\n");
	printf("\nEnter train number\n");
	scanf("%s",train_no);
	send(clientSocket,train_no,1024,0);
	recv(clientSocket,msg,1024,0);
	if(strcmp(msg,"1")==0)
	printf("Train already exists,try adding with another train number\n");
	else {

		printf("\nEnter source\n");
		scanf("%s",source);
		send(clientSocket,source,1024,0);
		printf("\nEnter destination\n");
		scanf("%s",destination);
		send(clientSocket,destination,1024,0);
		printf("\nEnter seats in the train\n");
		scanf("%s",d_seat);
		send(clientSocket,d_seat,1024,0);

		printf("\nTrain Added Successfully\n");

	}

	Admin_Menu(clientSocket);
}
void Show_Train(int clientSocket){
	char train_no[1024],source[1024],destination[1024],seats[1024];
	printf("\nTrains Available from 1 to 7 oct 2019\n");
	printf("\nTrain NO\tSource\tDestination\tSeats\n");
	while(1)
	{
		recv(clientSocket,train_no,1024,0);
		if(strcmp(train_no,"Exit")==0)
		break;
		printf("\n%s",train_no);
		recv(clientSocket,source,1024,0);
		printf("\t%s",source);
		recv(clientSocket,destination,1024,0);
		printf("\t%s",destination);
		recv(clientSocket,seats,1024,0);
		printf("\t%s",seats);
	}
	Admin_Menu(clientSocket);
}
void Delete_Train(int clientSocket){
	char train_no[1024],source[1024],destination[1024],seats[1024];
	printf("\nTrains Available from 1 to 7 oct 2019\n");
	printf("\nTrain NO\tSource\tDestination\tSeats\n");
	while(1)
	{
		recv(clientSocket,train_no,1024,0);
		if(strcmp(train_no,"Exit")==0)
		break;
		printf("\n%s",train_no);
		recv(clientSocket,source,1024,0);
		printf("\t%s",source);
		recv(clientSocket,destination,1024,0);
		printf("\t%s",destination);
		recv(clientSocket,seats,1024,0);
		printf("\t%s",seats);
	}
	printf("\nEnter Train Number");
	scanf("%s",train_no);
	send(clientSocket,train_no,1024,0);
	printf("\nTrain deleted Successfully\n");
	Admin_Menu(clientSocket);

}
void Delete_User(int clientSocket){
	char uid[1024],uname[1024],type[1024],count[1024];
	printf("\nUsers Available\n");
	printf("\nUID\tUserNAME\tTYPE\tCOUNT\n");
	while(1)
	{
		recv(clientSocket,uid,1024,0);
		if(strcmp(uid,"Exit")==0)
		break;
		printf("\n%s",uid);
		recv(clientSocket,uname,1024,0);
		printf("\t%s",uname);
		recv(clientSocket,type,1024,0);
		printf("\t%s",type);
		recv(clientSocket,count,1024,0);
		printf("\t%s",count);
	}
	printf("\nEnter Uid");
	scanf("%s",uid);
	send(clientSocket,uid,1024,0);
	printf("\nUser deleted Successfully\n");
	Admin_Menu(clientSocket);

}
void Admin_Menu(int clientSocket){
	printf("\n 1: Add Train\n 2: Delete Train\n 3: Delete User \n 4: Show Train\n 5: Logout \n");
	printf("\n Enter your choice");
	char choice1[1024];
	scanf("%s",choice1);
	int choice=s2i(choice1);
	printf("%d",choice);
	send(clientSocket,choice1,1024,0);
	switch(choice)
	{
		case 1: Add_Train(clientSocket);
						break;
		case 2: Delete_Train(clientSocket);
						break;
		case 3: Delete_User(clientSocket);
						break;
		case 4: Show_Train(clientSocket);
						break;
		case 5: Logout(clientSocket);
						break;
		default: printf("\n Wrong choice entered\n");
						Admin_Menu(clientSocket);
	}
}
void login(int);
void Add_User(int clientSocket)
{
	char buffer1[1024],buffer2[1024],msg[1024];
	printf("\nYou are not registered, kindly register yourself\n");
	printf("\n Username:\t");
	scanf("%s",buffer1);
	send(clientSocket,buffer1,1024,0);
	recv(clientSocket,msg,1024,0);
	int z=s2i(msg);
	printf("%d\n",z);
	if(z==1)
	{
		printf("\n Password:\t");
		scanf("%s",buffer2);
		send(clientSocket,buffer2,1024,0);
		printf("\nSuccessfully Registered, Kindly login now with fresh credentials\n");
	//	login(clientSocket);
	}
	else
	{
		//printf("\n Password:\t");
		printf("\n User name already exists , try some other username\n");
		Add_User(clientSocket);
	}
}
void login(int clientSocket){
	  char buffer1[1024],buffer2[1024],msg[1024];
		printf("UserName: \t");
 		scanf("%s",buffer1);
 		send(clientSocket,buffer1,1024,0);
 		printf("Password: \t");
 		scanf("%s",buffer2);
 		send(clientSocket,buffer2,1024,0);
		recv(clientSocket,msg,1024,0);
		int ch=s2i(msg);
		switch(ch){
			 case 1: printf("\nUser Already logged in\n");
			         login(clientSocket);
							 break;
			 case 2: printf("\nUser logged in Successfully\n");
			        User_Menu(clientSocket);
							 break;
			 case 3: printf("\n Agent logged in Successfully\n");
			         User_Menu(clientSocket);
							 break;
			 case	4: printf("\n Admin logged in Successfully\n");
			         Admin_Menu(clientSocket);
							 break;
			 case 5: printf("\n Password mismatched\n");
			 				 login(clientSocket);
							 break;
			 case 6: printf("\n User doesn't exists\n");
			         Add_User(clientSocket);
							 login(clientSocket);
							 break;
			 deafult:			printf("error");

			}
	 }


int main(){

	int clientSocket, ret;
	struct sockaddr_in clientAddr;
	char buffer1[1024],buffer2[1024],buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&clientAddr, '\0', sizeof(clientAddr));
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(PORT);
	clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");
  login(clientSocket);

close(clientSocket);




	return 1;
}
