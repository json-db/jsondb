#ifndef __HTTPD_H__
#define __HTTPD_H__

#include "net.h"

void readRequest(int client_fd, char *request);
void parseRequest(char *request, char *op, char *path, char *body);
void responseJson(int client_fd, char *json);
void responseFile(int client_fd, char *path);

#endif
