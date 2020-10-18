#include<stdio.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
/*Code Explanation:
Used in Connection-oriented communication.
It offers error and flow control
It is used in Data Link and Transport Layers
Stop and Wait ARQ mainly implements Sliding Window Protocol concept with Window Size 1
*/
int c_socket;

char arr[100];
int arr_size;

int ClientCreate(int port, int anyip, char IPADDR[])		 //create the client socket
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
		printf("Connection Issue.\n"); //returns 1 if error in connecting
		return 1;
	}
	else return 0;
}
/*Rule 1) Send one data packet at a time.
Rule 2) Send next packet only after receiving acknowledgement for previous.*/
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

		int error = ClientCreate(port, 0, ipaddr); //calling the client create function for the provided credentials
		if(error == 1)
		{
			close(c_socket);
			printf("Connection Issue.\n");
			return 0;
		}
		else 
		{
			char data[100];
			printf("Enter data to send: ");
			scanf("%s", data); //message that has to be transmitted 

			char ack[2];     //null terminated acknowledgement array
			ack[0] = '1';
			ack[1] = '\0';

			char buf[2];
			buf[0] = '1';
			buf[1] = '\0';

			char exit[2];
			exit[0] = '0';
			exit[1] = '\0';

			int index = 0;
			while(index < strlen(data))    //looping through the message
			{
				printf("Sending: %c\n", data[index]);
				buf[0] = data[index];
				send(c_socket, exit, sizeof(exit), 0);
				send(c_socket, buf, sizeof(buf), 0);
				recv(c_socket, ack, sizeof(ack), 0);     //receive acknowledgement for each char sent
				printf("ACK: %s\n", ack);
				if(ack[0] == '1') index++;
			}
			exit[0] = '1';
			send(c_socket, exit, sizeof(exit), 0);

			
		}

		printf("Do you want to exit(1): ");
		scanf("%d", &exit);      //exiting the client transmitter end

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