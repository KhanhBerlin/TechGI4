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


int packData(unsigned char *buffer, unsigned int a, unsigned int b) {
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
  int sockfd;
  struct sockaddr_in their_addr; // connector's address information
  struct hostent *he;
  int server_port;
  int a = 0;
  int b = 0;

  printf("Streaming socket client example\n\n");

  if (argc != 5) {
    fprintf(stderr,"Usage: tcpClient serverName serverPort int1 int2\n");
    exit(EXIT_FAILURE);
  }

  server_port = atoi(argv[2]);
  a = atoi(argv[3]);
  b = atoi(argv[4]);

  //Resolv hostname to IP Address
  if ((he=gethostbyname(argv[1])) == NULL) {  // get the host info
    herror("gethostbyname");
    exit(EXIT_FAILURE);
  }

  /* ******************************************************************
     TO BE DONE: Create socket
   ******************************************************************* */
  int mysock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
  if (mysock == -1) {
    fprintf(stderr, "creating socket failed!!!\n");
    exit(EXIT_FAILURE);
  }
  fprintf(stdout, "created socket %d successfully\n", mysock);

  //setup transport address
  their_addr.sin_family = AF_INET;
  their_addr.sin_port = htons((uint16_t)server_port);
  their_addr.sin_addr = *((struct in_addr *)he->h_addr);
  memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

  /* ******************************************************************
     TO BE DONE:  Binding
   ******************************************************************* */

  unsigned char buffer[4];

  packData(buffer, a, b);

  printf("hi1\n");

  /* ******************************************************************
     TO BE DONE:  Send data
   ******************************************************************* */
  int foo = sendto(mysock, &buffer, 4, 0, (struct sockaddr *)&their_addr, sizeof(their_addr));
  if (foo == -1) {
    fprintf(stderr, "failed to send data\n");
  } else {
    fprintf(stdout, "sent %d things\n", foo);
  }
  printf("hi2\n");

  /* ******************************************************************
     TO BE DONE:  Close socket
   ******************************************************************* */
  if (close(mysock) == -1) {
    fprintf(stderr, "couldn't close socket %d", mysock);
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
