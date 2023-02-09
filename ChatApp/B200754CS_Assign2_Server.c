#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<pthread.h>
#define PORT 8000
int clientFD[10];
void broadCastMessage(char message[]){
    for(int i=0;i<10;++i){
        if(clientFD[i]!=-1){
            send(clientFD[i],message,strlen(message),0);
        }
    }
}
void Message(int index,char message[]){
    for(int i=0;i<10;++i){
        if(clientFD[i]!=-1 && i!=index){
            send(clientFD[i],message,strlen(message),0);
        }
    }
} 
struct client_details{
    int socket;
    int clientIndex; 
};
pthread_t clientThread[10];
int client_cnt=0;
void *chat(void*arg){
    pthread_detach(pthread_self());
    struct client_details cd=*((struct client_details*)arg);
    char buffer[1024];
    char name[10];
    int sock=cd.socket;
    int clientIdx=cd.clientIndex;
    clientFD[clientIdx]=sock;
    bzero(buffer,sizeof(buffer));
    bzero(name,sizeof(name));
    int x=recv(sock,name,10,0);
    char message[1024];
    sprintf(message,"%s has joined the chat",name);
    Message(clientIdx,message);
    while(1){
        int x=recv(sock,buffer,sizeof(buffer),0);
        if(x==0){
            clientFD[clientIdx]=-1;
            break;
        }
        printf("%s:%s\n",name,buffer);
        bzero(message,sizeof(message));
        sprintf(message,"%s:%s",name,buffer);
        broadCastMessage(message);
        bzero(buffer,sizeof(buffer));
    }
    char msg[1024];
    sprintf(msg,"%s, has left the chat",name);
    broadCastMessage(msg);
}
int main(){
    int serverFD,server_socket;
    struct sockaddr_in server_addr;
    int len=sizeof(server_addr);
    memset(clientFD,-1,sizeof(clientFD));
    char buffer[1024];
    serverFD=socket(AF_INET,SOCK_STREAM,0);
    if(serverFD<0){
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }
    else printf("Socket created successfully\n");
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    server_addr.sin_port=htons(PORT);

    int bind_status=bind(serverFD,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(bind_status<0){
       perror("Bind Failed\n");
       exit(EXIT_FAILURE);
    }
    int listen_status=listen(serverFD,10);
    if(listen_status<0){
        perror("Listen Failed\n");
        exit(EXIT_FAILURE);
    }
    else{
        printf("Listening....\n");
    }
    while(1){
        server_socket=accept(serverFD,(struct sockaddr*)&server_addr,(socklen_t*)&len);
        if(server_socket<0){
            perror("Client connection failed\n");
            continue;
        }
        else{
            printf("Client Connected Successfully\n");
            struct client_details cd;
            cd.socket=server_socket;
            cd.clientIndex=client_cnt;
            pthread_create(&clientThread[client_cnt],NULL,&chat,(void*)&cd);
            sleep(2);
            ++client_cnt;
        }
    }
}
