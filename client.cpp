#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <assert.h>


int main(int argc , char *argv[])
{

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
    clientinfo.sin_port = htons(8700); //using port, htons(Host TO Network Short integer)

    //connect
    int err = connect(server_fd,(struct sockaddr *)&clientinfo,sizeof(clientinfo));
    if(err==-1){printf("Connection error");}

    //Send a message to server
    char message[] = {"Hi there"};
    char receiveMessage[100] = {};
    send(server_fd,message,sizeof(message),0);
    recv(server_fd,receiveMessage,sizeof(receiveMessage),0);

    printf("%s",receiveMessage);
    printf("close Socket\n");
    close(server_fd);
    return 0;
}