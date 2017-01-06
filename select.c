/*漂流瓶服务端程序
	编写者:张静
	日期:2015 12 1
	版本:0.2*/
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "wrap.h"
#define MAXLINE 80
#define SERV_PORT 8000
#define LEN sizeof(struct lb)
int n2=0;
struct lb 
{
	int Number;
	char pz[1][MAXLINE];
	struct lb *next;
};
int main(int argc, char **argv)
{	
	struct lb *p1=NULL,*p2=NULL,*head=NULL,*cz=NULL;
	srand(time(NULL));
	char plp[10][MAXLINE];
	int i,m=0, maxi, maxfd, listenfd, connfd, sockfd,randomNumber=0;
	int nready, client[FD_SETSIZE];
	ssize_t n;
	fd_set rset, allset;
	char buf[MAXLINE];
	char str[INET_ADDRSTRLEN];
	socklen_t cliaddr_len;
	struct sockaddr_in	cliaddr, servaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	Listen(listenfd, 20);

	maxfd = listenfd;		/* initialize */
	maxi = -1;			/* index into client[] array */
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1;	/* -1 indicates available entry */
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

	for ( ; ; ) {
		rset = allset;	/* structure assignment */
		nready = select(maxfd+1, &rset, NULL, NULL, NULL);
		if (nready < 0)
			perr_exit("select error");

		if (FD_ISSET(listenfd, &rset)) { /* new client connection */
			cliaddr_len = sizeof(cliaddr);
			connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);

			printf("received from %s at PORT %d\n",
			       inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
			       ntohs(cliaddr.sin_port));

			for (i = 0; i < FD_SETSIZE; i++)
				if (client[i] < 0) {
					client[i] = connfd; /* save descriptor */
					break;
				}
			if (i == FD_SETSIZE) {
				fputs("too many clients\n", stderr);
				exit(1);
			}

			FD_SET(connfd, &allset);	/* add new descriptor to set */
			if (connfd > maxfd)
				maxfd = connfd; /* for select */
			if (i > maxi)
				maxi = i;	/* max index in client[] array */

			if (--nready == 0)
				continue;	/* no more readable descriptors */
		}

		for (i = 0; i <= maxi; i++) {	/* check all clients for data */
			if ( (sockfd = client[i]) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset)) {
				if ( (n = Read(sockfd, buf, MAXLINE)) == 0) {
					/* connection closed by client */
					Close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				}
				 else{
//-----------------------------------------------------------------------------------------------					
			if(buf[0]=='\n')
				{	if(n2==0)
						{
							write(sockfd,"当前没有瓶子,请发送几个瓶子试试!",47);
						}
					else if(n2>0)
						{
							do
								{
									randomNumber=rand()%n2;	
								}while(randomNumber==0);
							write(sockfd,"捞到一个瓶子:",20);
							cz=head;
								while(1)				
								{
									if(cz->Number==randomNumber)
										{
											write(sockfd,cz->pz[0],strlen(cz->pz[0]));
											printf("客户捞走%d号瓶:",cz->Number);
											printf("瓶子内容:%s",cz->pz[0]);
											break;
										}
									else
											cz=cz->next;
								}	
						}			
				}
				else
				{ //*p1 *p2 *head=NULL1	;
					cz=p1=(struct lb *)malloc(LEN);
					if(head==NULL)
						{
							p2=p1;
							head=p1;
							head->Number=++n2;
							strcpy(head->pz[0],buf);
							bzero(buf,MAXLINE); 
							head->next=NULL;
							write(sockfd,"内容已储存:",17);
							write(sockfd,head->pz[0],n);									
						}
					else
						{
							p1->Number=++n2;
							strcpy(p1->pz[0],buf);
							bzero(buf,MAXLINE);	
							p2->next=p1; 
							p2=p2->next;
							p1->next=NULL;
							write(sockfd,"内容已存储:",17);
							write(sockfd,p1->pz[0],n);
						}
				}	
				 }
//-----------------------------------------------------------------------------------------------
	
				if (--nready == 0)
					break;	/* no more readable descriptors */
			}
		}
	}
}
