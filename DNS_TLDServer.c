#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#define CLIENT_PORT 5000
#define com_google 7000
#define org_google 7001
#define net_google 7002
#define com_facebook 7003
#define org_facebook 7004
#define net_facebook 7005
#define com_amazon 7006
#define net_amazon 7007
#define org_amazon 7008
#define MAXLINE 1024

int main(int argc,const char*argv[]){
    int sockFD;
    sockFD=socket(AF_INET,SOCK_DGRAM,0);
    
    struct sockaddr_in tld_server_addr,auth_server_addr,client_addr;

    client_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    client_addr.sin_port=htons(CLIENT_PORT);
    client_addr.sin_family=AF_INET;

    tld_server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    if(strcmp(argv[1],".com")==0){
        printf("TLD .com\n");
        tld_server_addr.sin_port=htons(6000);
    }
    else if(strcmp(argv[1],".org")==0){
        printf("TLD .org\n");
        tld_server_addr.sin_port=htons(6400);
    }
    else if(strcmp(argv[1],".net")==0){
        printf("TLD .net\n");
        tld_server_addr.sin_port=htons(6600);
    }
    tld_server_addr.sin_family=AF_INET;
    int len=sizeof(struct sockaddr);
    
    char buffer[1024];
    
    bind(sockFD,(struct sockadd*)&tld_server_addr,sizeof(tld_server_addr));
    while(1){ 
        int n=recvfrom(sockFD,buffer,sizeof(buffer),0,(struct sockadd*)&client_addr,&len);
        // tld server receives domain names;
        buffer[n]='\0';
        printf("Domain Name Received:%s\n",buffer);

        auth_server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
        if(strcmp(buffer,"www.google.com")==0){
            auth_server_addr.sin_port=htons(7000);
        }
        else if(strcmp(buffer,"www.google.org")==0){
            auth_server_addr.sin_port=htons(7001);
        }
        else if(strcmp(buffer,"www.google.net")==0){
            auth_server_addr.sin_port=htons(7002);
        }
        else if(strcmp(buffer,"www.facebook.com")==0){
            auth_server_addr.sin_port=htons(7003);
        }
        else if(strcmp(buffer,"www.facebook.org")==0){
            auth_server_addr.sin_port=htons(7004);
        }
        else if(strcmp(buffer,"www.facebook.net")==0){
            auth_server_addr.sin_port=htons(7005);
        }
        else if(strcmp(buffer,"www.amazon.com")==0){
            auth_server_addr.sin_port=htons(7006);
        }
        else if(strcmp(buffer,"www.amazon.org")==0){
            auth_server_addr.sin_port=htons(7007);
        }
        else if(strcmp(buffer,"www.amazon.net")==0){
            auth_server_addr.sin_port=htons(7008);
        }
        auth_server_addr.sin_family=AF_INET;

        sendto(sockFD,buffer,sizeof(buffer),0,(struct sockaddr*)&auth_server_addr,sizeof(auth_server_addr));
    
        n=recvfrom(sockFD,buffer,sizeof(buffer),0,(struct sockaddr*)&auth_server_addr,&len);

        buffer[n]='\0';
        sendto(sockFD,buffer,sizeof(buffer),0,(struct sockaddr*)&client_addr,sizeof(client_addr));
    }
    close(sockFD);
    return 0;
}
