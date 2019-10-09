#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../headers/Worker.h"
#include "../headers/MessageBuffer.h"
#include "../headers/Logger.h"

#define on_error(...) { fprintf(stderr, __VA_ARGS__); fflush(stderr); exit(1); }

int StartTCPServer (int port, int buffsize, MessageBuffer* mb) {
 
  int server_fd, client_fd, err;
  struct sockaddr_in server, client;
  char buf[buffsize];

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) on_error("Could not create socket\n");

  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  int opt_val = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof opt_val);

  err = bind(server_fd, (struct sockaddr *) &server, sizeof(server));
  if (err < 0) on_error("Could not bind socket\n");

  err = listen(server_fd, 2000);
  if (err < 0) on_error("Could not listen on socket\n");

  printf("Server is listening on %d\n", port);
  char *cl_address;
  while (1) {
    logg("startloop",LOGREC);
    socklen_t client_len = sizeof(client);
    client_fd = accept(server_fd, (struct sockaddr *) &client, &client_len);
    logg("accept",LOGREC);

    if (client_fd < 0) {
      logg("Could not establish new connection",LOGREC);
      continue;}
    cl_address = inet_ntoa(client.sin_addr);
    int x1,x2;
    if(sscanf(cl_address, "10.0.%d.%d", &x1, &x2 ) != 2){
      close(client_fd);
      continue;
    }
    x1 = x1*100 + x2;
    int p = ntohs(client.sin_port);
    char p_s[10];
    sprintf(p_s, "%d",x1);
    p_s[4] = '\0';
    printf("Client Port %d\n",p);
    printf("CLIENT ADDRESS %s\n", cl_address);
    int read;

      logg("Reading", LOGREC);
    read = recv(client_fd, buf, buffsize, 0);
    if(read <= 0){
      close(client_fd);
      logg("ZERO LENGTH", LOGREC);
      continue;
    }
    buf[read] = '\0';
    printf("%s, \n REEEEEEEADDD %d\n",buf,read);
        
      
    
    WorkerData wd;
    wd.length = read;
    wd.message = buf;
    wd.address = p_s;
    char ptr[500];
    sprintf(ptr, "%s,%s",buf,p_s);
    logg(ptr, LOGREC);
    close(client_fd);
    logg("before Worker",LOGREC);
    GiveDataToWorker(mb,wd);
    logg("after Worker",LOGREC);
  }

  return 0;
}