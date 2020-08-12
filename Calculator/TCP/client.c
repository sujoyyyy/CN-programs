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
	int c_sock;
	char buf[20],n[20],n1[20],op[20],result[20];
	c_sock=socket(AF_INET,SOCK_STREAM,0);
	struct  sockaddr_in client;
	memset(&client, 0, sizeof(client));
	client.sin_family=AF_INET;
	client.sin_port=htons(9000);
	client.sin_addr.s_addr=INADDR_ANY;
	if(connect(c_sock,(struct sockaddr*)&client,sizeof(client))==-1)
	{
		printf("Server is busy\n");
		return 0;
	}
	recv(c_sock,buf,sizeof(buf),0);
	printf("%s\n",buf );
	printf("Enter first number\n");
	scanf("%s",n);
	send(c_sock,n,sizeof(n),0);
	printf("Enter second number\n");
	scanf("%s",n1);
	send(c_sock,n1,sizeof(n1),0);
	printf("Enter operation\n");
	scanf("%s",op);
	send(c_sock,op,sizeof(op),0);
	recv(c_sock,result,sizeof(result),0);
	printf("Result: %s\n",result);
	close(c_sock);
	return 0;
}