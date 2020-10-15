#include<stdio.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
int c_socket;
char arr[100];
int arr_size;
/*In these protocols, the sender has a buffer called the sending window and the receiver has buffer called the receiving window.
The size of the sending window determines the sequence number of the outbound frames. If the sequence number of the frames is an n-bit field, then the range of sequence numbers that can be assigned is 0 to 2𝑛−1. 
Consequently, the size of the sending window is 2𝑛−1. Thus in order to accommodate a sending window size of 2𝑛−1, a n-bit sequence number is chosen.
The sequence numbers are numbered as modulo-n. For example, if the sending window size is 4, then the sequence numbers will be 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, and so on. 
The number of bits in the sequence number is 2 to generate the binary sequence 00, 01, 10, 11.
The size of the receiving window is the maximum number of frames that the receiver can accept at a time. It determines the maximum number of frames that the sender can send before receiving acknowledgment.*/


int ClientCreate(int port, int anyip, char IPADDR[])		// Return 1 for error
{
	c_socket = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in client;
	memset(&client, 0, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port = htons(port);
	if(anyip == 0) client.sin_addr.s_addr = inet_addr(IPADDR);
	else client.sin_addr.s_addr = INADDR_ANY;
	if(connect(c_socket, (struct sockaddr *)&client, sizeof(client)) == -1)
	{
		printf("//////////////Connection Issue.\n");
		return 1;
	}
	else return 0;
}

int main()
{
	
	int exit = 0;

	while(exit == 0)
	{
		char ipaddr[20];
		printf("Enter IP Address: ");
		scanf("%s", ipaddr);

		int port;
		printf("Enter Port: ");
		scanf("%d", &port);

		int window_size = 1;
		printf("Enter Window Size: ");
		scanf("%d", &window_size);

		int error = ClientCreate(port, 0, ipaddr);
		if(error == 1)
		{
			close(c_socket);
			printf("Connection Issue.\n");
			//return 0;
		}
		else 
		{
			char data[100];
			printf("Enter data to send: ");
			scanf("%s", data);

			char ack[2];
			ack[0] = '1';
			ack[1] = '\0';

			char buf[2];
			buf[0] = '1';
			buf[1] = '\0';

			char exit[2];
			exit[0] = '0';
			exit[1] = '\0';

			int index = 0;
			while(index < strlen(data))
			{
				send(c_socket, exit, sizeof(exit), 0);
				for(int i=0;i<window_size;i++)
				{
					printf("Sending: %c\n", data[index + i]);
					buf[0] = data[index + i];
					send(c_socket, buf, sizeof(buf), 0);
				}
				int ack_check = -1;
				for(int i=0;i<window_size;i++)
				{
					recv(c_socket, ack, sizeof(ack), 0);
					printf("ACK: %s\n", ack);
					if(ack[0] == '0')
					{
						ack_check = i;
						i=window_size;
					}
				}
				if(ack_check == -1) index = index + window_size;
				else index = index + ack_check;
			}
			exit[0] = '1';
			send(c_socket, exit, sizeof(exit), 0);

		}

		printf("Do you want to exit(1): ");
		scanf("%d", &exit);

		if(exit != 0)
		{
			char exit_str[] = "/exit";
			send(c_socket, exit_str, sizeof(exit_str), 0);
			send(c_socket, exit_str, sizeof(exit_str), 0);
		}

		close(c_socket);
	}
	return 0;
}