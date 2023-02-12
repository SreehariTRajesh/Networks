#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LOCALSERVER_PORT 4000
#define MAXLINE 1024

int main(int argc,const char* argv[]){
    if(argc!=2){
        printf("Usage <%s> <DOMAIN NAME>",argv[1]);
        exit(EXIT_FAILURE);
    }
    int sockFD;
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    server_addr.sin_port=htons(LOCALSERVER_PORT);
    server_addr.sin_family=AF_INET;

    sockFD=socket(AF_INET,SOCK_DGRAM,0);
    int len=sizeof(server_addr);
    char buffer[1024];
    strcpy(buffer,argv[1]);
    sendto(sockFD,buffer,sizeof(buffer),0,(struct sockaddr*)&server_addr,sizeof(server_addr));
    bzero(buffer,sizeof(buffer));
    recvfrom(sockFD,buffer,sizeof(buffer),0,(struct sockaddr*)&server_addr,&len);
    printf("%s\n",buffer); 
    return 0;
} 
