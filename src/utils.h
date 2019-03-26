#ifndef COPY_UTILS_H
#define COPY_UTILS_H

#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <psdk_inc/_ip_types.h>
#include <stdio.h>
#include <string.h>

#define WORD_SIZE_IN_BYTES 4

void throwError();

void send_all(int socket, char* buffer, int len);

void recv_all(int socket, char* buffer);

#endif //COPY_UTILS_H
