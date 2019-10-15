// See license.txt for details about licensing.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

typedef int (*fd_open_callback_t)(int);
typedef int (*fd_close_callback_t)(int);
typedef int (*fd_read_callback_t)(int, char*);

static void error(const char *msg) {
  perror(msg);
  exit(0);
}

#ifndef h_addr
#define h_addr h_addr_list[0]
#endif

int socket_client (char* hostname, int portno) {
  int sockfd, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  // printf("HOSTNAME %s PORTNO %d\n", hostname, portno);
  
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    return sockfd;
  server = gethostbyname(hostname);
  if (server == NULL) 
    return -1;
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, 
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
    return -1;
  return sockfd;
}

int socket_poll(int fd) {
  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;
  fd_set fdset;
  FD_ZERO(&fdset);
  FD_SET(fd, &fdset);
  return select(fd + 1, &fdset, NULL, NULL, &timeout) > 0;
}

int is_socket_closed(int fd) {
  char c;
  ssize_t x = recv(fd, &c, 1, MSG_PEEK);
  return x <= 0;
}

int socket_server(int portno, fd_open_callback_t do_open, fd_read_callback_t do_read, fd_close_callback_t do_close) {
  int listen_desc, conn_desc;
  int maxfd, maxi;
  fd_set tempset, savedset;
  int client[FD_SETSIZE], numready;
  struct sockaddr_in serv_addr, client_addr;

  // printf("PORTNO %d\n", portno);
  
  listen_desc = socket(AF_INET, SOCK_STREAM, 0);

  // printf("LISTEN FD %d\n", listen_desc);
  
  if(listen_desc < 0)
    return -1;

  bzero((char *)&serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  if (bind(listen_desc, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    return -1;

  listen(listen_desc, 5);

  maxfd = listen_desc;
  maxi = -1; 
  for (int i=0; i<FD_SETSIZE; i++)
    client[i] = -1;
  FD_ZERO(&savedset);
  FD_SET(listen_desc, &savedset);

  while(1) {
    tempset = savedset;
    numready = select(maxfd+1, &tempset, NULL, NULL, NULL); 

    if(FD_ISSET(listen_desc, &tempset)) {
      printf("new client connection\n");
      socklen_t size = sizeof(client_addr);
      conn_desc = accept(listen_desc, (struct sockaddr *)&client_addr, &size);
      int j;
      for (j=0; j<FD_SETSIZE; j++) {
        if(client[j] < 0) {
          client[j] = conn_desc;
          break;
        }
      }
      FD_SET(conn_desc, &savedset);
      if(conn_desc > maxfd)
        maxfd = conn_desc; 
      if(j > maxi)
        maxi = j; 
      do_open(conn_desc);
    }

    for (int k=0; k<=maxi; k++) {
      if(client[k] > 0) {
        if(FD_ISSET(client[k], &tempset)) {
          int num_bytes;
          const int MAX_SIZE = 256;
          char buff[MAX_SIZE];
          if( (num_bytes = read(client[k], buff, MAX_SIZE)) > 0) {
             buff[num_bytes] = '\0';
             do_read(client[k], buff);
          }

          if(num_bytes == 0) {
            do_close(client[k]);
            close(client[k]);
            FD_CLR(client[k], &savedset);
            client[k] = -1;
          }

          if(--numready <=0)
            break; 
        }
      }
    }
  }

  close(listen_desc);
  return 0;
}

int server_socket_open (int portno) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  int enable = 1;
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    return -1;

  // printf("LISTEN FD %d\n", listen_desc);
  
  if (fd < 0)
    return fd;

  struct sockaddr_in serv_addr;

  bzero((char *)&serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  if (bind(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    return -1;

  listen(fd, 5);

  return fd;
}

int server_socket_poll (int fd, int secs, int usecs) {
  struct timeval timeout;
  timeout.tv_sec  = secs;
  timeout.tv_usec = usecs;
  fd_set set;
  FD_ZERO(&set);
  FD_SET(fd, &set);
  int numready = select(fd+1, &set, NULL, NULL, &timeout); 

  if(FD_ISSET(fd, &set)) {
    struct sockaddr_in client_addr;
    socklen_t size = sizeof(client_addr);
    int conn_fd = accept(fd, (struct sockaddr *)&client_addr, &size);
    return conn_fd;
  }
  return -1;
}
  
// int do_open (int fd) {
//   return 0;
// }
// 
// int do_read (int fd, char* buf) {
//   printf("Received:- %s", buf);
//   return 0;
// }
// 
// int do_close (int fd) {
//   return 0;
// }
// 
// int main (int argc, char* argv[]) {
//   server(atoi(argv[1]), &do_open, &do_read, &do_close);
// }
