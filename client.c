/*漂流瓶客户端程序
	编写者:张静
	日期:2015 12 1
	版本:0.2 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE 80
#define SERV_PORT 8000

int main(int argc, char *argv[])
{
	struct sockaddr_in servaddr;
	char buf[MAXLINE];
	int sockfd, n,x=0;
	char *str;
    
	if (argc != 2) {
		fputs("usage: ./client message\n", stderr);
		exit(1);
	}
	str = argv[1];
    
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);
    
	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	while(1)
	{//读取我的标准输入发送到服务端
			while(x==0)
			{
				printf("                ～漂流瓶～\n");
				printf("回车键捞取瓶子||直接输入字符后回车发送瓶子\n");
				x=1;
			}
			bzero(buf,MAXLINE);
			n=read(0,buf,MAXLINE);
			write(sockfd, buf, n);
//-----------------------------------------------
			bzero(buf,MAXLINE);
			n=read(sockfd,buf,MAXLINE);
			write(STDOUT_FILENO, buf, n);
	}
	close(sockfd);
	return 0;
}
