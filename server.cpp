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


int main(int argc , char *argv[])
{
    int sockfd = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    assert(sockfd >= 0);
    //AF_INET: using IPv4
    //SOCK_STREAM: using TCP
    //SOCK_DGRAM: using UDP
    //protocal: set to 0

    //socket setting
    struct sockaddr_in serverInfo;
    memset(&serverInfo,0,sizeof(sockaddr_in));
    serverInfo.sin_family = PF_INET; //sockaddr_in is IPv4
    serverInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    serverInfo.sin_port = htons(8700); //using port, htons(Host TO Network Short integer)
    int retval = bind(sockfd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
    assert(!retval);
    retval = listen(sockfd,1024);
    assert(!retval);

    //message
    char message[100] = "server sends to client";
    char inputBuffer[100];
    ssize_t sz;
    while(1)
    {
	    //connect
	    struct sockaddr_in client_addr;
	    socklen_t addrlen = sizeof(sockaddr_in);
	    int client_fd = accept(sockfd,(struct sockaddr*) &client_addr, &addrlen);
	    assert(client_fd >= 0);

	    sz = recv(client_fd,inputBuffer,sizeof(inputBuffer),0);
	    sz = send(client_fd,message,sizeof(message),0);
	    printf("receive form client: %s\n",inputBuffer);
	    close(client_fd);
        memset(message,0,100);
    }
    close(sockfd);
    return 0;
}