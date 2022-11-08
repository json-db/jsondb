#include <pthread.h>
#include "http/net.h"
#include "http/httpd.h"

void *serve(void *argu) {
  int client_fd = *(int*) argu;
  if (client_fd == -1) {
    printf("Can't accept");
    return NULL;
  }
  char request[TMAX], path[SMAX], op[SMAX], body[TMAX];
  readRequest(client_fd, request);
  printf("===========request=============\n%s\n", request);
  parseRequest(request, op, path, body);
  printf("op=%s path=%s body=%s\n", op, path, body);
  if (strstr(path, "/jsondb/") != NULL) {
    responseJson(client_fd, "{\"db\":\"JsonDB\"}");
  } else {
    responseJson(client_fd, "{\"error\":404}");
  }
  sleep(1);
  close(client_fd);
  return NULL;
}

int main(int argc, char *argv[]) {
  int port = (argc >= 2) ? atoi(argv[1]) : PORT;
  net_t net;
  net_init(&net, TCP, SERVER, port, NULL);
  net_bind(&net);
  net_listen(&net);
  printf("Server started at: http://127.0.0.1:%d\n", net.port);
  while (1) {
    int client_fd = net_accept(&net);
    pthread_t thread1;
    pthread_create(&thread1, NULL, &serve, &client_fd);
  }
}
