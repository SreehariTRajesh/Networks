#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ROOT_SERVER_PORT 5000 
#define LOCAL_SERVER_PORT 4000
#define TLD_SERVER_PORT0 6000
#define TLD_SERVER_PORT1 6400
#define TLD_SERVER_PORT2 6600
#define MAXLINE 1024

int main(){
    int sockFD;
    struct sockaddr_in root_server_addr,client_addr,tld_server_addr;
    bzero(&root_server_addr,sizeof(root_server_addr));
    bzero(&client_addr,sizeof(client_addr));
    bzero(&tld_server_addr,sizeof(tld_server_addr));
    sockFD=socket(AF_INET,SOCK_DGRAM,0);
    root_server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    root_server_addr.sin_port=htons(ROOT_SERVER_PORT);
    root_server_addr.sin_family=AF_INET;
    
    client_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    client_addr.sin_port=htons(LOCAL_SERVER_PORT);
    client_addr.sin_family=AF_INET;
    
    int len=sizeof(client_addr);
    bind(sockFD,(struct sockaddr*)&root_server_addr,sizeof(root_server_addr));
    char buffer[1024];
    while(1){
        bzero(buffer,sizeof(buffer));
        int n=recvfrom(sockFD,buffer,sizeof(buffer),0,(struct sockaddr*)&client_addr,&len);
        printf("Domain Name Received:%s\n",buffer);
        if(buffer[strlen(buffer)-1]=='m' && buffer[strlen(buffer)-2]=='o' && buffer[strlen(buffer)-3]=='c'){
            tld_server_addr.sin_port=htons(TLD_SERVER_PORT0);
        }
        if(buffer[strlen(buffer)-1]=='g' && buffer[strlen(buffer)-2]=='r' && buffer[strlen(buffer)-3]=='o'){
            tld_server_addr.sin_port=htons(TLD_SERVER_PORT1);
        }
        if(buffer[strlen(buffer)-1]=='t' && buffer[strlen(buffer)-2]=='e' && buffer[strlen(buffer)-3]=='n'){
            tld_server_addr.sin_port=htons(TLD_SERVER_PORT2);
        }
        tld_server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
        tld_server_addr.sin_family=AF_INET;
    
        sendto(sockFD,buffer,sizeof(buffer),0,(struct sockaddr*)&tld_server_addr,sizeof(tld_server_addr));

        n=recvfrom(sockFD,buffer,sizeof(buffer),0,(struct sockaddr*)&tld_server_addr,&len);

        sendto(sockFD,buffer,sizeof(buffer),0,(struct sockaddr*)&client_addr,sizeof(client_addr));
    }
    close(sockFD);
    return 0;
}
