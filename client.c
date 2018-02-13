#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<signal.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>

void sig_alarm(int signo);

int fd_set_blocking(int fd,int blocking){
  int flags = fcntl(fd,F_GETFL,0);
  if(flags==-1){
   return 0;
  }
  if(blocking){
   flags &= ~O_NONBLOCK;
  }else{
   flags |= O_NONBLOCK;
  }
  
  return fcntl(fd,F_SETFL,flags) != -1;
}

int main(){
 struct sockaddr_in address;
 void*  sig_func;
 int clisock = socket(AF_INET,SOCK_STREAM,0);
 int x = fd_set_blocking(clisock,0);
 printf("%d\n",x);
 char buffer[1024];
 int len,n;
 bzero(&address,sizeof(address));
 
 if(clisock<0){
  printf("Error creating socket..\n");
 }else{
  printf("Socket created successfully..\n");
  address.sin_family = AF_INET;
  address.sin_port = htons(80);
  address.sin_addr.s_addr = inet_addr("54.225.128.21");
  
  sig_func = signal(SIGALRM,sig_alarm);
  if(alarm(5)!=0){
    printf("alarm already set\n");
  }
   //sleep(10);
 
  if((n=connect(clisock,(struct sockaddr *)&address,sizeof(address)))==0){
  
   printf("connection successfull..\n");
   alarm(0);
   signal(SIGALRM,sig_func);
   //communicate
   while(1){
    printf("Enter data:\n");
    scanf("%s",buffer);
    if(write(clisock,buffer,strlen(buffer))){
     
    }else{
     printf("Sent failed..\n");
    }
    len = read(clisock,buffer,1024);
    buffer[len]='\0';
    if(len ==0)
    break;
    printf("received: %s\n",buffer);
   }
  }else if(n<0){
    if(errno==EINTR)
      errno = ETIMEDOUT;
  }
  close(clisock);
 }
 return 0;
}

void sig_alarm(int signo){
 printf("timeout\n");
 return;
}
