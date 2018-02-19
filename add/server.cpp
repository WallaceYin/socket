#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <time.h>

int servfd = -1;
char response[] = 
	"HTTP/1.1 200 OK\r\n"
	"Content-Length: %d\r\n"
	"\r\n"
	"Sum: %d\n"
	"\0";

void sigint_handler(int signum)
{
	printf("\nSIGINT detected, the server is closed\n");
	close(servfd);
	exit(0);
}

void welcome(void)
{
	time_t Time;
	Time = time(NULL);
	char *Timestr;
	Timestr = ctime(&Time);
	printf("Time: %s", Timestr);
	printf("The server has been open\n");
}

int main()
{
	servfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	signal(SIGINT, sigint_handler);

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(8080);
	int ret = bind(servfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (ret < 0)
	{
		printf("bind fail: Please try it later.\n");
		return 0;
	}
	listen(servfd, 20);

	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size = sizeof(clnt_addr);
	int clnt_sock = -1;

	welcome();
	while ((clnt_sock = accept(servfd, (struct sockaddr*)&clnt_addr, &clnt_addr_size)) != -1)
	{
		char buf[1024];
		memset(buf, 0, sizeof(buf));
		read(clnt_sock, buf, sizeof(buf)-1);
		//printf("%s\n", buf);	
		char *_header;
		_header = strtok(buf, "\r\n");
		char *_url;
		_url = strtok(_header, " ");
		_url = strtok(NULL, " ");
		char *Operator;
		int s[3];
		Operator = strtok(_url, "/");
		char *number;
		number = strtok(NULL, "/");
		s[0] = atoi(number);
		number = strtok(NULL, "/");
		s[1] = atoi(number);
		//printf("%d %d\n", s[0], s[1]);
		s[2] = s[0] + s[1];
		char sum[10];
		memset(sum, 0, sizeof(sum));
		sprintf(sum, "%d", s[2]);
		char clnt_send[1024];
		memset(clnt_send, 0, 1024);
		sprintf(clnt_send, response, strlen("Sum :") + strlen(sum), s[2]);
		write(clnt_sock, clnt_send, strlen(clnt_send));
		close(clnt_sock);
	}

	return 0;
}
