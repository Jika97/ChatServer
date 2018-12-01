#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <sys/socket.h>

#define BUFFER_SIZE 256

int cree_socket_tcp_client(int argc, char** argv)
{
  struct sockaddr_in adresse;
  int sock;

  if (argc != 3)
    {
      fprintf(stderr, "Usage : %s adresse port\n", argv[0]);
      exit(0);
    }  
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      fprintf(stderr, "Erreur socket\n");
      return -1;
    }
  
  memset(&adresse, 0, sizeof(struct sockaddr_in));
  adresse.sin_family = AF_INET;
  adresse.sin_port = htons(atoi(argv[2]));
  inet_aton(argv[1], &adresse.sin_addr);

  if (connect(sock, (struct sockaddr*) &adresse, 
           sizeof(struct sockaddr_in)) < 0)
  {
    close(sock);
    fprintf(stderr, "Erreur connect\n");
    return -1;
  }
  return sock;
}

int main(int argc, char**argv)
{
  int sock;
  char buffer[BUFFER_SIZE];
  sock = cree_socket_tcp_client(argc, argv);
  if (sock < 0)
    {
      puts("Erreur connection socket client");
      exit(1);
    }

  while(1)
    {
      if (read(sock, buffer, BUFFER_SIZE)==0)
        break;
      puts(buffer);
      if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
        break;
      buffer[strlen(buffer)-1] = '\0';
      write(sock, buffer, BUFFER_SIZE);
    } 
  return 0;
}