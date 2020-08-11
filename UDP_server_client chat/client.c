#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<netdb.h>

int main()
{
	int c_sock, n;
	char buff[255];
	c_sock = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in client;
	socklen_t *client_len;
	client_len = sizeof(client);
	client.sin_family = AF_INET;
	client.sin_port = htons(9009);
	client.sin_addr.s_addr = INADDR_ANY;
	while(1)
	{
		bzero(buff, 255);
		fgets(buff, 255, stdin);
		n = sendto(c_sock, buff, sizeof(buff), 0, (struct sockaddr*)&client, sizeof(client));
		bzero(buff, 255);
		n = recvfrom(c_sock, buff, sizeof(buff), 0, (struct sockaddr*)&client, client_len);
		printf("Server: %s\n", buff);
		
	}

	close(c_sock);
	return 0;
}
