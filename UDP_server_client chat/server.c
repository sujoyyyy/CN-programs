#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>

int main(int argc, char *argv[])
{
	int s_sock, n;
	char buff[255];
	struct sockaddr_in server, client;
	socklen_t client_len;
	s_sock = socket(AF_INET, SOCK_DGRAM, 0);
	server.sin_family = AF_INET;
	server.sin_port = htons(9009);
	server.sin_addr.s_addr = INADDR_ANY;
	bind(s_sock, (struct sockaddr*)&server, sizeof(server));
	client_len = sizeof(client);
	while(1)
	{
		bzero(buff, 255);
		n = recvfrom(s_sock, buff, sizeof(buff), 0, (struct sockaddr*)&client, &client_len);
		printf("Client: %s\n", buff);
		bzero(buff, 255);
		fgets(buff, 255, stdin);
		n = sendto(s_sock, buff, sizeof(buff), 0, (struct sockaddr*)&client, sizeof(client));
		
		
	}
	close(s_sock);
	return 0;
}
