#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
# include <string.h>

using namespace std;
# define BUF_LEN 512
#define MAXLINE 4096

int main(int argc, char** argv)
{
int    sockfd,n;
struct sockaddr_in    servaddr;
char 			text[BUF_LEN];
char 			handle[BUF_LEN];
string 			travailsave[20];
string   priosave[20];
int recuqmount;
char prio[3];
char continu[1];
continu[0]='y';
int count=0;
if( argc != 2){
exit(0);
}

if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
cout<<"établisation du socket est echoe"<<endl;
exit(0);
}

memset(&servaddr, 0, sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_port = htons(4479);
if( inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
cout<<"address du socket est echoe"<<endl;
exit(0);
}

if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
cout<<"connection du socket est echoe"<<endl;
exit(0);
}
memcpy(text,"client",6);
if(send(sockfd, text, strlen(text),0)< 0)
{
cout<<"send msg erre ";
exit(0);
}

while(continu[0]=='y'){

cout<<"saisir msg: ";
cin>>text;
travailsave[count]=text;
if(send(sockfd, text, strlen(text),0)< 0)
{
cout<<"send msg erre ";
exit(0);
}



cout<<"\nsaisir priorite(maxprio=999): ";
cin>>prio;
priosave[count]=prio;

if(send(sockfd, prio, strlen(prio),0)< 0)
{
cout<<"send prority erre ";
exit(0);
}


cout<<"\ncontinuer saisir msg?(y/n): "<<endl;
cin>>continu;
if(send(sockfd, continu, strlen(continu),0)< 0)
{
cout<<"send continue erre ";
exit(0);
}

  ++count;
}
system("clear");


cout<<"les travail est envoyé :\n";
for(int n=0;n<count;n++)
cout<<"\tmsg "<< ": "<<travailsave[n]<<"      prio:"<<priosave[n]<<endl;


close(sockfd);

}
