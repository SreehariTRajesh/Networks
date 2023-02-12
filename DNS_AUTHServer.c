#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<math.h>

#define AUTH_PORT 7000
#define CLIENT_PORT 6000

#define MAXLINE 1024
struct NSLookupEntry{
    char ip[32];
    char domain[32];
};
char*generateIPv4(int cnt){
    srand(cnt);
    char*IP;
    IP=malloc(32*sizeof(char));
    int cnt0=rand()%255;
    int cnt1=rand()%255;
    int cnt2=rand()%255;
    int cnt3=rand()%255;
    sprintf(IP,"%d.%d.%d.%d",cnt0,cnt1,cnt2,cnt3);    
    return IP;
}
struct NSLookupEntry* generateIPTable(const char* domain,int count,int randomizer){
    struct NSLookupEntry *IPTable;
    IPTable=malloc(count*sizeof(struct NSLookupEntry));
    for(int i=0;i<count;++i){
        sprintf(IPTable[i].domain,"ns%d.%s",i,domain);
        strcpy(IPTable[i].ip,generateIPv4(i*100+12000+randomizer*10));
    }
    return IPTable;
}
int main(int argc,const char *argv[]){
    int sockFD;
    struct NSLookupEntry*IPTable=generateIPTable(argv[1],atoi(argv[2]),atoi(argv[3]));
    sockFD=socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in auth_server_addr,client_addr;
    auth_server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    int x;
    if(strcmp(argv[1],"google.com")==0){
        x=7000;
    }
    else if(strcmp(argv[1],"google.org")==0){
        x=7001;
    }
    else if(strcmp(argv[1],"google.net")==0){
        x=7002;
    }
    else if(strcmp(argv[1],"facebook.com")==0){
        x=7003;
    }
    else if(strcmp(argv[1],"facebook.org")==0){
        x=7004;
    }
    else if(strcmp(argv[1],"facebook.net")==0){
        x=7005;
    }
    else if(strcmp(argv[1],"amazon.com")==0){
        x=7006;
    }
    else if(strcmp(argv[1],"amazon.org")==0){
        x=7007;
    }
    else if(strcmp(argv[1],"amazon.net")==0){
        x=7008;
    }
    auth_server_addr.sin_port=htons(x);
    auth_server_addr.sin_family=AF_INET;

    bind(sockFD,(struct sockaddr*)&auth_server_addr,sizeof(auth_server_addr));

    int len=sizeof(client_addr);
    char buffer[MAXLINE];
    printf("AuthServer:\n");
    for(int i=0;i<atoi(argv[2]);++i){
        printf("IP:%s HostName:%s\n",IPTable[i].ip,IPTable[i].domain);
    }
    while(1){
        bzero(buffer,MAXLINE);
        int n=recvfrom(sockFD,buffer,MAXLINE,0,(struct sockaddr*)&client_addr,&len);
        buffer[n]='\0';
        printf("Buffer Message Received:%s\n",buffer);
        srand(0);
        int x=rand()%atoi(argv[2]);
        strcpy(buffer,IPTable[x].ip);
        sendto(sockFD,buffer,MAXLINE,0,(struct sockaddr*)&client_addr,sizeof(client_addr));
    }
    close(sockFD);
    return 0;
}
