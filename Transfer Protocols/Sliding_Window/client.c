#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
/*In these protocols, the sender has a buffer called the sending window and the receiver has buffer called the receiving window.
The size of the sending window determines the sequence number of the outbound frames. If the sequence number of the frames is an n-bit field, then the range of sequence numbers that can be assigned is 0 to 2𝑛−1. 
Consequently, the size of the sending window is 2𝑛−1. Thus in order to accommodate a sending window size of 2𝑛−1, a n-bit sequence number is chosen.
The sequence numbers are numbered as modulo-n. For example, if the sending window size is 4, then the sequence numbers will be 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, and so on. 
The number of bits in the sequence number is 2 to generate the binary sequence 00, 01, 10, 11.
The size of the receiving window is the maximum number of frames that the receiver can accept at a time. It determines the maximum number of frames that the sender can send before receiving acknowledgment.*/

int main()
{
        int sock,val,i,count,port;
        char recvdata[50],sentdata[50];
        struct sockaddr_in server_addr;
        printf("\n\n Client Running ...... ");
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("Socket");
            exit(1);
        }
        printf("\nEnter the port number");
        scanf("%d",&port);
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr= htonl(INADDR_ANY);
        bzero(&(server_addr.sin_zero),8);
        if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
        {
            perror("Connection Error");
            exit(1);
        }
        while(1)
        {
                //get the pack number from client
                printf("\n Enter packet number ");
                scanf("%d",&val);
                // sent the value to server
                send(sock,&val,sizeof(val),0);
                // get the data from the user
                printf("\n\n Enter data ");
                scanf("%s",sentdata);
                // sent the to server
                send(sock,sentdata,strlen(sentdata),0);
                   if(strcmp(sentdata,"end")==0)
                   break;
                // recev the result from server
                recv(sock,&count,sizeof(count),0);
                i=recv(sock,recvdata,50,0);
                recvdata[i]='\0';
                printf("\n %s %d",recvdata,count);
        }
        close(sock);
        return 0;
}
