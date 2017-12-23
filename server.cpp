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

#define MAX_FD 1024
#define BUFFER_SIZE 1024


int main(int argc , char *argv[])
{
	assert(argc>1);
	int port = atoi(argv[1]);
    int sockfd = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    assert(sockfd >= 0);
    //AF_INET: using IPv4
    //SOCK_STREAM: using TCP
    //SOCK_DGRAM: using UDP
    //protocal: set to 0

    //socket setting
    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr,0,sizeof(sockaddr_in));
    server_addr.sin_family = PF_INET; //sockaddr_in is IPv4
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port); //using port, htons(Host TO Network Short integer)
    int retval = bind(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
    assert(!retval);
    retval = listen(sockfd,1024);
    assert(!retval);

	// 宣告 select() 使用的資料結構
	fd_set readset;
	fd_set working_readset;
	FD_ZERO(&readset);
	// 將 socket 檔案描述子放進 readset
	FD_SET(sockfd, &readset);

    //message
    char message[] = "welcome to server!";
    char buffer[BUFFER_SIZE];
    ssize_t sz;

	while (1)
	{
	    memcpy(&working_readset, &readset, sizeof(fd_set));
	    retval = select(MAX_FD, &working_readset, NULL, NULL, NULL);
	    assert(retval>0);

	    if(retval == 0) // 排除沒有事件的情形
	        continue;

	    for (int fd = 0; fd<MAX_FD; fd++) // 用迴圈列舉描述子
	    {
	        // 排除沒有事件的描述子
	        if (!FD_ISSET(fd, &working_readset))
	            continue;

	        // 分成兩個情形：接受新連線用的 socket 和資料傳輸用的 socket
	        if (fd == sockfd)
	        {
	            // sockfd 有事件，表示有新連線
	            socklen_t addrlen = sizeof(client_addr);
	            int client_fd = accept(fd, (struct sockaddr*) &client_addr, &addrlen);
	            if (client_fd >= 0)
	            	FD_SET(client_fd, &readset); // 加入新創的描述子，用於和客戶端連線
	            send(client_fd,message,strlen(message)+1,0);
	        }
	        else
	        {
	            // 這裡的描述子來自 accept() 回傳值，用於和客戶端連線
	            sz = recv(fd, buffer, BUFFER_SIZE, 0); // 接收資料

	            if (sz == 0) // recv() 回傳值為零表示客戶端已關閉連線
	            {
	            	printf("fd [%d] is closed.\n", fd);
	                // 關閉描述子並從 readset 移除
	                close(fd);
	                FD_CLR(fd, &readset);
	            }
	            else if (sz < 0) // 發生錯誤
	            {
	            	goto error; 
	            }
	            else // sz > 0，表示有新資料讀入
	            {
	            	printf("from fd [%d]: %s", fd, buffer); memset(buffer,0,sz);
	            }
	        }
	    }
	}
	error:
	perror("error");

    close(sockfd);
    return 0;
}