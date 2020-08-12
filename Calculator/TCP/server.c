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
	int s_sock,c_sock;
	char buf[20]="Hello Client";
	char n[20],n1[20],op[20];
	s_sock= socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in server, other;
	memset(&server,0,sizeof(server));
	memset(&other,0,sizeof(other));
	server.sin_family=AF_INET;
	server.sin_port=htons(9000);
	server.sin_addr.s_addr=INADDR_ANY;
	socklen_t add;
	if(bind(s_sock,(struct sockaddr*)&server,sizeof(server))==-1)
	{
		printf("Server busy\n");
		return 0;	}
	listen(s_sock,100);
	add=sizeof(other);
	c_sock=accept(s_sock,(struct sockaddr*)&other,&add);
	send(c_sock,buf,sizeof(buf),0);
	printf("Enter first number Client\n");
    recv(c_sock,n,sizeof(n),0);
    printf("First Number: %s\n",n);
    printf("Enter second number Client\n");
    recv(c_sock,n1,sizeof(n1),0);
    printf("Second Number: %s\n",n1);
    printf("Operation Advertisement\n1.Add\n2.Subtract\n3.Multiply\n4.Divide\n");
    recv(c_sock,op,sizeof(op),0);
    printf("You selected the following operation number: %s\n",op);
    int x=atoi(n);
    int y= atoi(n1);
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
    send(c_sock,res,sizeof(res),0);
	close(s_sock);
	return 0;
}