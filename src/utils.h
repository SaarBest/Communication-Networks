#ifndef COPY_UTILS_H
#define COPY_UTILS_H

#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define WORD_SIZE_IN_BYTES 4
#define MAX_MESSAGE_LENGTH 2000
#define MAX_CRED_LENGTH 16
#define MAX_COURSE_LINE_LENGTH 106
#define MAX_COURSE_NAME_LENGTH 101
#define MAX_TEXT_RATE_LENGTH 1025
#define END_MESSAGE "end"
#define OK_MESSAGE "ok"
#define FAIL_MESSAGE "fail"
#define EMPTY_STRING ""

//comment
typedef struct node {
    int sock;
    char* username;
    struct node * next;
} node_t;

void add_to_list(node_t **head, int val, char* username);

void remove_from_list(node_t ** head, int val);

int get_fd_max(node_t * head);

void throwError();

void throwInvalidArguments();

int is_int(char* str);

void send_all(int socket, char* buffer, int len);

void recv_all(int socket, char* buffer);

#endif //COPY_UTILS_H
