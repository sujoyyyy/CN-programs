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
struct sockaddr_in server_addr;
int l = sizeof(struct sockaddr_in);
int send_file(int fd, int sockfd)
{
  long int n=0,m;
    char buf[SIZE];

    n = read(fd, buf, SIZE);
  while (n)
  {
    if (n == -1)
    {
      perror("[-]Read fail\n");
      return EXIT_FAILURE;
    }
    
    m = sendto(sockfd, buf, n, 0, (struct sockaddr *)&server_addr, l);
    if (m == -1)
    {
      perror("[-]Send error\n");
      return EXIT_FAILURE;
    }
    bzero(buf, SIZE);
    n = read(fd, buf, SIZE);
  }
  m = sendto(sockfd, buf, n, 0, (struct sockaddr *)&server_addr, l);
}

int main(){
  char *ip = "127.0.0.1";
  int port = 8080;
  int e;

  int sockfd;
  struct sockaddr_in server_addr;
  int fp;
  char *filename = "land.BMP";

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("ERROR");
    exit(1);
  }
  printf("Socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e == -1) {
    perror("Error in socket");
    exit(1);
  }
	printf("Connected to Server.\n");
  fp = open(filename, O_RDONLY);
  if (fp == -1) {
    perror("[----Error in reading file.");
    exit(1);
  }

  send_file(fp, sockfd);
  printf("-------Data sent successfully.\n");

	printf("Closing the connection.\n");
  close(sockfd);

  return 0;
}
