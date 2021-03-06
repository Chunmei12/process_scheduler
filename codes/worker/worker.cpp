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
#include <fstream>
#include <iomanip>
#include <vector>


#define MAXLINE 1024
using namespace std;

int writefile(  char buf[1024]);
int readtest();
vector<string> vec;
int    sockfd;
char    sendline[1024];

int main(int argc, char** argv)
{

    struct sockaddr_in    servaddr;

    if( argc != 3){
    printf("usage: ./client <ipaddress>\n worker pas saisir\n");
    exit(0);
    }

    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
    exit(0);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(4479);
    if( inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
    printf("inet_pton error for %s\n",argv[1]);
    exit(0);
    }

    if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
    printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
    exit(0);
    }

   memset(&sendline,0,sizeof(sendline));
    if( send(sockfd, argv[2], strlen(argv[2]), 0) < 0)
    {
    printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
    exit(0);
    }

printf("entering\n");
  while(1){
   if( recv(sockfd, sendline, sizeof(sendline), 0) < 0)
      {
      printf("recv msg error: %s(errno: %d)\n", strerror(errno), errno);
      exit(0);
      }
    writefile(sendline);
    readtest();
    if (remove("travail.txt")!=0) {
      cout<<"err";
    }
    memcpy(&sendline,"libre",5);
    sendline[6]='0';
     if( send(sockfd, sendline, strlen(sendline), 0) < 0)
     {
     printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
     exit(0);
     }

   }
    close(sockfd);
    exit(0);
}

int writefile(  char buf[1024])
{
    ofstream myfile;
    myfile.open("travail.txt",ios::out);  //新建一个可写的文件
    myfile <<buf;
    myfile.close();
    return 0;
}

int readtest() {
    cout<<"worker"<<endl;
  char buffer[1024];
  system("clear");  //清屏
  ifstream in("travail.txt"); //读取文件
  if (! in.is_open()){
    cout << "Error opening file";
    exit (1);
  }
  while(in.getline(buffer,1024)){
    vec.push_back(buffer);
  }
  in.close();
  memset(buffer,0,sizeof(buffer));
  vector<string>::iterator it;
  do{
      it=vec.begin();
      for(size_t i=0; i<vec.size(); i++){
        cout<<"processus '"<<i<<"' est en train d'être executé: "<<vec[i]<<endl;
      }
      sleep(5);
      system("clear");  //清屏
      vec.erase(vec.begin());
  }while( it!=vec.end()--);
  printf("\n");
  return 0;
}
