//C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
//POSIX
#include <unistd.h>
#include <sys/types.h> 
//socket
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc , char *argv[])
{
	assert(argc>1);
	int port = atoi(argv[1]);
    //socket
    int server_fd = socket(AF_INET , SOCK_STREAM , 0);
    assert(server_fd >= 0);
    //AF_INET: using IPv4
    //SOCK_STREAM: using TCP
    //SOCK_DGRAM: using UDP
    //protocal: set to 0

    //socket information
    struct sockaddr_in clientinfo;
    memset(&clientinfo,0,sizeof(sockaddr_in));
    clientinfo.sin_family = PF_INET;//sockaddr_in is IPv4

    //localhost test
    clientinfo.sin_addr.s_addr = inet_addr("127.0.0.1"); //server's address
    clientinfo.sin_port = htons(port); //using port, htons(Host TO Network Short integer)

    //connect
    int retval = connect(server_fd,(struct sockaddr *)&clientinfo,sizeof(clientinfo));
    //sucess return 0
    assert(!retval);

    char buffer[BUFFER_SIZE]; memset(buffer,0,BUFFER_SIZE);
    char message[] = "a client client connect.\n";
    send(server_fd,message,strlen(message)+1,0);
    recv(server_fd,buffer,BUFFER_SIZE,0);
    printf("from server: %s\n", buffer); memset(buffer,0,BUFFER_SIZE);
    ssize_t sz;
    //Send a message to server
    while(1)
    {
        read(STDIN_FILENO,buffer,BUFFER_SIZE);
        if(!strcmp(buffer,"exit\n"))
            break;
        send(server_fd,buffer,strlen(buffer)+1,0); memset(buffer,0,BUFFER_SIZE);
    }
    printf("close Socket...\n");
    close(server_fd);
    return 0;
}