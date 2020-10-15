#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
/*Sliding window protocols are data link layer protocols for reliable and sequential delivery of data frames. 
The sliding window is also used in Transmission Control Protocol.
In this protocol, multiple frames can be sent by a sender at a time before receiving an acknowledgment from the receiver.
The term sliding window refers to the imaginary boxes to hold frames. Sliding window method is also known as windowing.*/

int s_socket, s_server;

int ServerCreate(int port)		// Create connections setup from the receiver end return 1 if error
{
	s_socket = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server, other;
	memset(&server, 0, sizeof(server));
	memset(&other, 0, sizeof(other));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;

	
	if(bind(s_socket, (struct sockaddr*)&server, sizeof(server)) != -1)
	{
		printf("Server Running.....\n");
		listen(s_socket, 5);

		return 0;
		
	}
	else
	{
		return 1; //connection error
	} 
}

int AcceptNewClient()
{
	struct sockaddr_in other;
	memset(&other, 0, sizeof(other));
	socklen_t add = sizeof(other);

	s_server = accept(s_socket, (struct sockaddr*)&other, &add);
	if(s_server == -1) return 1;
	else 
	{
		printf("\n//////////Conection accepted from %s,%d\n",inet_ntoa(other.sin_addr),ntohs(other.sin_port));
		return 0;
	}
}


int main()
{
	
	int ex = 0; //exit status
	while(ex == 0)
	{
		int port;
		printf("Enter Port: ");
		scanf("%d", &port);

		int window_size = 1;
		printf("Enter Window Size: ");
		scanf("%d", &window_size);

		int accepterror = 0;
		int error = ServerCreate(port);
		accepterror = AcceptNewClient();
		if(error == 1 || accepterror != 0)
		{
			close(s_server);
			close(s_socket);
			printf("Server Issue.\n");
			return 0;
		}
		else 
		{
			printf("\nServer Waiting...\n");

			char data[100];
			int index = 0;

			char ack[2];
			ack[0] = '1';
			ack[1] = '\0';

			char ack_temp[2];

			char buf[2];

			char exit[2];
			int exloop = 0;
			while(exloop == 0)
			{
				recv(s_server, exit, sizeof(exit), 0);
				if(exit[0] == '1')
				{
					exloop = 1;
				}
				else 
				{
					for(int i=0;i<window_size;i++)
					{
						recv(s_server, buf, sizeof(buf), 0);
						printf("Buf: %s\n", buf);
						data[index] = buf[0];        //buffer size is equal to receiver window size
						index++;
					}
					
					
					for(int i=0;i<window_size;i++)
					{
						printf("Ack: ");     //send acknowledgement bits for each char received
						scanf("%s", ack_temp);
						ack[0] = ack_temp[0];
						
						send(s_server, ack, sizeof(ack), 0);
						if(ack[0] == '0')
						{
							index = index - window_size + i;
							i = window_size;
						}
					}
					
				}
			}
			printf("Data: %s\n", data);
		}

		close(s_server);
		close(s_socket);

		printf("Do you want to exit(1): ");
		scanf("%d", &ex);
	}

	return 0;
}