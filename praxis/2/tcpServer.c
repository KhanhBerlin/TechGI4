/*
############################################################################
#                                                                          #
# Copyright TU-Berlin, 2011-2015                                           #
# Die Weitergabe, Veröffentlichung etc. auch in Teilen ist nicht gestattet #
#                                                                          #
############################################################################
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAX_BUFFER_LENGTH 100

/* calculate gcd (geklaut TM) */
int gcd(int a, int b)
{
  int c;
  while (a != 0) {
     c = a;
     a = b%a;
     b = c;
  }
  return b;
}

void unpackData(unsigned char *buffer, unsigned int *a, unsigned int *b)
{
  *a = (buffer[0]<<8) | buffer[1];
  *b = (buffer[2]<<8) | buffer[3];
}

int packData(unsigned char *buffer, unsigned int a, unsigned int b)
{
  /* ******************************************************************
     TO BE DONE:  pack data
   ******************************************************************* */
  buffer[0] = (char)(a>>8);
  buffer[1] = (char)a;
  buffer[2] = (char)(b>>8);
  buffer[3] = (char)b;

  return 0;
}

int main(int argc, char *argv[])
{
  int sockfd, sockfd2;
  struct sockaddr_in their_addr; // connector's address information
  int server_port;
  int a = 0;
  int b = 0;

  printf("Streaming socket server example\n\n");

  /* check number of arguments */
  if (argc != 2) {
    fprintf(stderr,"Usage: tcpServer tcpPort\n");
    exit(EXIT_FAILURE);
  }

  /* initialize port number from give argument */
  server_port = atoi(argv[1]);

  /* create socket */
  sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  if (sockfd == -1) {
    fprintf(stderr, "creating socket failed!!!\n");
    exit(EXIT_FAILURE);
  }

  /* setup address */
  their_addr.sin_family = AF_INET;
  their_addr.sin_port = htons((uint16_t)server_port);
  their_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

  /* bind address to socket */
  int mybind = bind(sockfd, (struct sockaddr *)&their_addr, sizeof(their_addr));
  if (mybind < 0){
    fprintf(stderr, "binding address to socket %d failed\n", sockfd);
    exit(EXIT_FAILURE);
  }

  /* listen to socket */
  if (listen(sockfd, 8) == -1) {
    fprintf(stderr, "couldn't listen to socket %d\n", sockfd);
    exit(EXIT_FAILURE);
  }

  /* accept connection */
  int addrsize = sizeof(their_addr);
  sockfd2 = accept(sockfd, (struct sockaddr *)&their_addr, &addrsize);
  if (sockfd2 == -1) {
    fprintf(stderr, "failed to accept connection\n");
    exit(EXIT_FAILURE);
  }

  /* receave data and write into buffer */
  unsigned char buffer[4];
  if (read(sockfd2, &buffer, 4) == -1) {
    fprintf(stderr, "failed to read from socket %d\n", sockfd2);
    exit(EXIT_FAILURE);
  }

  /* unpack/print data */
  unpackData(buffer, &a, &b);
  printf("gcd of %d and %d is %d\n", a, b, gcd(a,b));

  return EXIT_SUCCESS;
}
