#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#define SIZE 8
off_t count=0;
void write_file(int new_sock)
{
  int n,m;
  int fd;
  char buffer[SIZE];
    if ((fd = open("new.BMP", O_CREAT | O_WRONLY, 0600)) == -1)
        {
            perror("open fail");
            exit(3);
        }
      else
        printf("[+]File opened succesfully.\n[+]Writing...\n");
        bzero(buffer, SIZE);
    
        n = recv(new_sock, buffer, SIZE, 0);
        while (n)
        {
            if (n == -1)
            {
                perror("[-]Recv fail");
                exit(5);
            }
            if ((m = write(fd, buffer, n)) == -1)
            {
                perror("[-]Write fail");
                exit(6);
            }
            count+=m;
            
            bzero(buffer, SIZE);
            n = recv(new_sock, buffer, SIZE, 0);
        }
  close(fd);        
  return;
}

int main(){
  char *ip = "127.0.0.1";
  int port = 8080;
  int e;

  int sockfd, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;
  char buffer[SIZE];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("Error");
    exit(1);
  }
  printf("Socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e < 0) {
    perror("-----Error in bind");
    exit(1);
  }
  printf("Binding DONE\n");

  if(listen(sockfd, 10) == 0){
		printf("Listening....\n");
	}else{
		perror("-----Error in listening");
    exit(1);
	}

  addr_size = sizeof(new_addr);
  new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
  write_file(new_sock);
  printf("Data written.\n");

  return 0;
}
