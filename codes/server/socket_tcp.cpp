#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
# include <fcntl.h>	/* For O_* constants */
# include <sys/stat.h>	/* For mode constants */
# include <mqueue.h>
# include <string.h>
#include<sys/msg.h>
#include <sys/ipc.h>
#include <fstream>
#include <vector>
#include <signal.h>
# define BUF_LEN 512
#define MAXLINE 1024
#define MQ_NAME "/tem"
#define  filename "processus.txt"
#define FIFO_FILE   "processus.fifo"
using namespace std;
int writefile (char buf[1024]);
int plock(int mutex);
int vlock(int mutex);
char * readfile ();
void backserie(mqd_t My_MQ, char handle[BUF_LEN],int connfd,int count);
void deltefile_old();



int main(int argc, char** argv)
{

//const char* MQ_NAME1[]={"/temp","/queuetest"};
int    listenfd;
struct sockaddr_in    servaddr;
int     n=0;
int count=0;
char prio[3]={'0','0','0'};
int prioer=0;
vector<char *> clientpid;
int full=0;
int lock=0;
char continu[2];
continu[0] = 'y';
continu[1]='\0';
int connfd;
int client=0;
//const char* MQ_NAME=MQ_NAME1[client];
int react[1]={0};
 int fd;

char 			buff[BUF_LEN];
char 			handle[BUF_LEN];

deltefile_old();
system("clear");
if(mkfifo(FIFO_FILE, S_IRWXU | S_IRGRP | S_IWGRP) == -1)
{
	fprintf(stderr, "Erreur de création du tube");
	exit(EXIT_FAILURE);
}


if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
exit(0);
}

memset(&servaddr, 0, sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
servaddr.sin_port = htons(4479);

if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
   cout<<"bind errer";
exit(0);
}

if( listen(listenfd, 10) == -1){
cout<<"listen ERRE";
exit(0);
}

//continu[0] == 'y';

            /*prosix MQ:messages queeu*/
            struct mq_attr	attr;
            mqd_t 		My_MQ;
            attr.mq_flags = rand()/10;
            attr.mq_maxmsg = 10;
            attr.mq_msgsize = BUF_LEN;
            attr.mq_curmsgs = 0;
            /*processus mq créer*/
            My_MQ = mq_open(MQ_NAME, O_RDWR | O_CREAT, 0644, &attr);
            if ((int) My_MQ == -1)
            {
                  cout<<"Error : mq_open failed !"<<endl;
                  exit(0);
            }
int mainclient=getpid();
while(1){




            while((connfd = accept(listenfd, (struct sockaddr*)NULL, NULL))<=0)continue;
						pid_t childPID =fork();
						if(getppid()==mainclient)continue;

            char fs[BUF_LEN];
            if(recv(connfd, fs, BUF_LEN,0)< 0)
            {
            cout<<"recv meeeas erre"<<endl;
            exit(0);
            }
            int machinetype= strcmp(fs,"worker");

            char machinetype1;
						int justify;
            char typeworker1[50];
			      char* filesend;
            switch(machinetype)
			{
            case 0:
						printf("new worker id: %d\n",connfd);
            fd= open(FIFO_FILE, O_RDONLY);  //communique entre prefessuse par FIFO
				    while(1)
					{
				           read(fd,react, 1);
				            justify=react[0];
										if(justify==1){
						             	react[0]=0;
											   	justify=react[0];
	                        filesend=readfile();

	                        if(send(connfd, filesend, strlen(filesend),0)< 0)
	                        {
	                        	cout<<"pas  file de processus est erre"<<endl;
	                        	exit(0);
	                        }

	                      if(n=recv(connfd, typeworker1, BUF_LEN, 0)) //reponse de worker
												    cout<<"\t\t\t\t\tworker "<<connfd<< " : libre"<<endl;

                 }

					}

		        break;

                default:
								printf("user %d a des travai ci-dessus :\n",connfd);
                    client=getpid();
                    while(continu[0]=='y')
											{
												  memset(prio, 0, BUF_LEN);
                        n = recv(connfd, buff, sizeof(buff), 0);
                        buff[n] = '\0';
                        n = recv(connfd, prio, sizeof(buff), 0);
                        prio[n]='\0';
                        prioer=atoi(prio);
                        count++;
                        cout<<"\tmsg: "<<buff<<"  priorite:"<<prioer<<endl;
                        mq_send(My_MQ, buff, strlen(buff)+1, prioer);
                        n = recv(connfd, continu, BUF_LEN, 0);
                        continu[1]='\0';

                    }
                   	backserie( My_MQ ,handle,connfd,count);
                    fd= open(FIFO_FILE, O_WRONLY);  //communique entre prefessuse par FIFO
										react[0]=1;

			          		mq_close(My_MQ);
                    mq_unlink(MQ_NAME);
                    close(connfd);
										write(fd, react, 1 );
                    wait();
									  kill( getpid(), SIGKILL );


                }
                     //对每个client的东西进行排序
            }

close(listenfd);
}


void backserie(mqd_t My_MQ, char	handle[BUF_LEN],int connfd,int count){
  while(count>0){
            char prio[10],timerun[10],stat[5]/*,space[2]=' ',spaceln[2]='\n'*/;
            mq_attr mqAttr;
            mq_getattr(My_MQ, &mqAttr);
            sprintf(prio,"%ld",mqAttr.mq_curmsgs);
            sprintf(timerun,"%ld",mqAttr.mq_msgsize);
            sprintf(stat,"%s","attend");
            memset(handle, 0, BUF_LEN);

            int len=mq_receive(My_MQ, handle, BUF_LEN, NULL);
            handle[len]='\0';
						char buf[BUF_LEN];
						sprintf(buf,"%s %s %s %s\n",handle,prio,timerun,stat);
						writefile(buf);
            count--;

  }
}

  char * readfile () {
  std::ifstream is (filename, std::ifstream::binary);
	string line;
    if (is) {
      // get length of file:
      is.seekg (0, is.end);
      int length = is.tellg();
      is.seekg (0, is.beg);

      char * buffer = new char [length];
      is.read (buffer,length);

      is.seekg (0, is.beg);//  delete file origine
			ofstream out("outfile.txt");
	    is.close();
	    out.close();
	    // delete the original file
	    remove("processus.txt");
	    // rename old to new
	    rename("outfile.txt","processus.txt");



  return buffer;
}
}



int writefile(  char buf[BUF_LEN])
{
ofstream myfile;
myfile.open(filename,ios::app);  //新建一个可写的文件
myfile << buf;
myfile.close();

return 0;
}


int plock(int mutex){
  mutex--;
  return mutex;
}


int vlock(int mutex){
  mutex++;
  return mutex;
}

void deltefile_old(){

	remove("processus.txt");
	remove("processus.fifo");

}
