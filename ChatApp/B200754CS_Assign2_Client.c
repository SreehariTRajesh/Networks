#include<arpa/inet.h>
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#define PORT 8000
void*receiveMessage(void*arg){
    int sock=*((int*)arg);
    char buffer[1024];
    while(1){
        bzero(buffer,1024);
        int x=recv(sock,buffer,sizeof(buffer),0);
        if(x==0){
            printf("Socket Closed\n");
            break;
        }
        printf("%s\n",buffer);
    }
}
int main(){
    int sock=0,clientFD;
    struct sockaddr_in server_addr;
    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0){
        perror("Socket creation Error\n");
        exit(EXIT_FAILURE);
    }
    else{
        printf("Client Socket Created Successfully\n");
    }
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=INADDR_ANY;
    server_addr.sin_port=htons(PORT);
    char name[10];
    printf("Enter your name:");
    scanf("%[^\n]%*c",name);
    int client_fd=connect(sock,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(client_fd<0){
        perror("Connection failed\n");
        exit(EXIT_FAILURE);
    }
    else printf("Connected Successfully\n");
    send(sock,name,10,0);
    pthread_t rt;
    pthread_create(&rt,NULL,&receiveMessage,(void*)&sock);
    while(1){
        char message[1024];
        bzero(message,sizeof(message));
        scanf("%[^\n]%*c",message);
        if(strcmp(message,"\\exit")==0 || strcmp(message,"\\quit")==0 || strcmp(message,"\\part")==0)
        {
            pthread_cancel(rt);
            close(sock);
            exit(0);
        }
        else send(sock,message,1024,0);
    }
    return 0;
}
