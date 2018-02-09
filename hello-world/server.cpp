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
const char response[] = 
	"HTTP/1.1 200 OK\r\n"
	"Content-Length: 11\r\n"
	"\r\n"
	"Hello World\n";

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
	//tm *Localtime;
	//Localtime = localtime(&Time);
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
	bind(servfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
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
		printf("Message from client:\n %s\n", buf);
		write(clnt_sock, response, sizeof(response));
		close(clnt_sock);
	}

	return 0;
}
