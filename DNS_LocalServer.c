#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<stdio.h>

#define PORT 4000
#define ROOT_PORT 5000
#define MAXLINE 1024

int main(){
    int lFD;
    struct sockaddr_in lserver_addr,client_addr;
    bzero(&lserver_addr,sizeof(lserver_addr));
    lFD=socket(AF_INET,SOCK_DGRAM,0);
    lserver_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    lserver_addr.sin_port=htons(PORT);
    lserver_addr.sin_family=AF_INET;
    bind(lFD,(struct sockaddr*)&lserver_addr,sizeof(lserver_addr));
    int len=sizeof(client_addr);
    char buffer[MAXLINE];
    while(1){
        int n=recvfrom(lFD,buffer,sizeof(buffer),0,(struct sockaddr*)&client_addr,&len);
        buffer[n]='\0';
        printf("Message From Client:%s\n",buffer);
        struct sockaddr_in root_server_addr;
        root_server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
        root_server_addr.sin_port=htons(ROOT_PORT);
        root_server_addr.sin_family=AF_INET;
        sendto(lFD,buffer,MAXLINE,0,(struct sockaddr*)&root_server_addr,sizeof(root_server_addr));
        n=recvfrom(lFD,buffer,MAXLINE,0,(struct sockaddr*)&root_server_addr,sizeof(root_server_addr));
        buffer[n]='\0';
        printf("Message received from root:%s\n",buffer);
        sendto(lFD,buffer,MAXLINE,0,(struct sockaddr*)&client_addr,sizeof(client_addr));
    }
    return 0;

}
