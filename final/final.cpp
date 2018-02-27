#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#define MAXLENGTH 65532

#include <iostream>
#include <map>
using namespace std;

map<string, string> filetype;
void mapinit()
{
	filetype.insert(pair<string, string>("html", "text/html"));
	filetype.insert(pair<string, string>("ico", "image/ico"));
	filetype.insert(pair<string, string>("css", "text/css"));
	filetype.insert(pair<string, string>("js", "application/js"));
	filetype.insert(pair<string, string>("json", "application/json"));
	filetype.insert(pair<string, string>("jpg", "image/jpg"));
	filetype.insert(pair<string, string>("jpeg", "image/jpeg"));
	filetype.insert(pair<string, string>("png", "image/png"));
	filetype.insert(pair<string, string>("gif", "image/gif"));
	return;
}

void url_process();

int servfd = -1;
const char response[] = 
	"HTTP/1.1 200 OK\r\n"
	"Content-Length: %d\r\n"
	"Content-Type:%s\r\n"
	"\r\n"
	"%s"
	"\0";

void sigint_handler(int signum)
{
	printf("SIGINT detected, the server is closed.\n");
	close(servfd);
	exit(0);
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
	if (ret == -1)
	{
		printf("bind failed. Please try it later.\n");
		return 0;
	}
	listen(servfd, 20);

	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size = sizeof(clnt_addr);
	int clnt_sock = -1;
	
	mapinit();

	while (1);
	//TODO: This program has not been fininshed!!!

}

