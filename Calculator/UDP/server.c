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
	char n1[255],n2[255],op[255];
	struct sockaddr_in server, client;
	socklen_t client_len;
	s_sock = socket(AF_INET, SOCK_DGRAM, 0);
	server.sin_family = AF_INET;
	server.sin_port = htons(9009);
	server.sin_addr.s_addr = INADDR_ANY;
	bind(s_sock, (struct sockaddr*)&server, sizeof(server));
	client_len = sizeof(client);
	
		
		printf("Enter first number Client\n");
		recvfrom(s_sock, n1, sizeof(n1), 0, (struct sockaddr*)&client, &client_len);
        printf("First Number: %s\n",n1);
		int x=atoi(n1);

        printf("Enter Second number Client\n");
		recvfrom(s_sock, n2, sizeof(n2), 0, (struct sockaddr*)&client, &client_len);
        printf("Second Number: %s\n",n2);
		int y=atoi(n2);
		

		printf("Operation Advertisement\n1.Add\n2.Subtract\n3.Multiply\n4.Divide\n");
		recvfrom(s_sock, op, sizeof(op), 0, (struct sockaddr*)&client, &client_len);
        printf("You selected the following operation number: %s\n",op);
		int z=atoi(op);
		double result;

		 switch(z){
    	case 1: {result=x+y;break;}
    	case 2: {result=x-y;break;}
    	case 3: {result=x*y;break;}
    	case 4: {result=x/y;break;}
    	default: printf("Wrong input\n");
    }
    char res[20];
    sprintf(res, "%f", result);
		n = sendto(s_sock, res, sizeof(res), 0, (struct sockaddr*)&client, sizeof(client));
		
		

	close(s_sock);
	return 0;
}
