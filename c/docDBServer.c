#include <pthread.h>
#include "http/net.h"
#include "http/httpd.h"
#include "docDB.h"

DB ddb;
DB *db = &ddb;

void *serve(void *argu) {
  int client = *(int*) argu;
  if (client == -1) {
    printf("Can't accept");
    return NULL;
  }
  char request[TMAX], path[SMAX], op[SMAX], body[TMAX];
  readRequest(client, request);
  printf("===========request=============\n%s\n", request);
  parseRequest(request, op, path, body);
  printf("op=%s path=%s body=%s\n", op, path, body);
  char cmd[SMAX], param[SMAX];
  int n = sscanf(path, "/jsondb/%[^/]/%[^/]", cmd, param);
  if (n == 2) {
    char docs[DOCS_SIZE];
    if (strcmp(cmd, "match")==0) {
      dbMatch(db, param, NULL, docs, DOCS_SIZE);
    }
    responseText(client, 200, docs);
  } else {
    responseText(client, 400, "Bad Request");
  }
  sleep(1);
  close(client);
  return NULL;
}

int main(int argc, char *argv[]) {
  // docDB init
  utf8init();

  dbOpen(db, "./jdb");
  // http init
  int port = (argc >= 2) ? atoi(argv[1]) : PORT;
  net_t net;
  net_init(&net, TCP, SERVER, port, NULL);
  net_bind(&net);
  net_listen(&net);
  printf("Server started at: http://127.0.0.1:%d\n", net.port);
  while (1) {
    int client = net_accept(&net);
    pthread_t thread1;
    pthread_create(&thread1, NULL, &serve, &client);
  }
  dbClose(db);
}
